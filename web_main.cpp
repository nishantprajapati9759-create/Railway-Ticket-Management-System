#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <winsock2.h>
#include "member1.h"
#include "member2.h"
#include "member3.h"
#include "member4.h"
#include "train_route.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string urlDecode(const string& str)
{
    string ret;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '%' && i + 2 < str.length())
        {
            int value;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &value);
            ret += static_cast<char>(value);
            i += 2;
        }
        else if (str[i] == '+')
        {
            ret += ' ';
        }
        else
        {
            ret += str[i];
        }
    }
    return ret;
}

string getParam(const string& query, const string& key)
{
    string searchKey = key + "=";
    size_t pos = query.find(searchKey);
    if (pos == string::npos)
    {
        return "";
    }
    pos += searchKey.length();
    size_t endPos = query.find('&', pos);
    string val = (endPos == string::npos) ? query.substr(pos) : query.substr(pos, endPos - pos);
    return urlDecode(val);
}

string readFileContent(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        return "<h1>Error: index.html not found!</h1>";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
string escapeJson(const string& s)
{
    string out;
    for (char c : s)
    {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\r' || c == '\n') continue;
        else out += c;
    }
    return out;
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    RailwayGraph rg;
    map<string, SeatManager> trainSeats;
    map<string, WaitingList> trainWaitlists;
    PassengerManagement pm;
    HashTable hashTable;
    vector<PassengerInfo> confirmedPassengers;
    map<int, string> pnrToTrain;
    int nextPNR = 1001;

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 20);

    cout << "\n===========================================================\n";
    cout << "  INDIAN RAILWAYS WEB PORTAL IS LIVE!                      \n";
    cout << "  Open Chrome/Edge and go to: http://127.0.0.1:8080        \n";
    cout << "===========================================================\n";

    while (true)
    {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            continue;
        }

        char buffer[4096] = {0};
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            closesocket(clientSocket);
            continue;
        }

        string request(buffer);
        size_t lineEnd = request.find("\r\n");
        if (lineEnd == string::npos)
        {
            closesocket(clientSocket);
            continue;
        }

        string firstLine = request.substr(0, lineEnd);
        size_t startPath = firstLine.find(' ');
        if (startPath == string::npos)
        {
            closesocket(clientSocket);
            continue;
        }
        startPath++;
        size_t endPath = firstLine.find(' ', startPath);
        string fullUrl = firstLine.substr(startPath, endPath - startPath);

        string path = fullUrl;
        string query = "";
        size_t qPos = fullUrl.find('?');
        if (qPos != string::npos)
        {
            path = fullUrl.substr(0, qPos);
            query = fullUrl.substr(qPos + 1);
        }

        string contentType = "application/json; charset=UTF-8";
        string body = "{}";

        if (path == "/")
        {
            contentType = "text/html; charset=UTF-8";
            body = readFileContent("index.html");
        }
        else if (path == "/api/login")
        {
            string user = getParam(query, "user");
            string pass = getParam(query, "pass");
            stringstream ss;

            if (user.empty() || pass.empty())
            {
                ss << R"({"success":false,"message":"Please enter both username and password."})";
            }
            else if (!checkUsername(user))
            {
                ss << R"({"success":false,"message":"Username not found! Please register first."})";
            }
            else if (!checkPassword(user, pass))
            {
                ss << R"({"success":false,"message":"Incorrect password! Please try again."})";
            }
            else
            {
                ss << R"({"success":true,"user":")" << user << R"(","message":"Login verified! Welcome back."})";
            }
            body = ss.str();
        }
        else if (path == "/api/register")
        {
            string user = getParam(query, "user");
            string pass = getParam(query, "pass");
            string confirm = getParam(query, "confirm");
            stringstream ss;

            if (user.empty() || user.find(' ') != string::npos)
            {
                ss << R"({"success":false,"message":"Username cannot be empty or contain spaces."})";
            }
            else if (pass.find(' ') != string::npos)
            {
                ss << R"({"success":false,"message":"Password cannot contain spaces."})";
            }
            else if (checkUsername(user))
            {
                ss << R"({"success":false,"message":"Username already exists! Choose another."})";
            }
            else if (pass.length() < 8)
            {
                ss << R"({"success":false,"message":"Password must be at least 8 characters long!"})";
            }
            else if (pass != confirm)
            {
                ss << R"({"success":false,"message":"Passwords do not match!"})";
            }
            else
            {
                ofstream login("customer login details.txt", ios::app);
                login << user << " " << pass << endl;
                login.close();
                ss << R"({"success":true,"user":")" << user << R"(","message":"Account created in customer login details.txt!"})";
            }
            body = ss.str();
        }
        else if (path == "/api/seats")
        {
            string trainName = getParam(query, "train");
            if (trainName.empty()) trainName = "DEFAULT";
            SeatManager& sm = trainSeats[trainName];

            stringstream ss;
            ss << R"({"seats":[)";
            for (int s = 1; s <= 20; s++)
            {
                ss << (sm.isSeatAvailable(s) ? 0 : 1);
                if (s < 20) ss << ",";
            }
            ss << "]}";
            body = ss.str();
        }
        else if (path == "/api/routes")
        {
            string src = getParam(query, "src");
            string dest = getParam(query, "dest");
            vector<RouteOption> options = rg.findAllRoutesDFS(src, dest);

            stringstream ss;
            ss << R"({"routes":[)";
            for (size_t i = 0; i < options.size(); i++)
            {
                string trainStr = options[i].trains.front();
                if (options[i].trains.back() != options[i].trains.front())
                {
                    trainStr += " -> " + options[i].trains.back();
                }
                string pathStr = "";
                for (size_t j = 0; j < options[i].stations.size(); j++)
                {
                    pathStr += options[i].stations[j];
                    if (j + 1 < options[i].stations.size()) pathStr += " -> ";
                }
                ss << R"({"src":")" << escapeJson(src) << R"(","dest":")" << escapeJson(dest)
   << R"(","train":")" << escapeJson(trainStr)
   << R"(","startTime":")" << escapeJson(options[i].startTime)
   << R"(","endTime":")" << escapeJson(options[i].endTime)
   << R"(","distance":)" << options[i].totalDistance
   << R"(,"fare":)" << options[i].totalFare
   << R"(,"path":")" << escapeJson(pathStr) << R"("})";

                if (i + 1 < options.size()) ss << ",";
            }
            ss << "]}";
            body = ss.str();
        }
        else if (path == "/api/book")
        {
            string name = getParam(query, "name");
            int age = stoi(getParam(query, "age"));
            string src = getParam(query, "src");
            string dest = getParam(query, "dest");
            string train = getParam(query, "train");
            string timing = getParam(query, "timing");
            int reqSeat = stoi(getParam(query, "seat"));
            int fare = stoi(getParam(query, "fare"));

            SeatManager& sm = trainSeats[train];
            WaitingList& wl = trainWaitlists[train];

            int trainBookedCount = 0;
            for (size_t i = 0; i < confirmedPassengers.size(); i++)
            {
                if (pnrToTrain[confirmedPassengers[i].getPNR()] == train)
                {
                    trainBookedCount++;
                }
            }

            stringstream ss;
            if (trainBookedCount < TOTAL_SEATS)
            {
                int assignedSeat = reqSeat;
                if (assignedSeat <= 0 || !sm.isSeatAvailable(assignedSeat))
                {
                    assignedSeat = sm.autoAssignSeat();
                }
                else
                {
                    sm.bookSeat(assignedSeat);
                }

                PassengerInfo p(name, age, src, dest, train, timing, assignedSeat, fare);
                int pnr = nextPNR++;
                p.setPNR(pnr);
                pnrToTrain[pnr] = train;
                confirmedPassengers.push_back(p);
                hashTable.insert(p, 1);
                pm.addPassenger(name, age, "Web");

                ss << R"({"success":true,"pnr":)" << pnr << R"(,"seat":)" << assignedSeat
<< R"(,"message":"Ticket Confirmed for )" << train << R"(! PNR Generated."})";
}
else if (!wl.isFull())
{
PassengerInfo p(name, age, src, dest, train, timing, 0, fare);
int pnr = nextPNR++;
p.setPNR(pnr);
pnrToTrain[pnr] = train;
wl.enqueue(p);
hashTable.insert(p, 2);
pm.addPassenger(name, age, "Web");
ss << R"({"success":true,"pnr":)" << pnr << R"(,"seat":0)"
<< R"(,"message":"Coach Full! Added to FIFO Waiting List Queue."})";
}
else
{
ss << R"({"success":false,"message":"Coach and Waiting List both full!"})";
}
body = ss.str();
}
else if (path == "/api/pnr")
{
string pnrStr = getParam(query, "pnr");
int pnr = pnrStr.empty() ? 0 : stoi(pnrStr);
int status = hashTable.getStatus(pnr);
stringstream ss;
if (status == 0)
{
ss << R"({"found":false})";
}
else
{
string pName = "Passenger";
string tName = pnrToTrain[pnr];
int seatNo = 0;
for (size_t i = 0; i < confirmedPassengers.size(); i++)
{
if (confirmedPassengers[i].getPNR() == pnr)
{
pName = confirmedPassengers[i].getName();
seatNo = confirmedPassengers[i].getSeatNumber();
break;
}
}
ss << R"({"found":true,"pnr":)" << pnr
<< R"(,"status":")" << (status == 1 ? "CONFIRMED" : "WAITLISTED")
<< R"(","name":")" << pName
<< R"(","train":")" << tName
<< R"(","seat":)" << seatNo << "}";
}
body = ss.str();
}
else if (path == "/api/cancel")
{
string pnrStr = getParam(query, "pnr");
int pnr = pnrStr.empty() ? 0 : stoi(pnrStr);
int status = hashTable.getStatus(pnr);
stringstream ss;
int foundIdx = -1;
for (size_t i = 0; i < confirmedPassengers.size(); i++)
{
if (confirmedPassengers[i].getPNR() == pnr)
{
foundIdx = (int)i;
break;
}
}
if (foundIdx != -1)
{
string tName = pnrToTrain[pnr];
int freedSeat = confirmedPassengers[foundIdx].getSeatNumber();
SeatManager& sm = trainSeats[tName];
WaitingList& wl = trainWaitlists[tName];
sm.cancelSeat(freedSeat);
confirmedPassengers.erase(confirmedPassengers.begin() + foundIdx);
hashTable.remove(pnr);
if (!wl.isEmpty())
{
PassengerInfo nextP = wl.dequeue();
sm.bookSeat(freedSeat);
nextP.setSeatNumber(freedSeat);
confirmedPassengers.push_back(nextP);
hashTable.insert(nextP, 1);
}
ss << R"({"success":true,"message":"Ticket Cancelled! Seat S1-)" << freedSeat << R"( is now Available again."})";
}
else if (status == 2)
{
hashTable.remove(pnr);
ss << R"({"success":true,"message":"Waitlisted Ticket Cancelled!"})";
}
else
{
ss << R"({"success":false,"message":"PNR )" << pnr << R"( not found or already cancelled!"})";
}
body = ss.str();
}
stringstream response;
response << "HTTP/1.1 200 OK\r\n"
<< "Content-Type: " << contentType << "\r\n"
<< "Access-Control-Allow-Origin: *\r\n"
<< "Content-Length: " << body.length() << "\r\n"
<< "Connection: close\r\n\r\n"
<< body;
string resStr = response.str();
send(clientSocket, resStr.c_str(), (int)resStr.length(), 0);
closesocket(clientSocket);
}
closesocket(serverSocket);
WSACleanup();
return 0;
}