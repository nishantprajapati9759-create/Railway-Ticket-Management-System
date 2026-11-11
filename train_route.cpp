#include "train_route.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

RailwayGraph::RailwayGraph() {
    loadRoutesFromFile();
}

void RailwayGraph::loadRoutesFromFile() {
    ifstream inFile(dataFile);
    if (!inFile) {
        cout << "[WARNING] '" << dataFile << "' not found. Starting with an empty route graph.\n";
        return;
    }

    adjList.clear();
    string trainNo, trainName, src, dest;
    int dist;

    while (inFile >> trainNo >> trainName >> src >> dest >> dist) {
        string info = trainNo + " (" + trainName + ")";
        addRoute(src, dest, dist, info, false);
    }
    inFile.close();
}

void RailwayGraph::saveRouteToFile(string trainNo, string trainName, string src, string dest, int distance) {
    ofstream outFile(dataFile, ios::app);
    if (outFile) {
        outFile << "\n" << trainNo << " " << trainName << " " << src << " " << dest << " " << distance;
        outFile.close();
    }
}

void RailwayGraph::addRoute(string src, string dest, int distance, string trainInfo, bool saveToDisk) {
    adjList[src].push_back({dest, distance, trainInfo});
    adjList[dest].push_back({src, distance, trainInfo}); // Two-way railway track

    if (saveToDisk) {
        saveRouteToFile("99999", trainInfo, src, dest, distance);
    }
}

void RailwayGraph::displayNetwork() {
    cout << "\n====================================================\n";
    cout << "          ACTIVE RAILWAY NETWORK TOPOLOGY           \n";
    cout << "====================================================\n";
    
    if (adjList.empty()) {
        cout << " No routes loaded. Ensure 'trains.txt' is in the root folder.\n";
        return;
    }

    for (const auto& station : adjList) {
        cout << " [" << station.first << "] connects to:\n";
        for (const auto& neighbor : station.second) {
            cout << "    ---> " << neighbor.destination << " (" << neighbor.distance << " km) via " << neighbor.trainInfo << "\n";
        }
    }
    cout << "====================================================\n";
}

void RailwayGraph::findShortestPath(string startStation, string endStation) {
    if (adjList.find(startStation) == adjList.end() || adjList.find(endStation) == adjList.end()) {
        cout << "\n[ERROR] One or both station names do not exist in 'trains.txt'.\n";
        cout << "Tip: Check spelling and capitalization (e.g., Delhi, Kashipur, Haridwar).\n";
        return;
    }

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    unordered_map<string, int> distances;
    unordered_map<string, string> parent;
    unordered_map<string, string> usedTrain;

    for (const auto& station : adjList) {
        distances[station.first] = INT_MAX;
    }

    distances[startStation] = 0;
    pq.push({0, startStation});

    while (!pq.empty()) {
        int currDist = pq.top().first;
        string currStation = pq.top().second;
        pq.pop();

        if (currDist > distances[currStation]) continue;
        if (currStation == endStation) break;

        for (const auto& edge : adjList[currStation]) {
            string nextStation = edge.destination;
            int weight = edge.distance;

            if (distances[currStation] + weight < distances[nextStation]) {
                distances[nextStation] = distances[currStation] + weight;
                parent[nextStation] = currStation;
                usedTrain[nextStation] = edge.trainInfo;
                pq.push({distances[nextStation], nextStation});
            }
        }
    }

    if (distances[endStation] == INT_MAX) {
        cout << "\n[ALERT] No connecting railway track exists between " << startStation << " and " << endStation << ".\n";
        return;
    }

    vector<string> path;
    string step = endStation;
    while (step != "") {
        path.push_back(step);
        if (step == startStation) break;
        step = parent[step];
    }
    reverse(path.begin(), path.end());

    cout << "\n====================================================\n";
    cout << "             OPTIMAL ROUTE CALCULATION              \n";
    cout << "====================================================\n";
    cout << " Origin      : " << startStation << "\n";
    cout << " Destination : " << endStation << "\n";
    cout << " Route Path  :\n";
    for (size_t i = 0; i < path.size(); i++) {
        cout << "   " << path[i];
        if (i < path.size() - 1) {
            cout << " --[" << usedTrain[path[i + 1]] << "]-->\n";
        }
    }
    cout << "\n Total Dist  : " << distances[endStation] << " km\n";
    cout << " Est. Fare   : Rs. " << distances[endStation] * 2 << " (at Rs. 2/km)\n";
    cout << "====================================================\n";
}

void runRoutePlanner(RailwayGraph& graph) {
    int choice = 0;
    do {
        cout << "\n********** TRAIN ROUTE & FARE PLANNER (MEMBER 4) **********\n";
        cout << "1. View Full Railway Network Map (Loaded from trains.txt)\n";
        cout << "2. Find Shortest Route & Train Info (Dijkstra's Algorithm)\n";
        cout << "3. Add New Railway Track (Saves to trains.txt)\n";
        cout << "4. Reload Routes from trains.txt\n";
        cout << "5. Return to Master Platform Menu\n";
        cout << "───────────────────────────────────────────────────────────\n";
        cout << "Enter selection: ";
        cin >> choice;

        if (choice == 1) {
            graph.displayNetwork();
        } 
        else if (choice == 2) {
            string src, dest;
            cout << "\nEnter Origin Station (e.g., Delhi): ";
            cin >> src;
            cout << "Enter Destination Station (e.g., Kashipur): ";
            cin >> dest;
            graph.findShortestPath(src, dest);
        } 
        else if (choice == 3) {
            string src, dest, trainName;
            int dist;
            cout << "\nEnter Train Name (use_underscores, e.g., Express_Special): ";
            cin >> trainName;
            cout << "Enter First Station Name: ";
            cin >> src;
            cout << "Enter Second Station Name: ";
            cin >> dest;
            cout << "Enter Track Distance (in km): ";
            cin >> dist;
            graph.addRoute(src, dest, dist, trainName, true);
            cout << "\n[SUCCESS] New bidirectional track added and saved to trains.txt!\n";
        }
        else if (choice == 4) {
            graph.loadRoutesFromFile();
            cout << "\n[SUCCESS] Reloaded all train routes from trains.txt!\n";
        }
    } while (choice != 5);
}