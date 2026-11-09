#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;


// =====================================================
//                       PERSON CLASS
// =====================================================

class Person {

protected:
    string name;
    int age;
    string gender;

public:

    Person(string n = "", int a = 0, string g = "") {
        name = n;
        age = a;
        gender = g;
    }
};


// =====================================================
//                     PASSENGER CLASS
// =====================================================

class Passenger : public Person {

private:
    static int nextId;
    int passengerID;

public:

    Passenger(string n, int a, string g = "")
        : Person(n, a, g) {

        passengerID = nextId++;
    }


    int getID() const {
        return passengerID;
    }


    void display() const {

        cout << "\nPassenger ID: " << passengerID;
        cout << "\nName: " << name;
        cout << "\nAge: " << age;
        cout << "\nGender: " << gender;

        cout << "\n--------------------------------\n";
    }
};

int Passenger::nextId = 101;


// =====================================================
//                 PASSENGER MANAGEMENT
// =====================================================

class PassengerManagement {

private:
    list<Passenger> passengers;

public:


    // Add Passenger

    void addPassenger(string name, int age, string gender = "") {

        passengers.emplace_back(name, age, gender);

        cout << "\nPassenger Added Successfully!\n";
    }


    // Display Passengers

    void displayPassengers() {

        if (passengers.empty()) {

            cout << "\nNo Passengers Available!\n";
            return;
        }

        cout << "\n========== PASSENGER DETAILS ==========\n";

        for (auto it = passengers.begin();
             it != passengers.end(); it++) {

            it->display();
        }
    }


    // Search Passenger

    void searchPassenger(int id) {

        for (auto it = passengers.begin();
             it != passengers.end(); it++) {

            if (it->getID() == id) {

                cout << "\nPassenger Found!\n";

                it->display();

                return;
            }
        }

        cout << "\nPassenger Not Found!\n";
    }


    // Delete Passenger

    void deletePassenger(int id) {

        for (auto it = passengers.begin();
             it != passengers.end(); it++) {

            if (it->getID() == id) {

                passengers.erase(it);

                cout << "\nPassenger Deleted Successfully!\n";

                return;
            }
        }

        cout << "\nPassenger Not Found!\n";
    }
};


// =====================================================
//                       TRAIN CLASS
// =====================================================

class Train {

private:

    int trainNumber;
    string trainName;
    string source;
    string destination;

    int totalSeats = 20;

public:

    Train(int no, string name, string src, string dest) {

        trainNumber = no;
        trainName = name;
        source = src;
        destination = dest;
    }


    // Getter for Train Number

    int getTrainNumber() const {
        return trainNumber;
    }


    // Getter for Total Seats

    int getTotalSeats() const {
        return totalSeats;
    }


    // Display Train Details

    void displayTrain() const {

        cout << "\nTrain Number: " << trainNumber;
        cout << "\nTrain Name: " << trainName;
        cout << "\nSource: " << source;
        cout << "\nDestination: " << destination;
        cout << "\nTotal Seats: " << totalSeats;

        cout << "\n--------------------------------\n";
    }
};


// =====================================================
//                   TRAIN MANAGEMENT
// =====================================================

class TrainManagement {

private:

    vector<Train> trains;


    // Binary Search

    int binarySearch(int trainNo) {

        int low = 0;
        int high = static_cast<int>(trains.size()) - 1;

        while (low <= high) {

            int mid = low + (high - low) / 2;

            if (trains[mid].getTrainNumber() == trainNo) {

                return mid;
            }

            else if (trains[mid].getTrainNumber() < trainNo) {

                low = mid + 1;
            }

            else {

                high = mid - 1;
            }
        }

        return -1;
    }


public:


    // Constructor: Add 10 Predefined Trains

    TrainManagement() {

        trains.emplace_back(12001, "Shatabdi Express",
                            "Delhi", "Dehradun");

        trains.emplace_back(12002, "Jan Shatabdi",
                            "Dehradun", "Delhi");

        trains.emplace_back(12951, "Rajdhani Express",
                            "Mumbai", "Delhi");

        trains.emplace_back(12627, "Karnataka Express",
                            "Bangalore", "New Delhi");

        trains.emplace_back(12430, "Lucknow Mail",
                            "Lucknow", "Delhi");

        trains.emplace_back(12205, "Nanda Devi Express",
                            "Dehradun", "Kota");

        trains.emplace_back(12017, "Dehradun Express",
                            "Delhi", "Dehradun");

        trains.emplace_back(12301, "Howrah Rajdhani",
                            "Howrah", "New Delhi");

        trains.emplace_back(12615, "Grand Trunk Express",
                            "Chennai", "New Delhi");

        trains.emplace_back(12903, "Golden Temple Mail",
                            "Mumbai", "Amritsar");


        // Sort trains according to train number

        sort(trains.begin(), trains.end(),

             [](const Train& t1, const Train& t2) {

                 return t1.getTrainNumber() <
                        t2.getTrainNumber();
             });
    }


    // Display All Trains

    void displayTrains() {

        if (trains.empty()) {

            cout << "\nNo Trains Available!\n";
            return;
        }

        cout << "\n========== AVAILABLE TRAINS ==========\n";

        for (const auto& train : trains) {

            train.displayTrain();
        }
    }


    // Search Train Using Binary Search

    void searchTrain(int no) {

        int index = binarySearch(no);

        if (index != -1) {

            cout << "\nTrain Found Successfully!\n";

            trains[index].displayTrain();
        }

        else {

            cout << "\nTrain Not Found!\n";
        }
    }
};


// =====================================================
//                       MAIN FUNCTION
// =====================================================

int main() {

    PassengerManagement pm;
    TrainManagement tm;

    int choice;


    do {

        cout << "\n========================================";
        cout << "\n       RAILWAY MANAGEMENT SYSTEM";
        cout << "\n========================================";

        cout << "\n1. Passenger Management";
        cout << "\n2. Train Management";
        cout << "\n3. Exit";

        cout << "\nEnter Your Choice: ";

        cin >> choice;


        switch (choice) {


        // =============================================
        //             PASSENGER MANAGEMENT
        // =============================================

        case 1: {

            int ch;

            do {

                cout << "\n========== PASSENGER MANAGEMENT ==========";

                cout << "\n1. Add Passenger";
                cout << "\n2. Display Passengers";
                cout << "\n3. Search Passenger";
                cout << "\n4. Delete Passenger";
                cout << "\n5. Back To Main Menu";

                cout << "\nEnter Your Choice: ";

                cin >> ch;


                if (ch == 1) {

                    string name, gender;
                    int age;

                    cout << "\nEnter Passenger Name: ";

                    cin >> ws;
                    getline(cin, name);

                    cout << "Enter Age: ";
                    cin >> age;

                    cout << "Enter Gender: ";
                    cin >> gender;

                    pm.addPassenger(name, age, gender);
                }


                else if (ch == 2) {

                    pm.displayPassengers();
                }


                else if (ch == 3) {

                    int id;

                    cout << "\nEnter Passenger ID: ";
                    cin >> id;

                    pm.searchPassenger(id);
                }


                else if (ch == 4) {

                    int id;

                    cout << "\nEnter Passenger ID: ";
                    cin >> id;

                    pm.deletePassenger(id);
                }


                else if (ch == 5) {

                    cout << "\nReturning To Main Menu...\n";
                }


                else {

                    cout << "\nInvalid Choice! Try Again.\n";
                }

            } while (ch != 5);

            break;
        }


        // =============================================
        //                  TRAIN MANAGEMENT
        // =============================================

        case 2: {

            int ch;

            do {

                cout << "\n========== TRAIN MANAGEMENT ==========";

                cout << "\n1. Display All Trains";
                cout << "\n2. Search Train";
                cout << "\n3. Back To Main Menu";

                cout << "\nEnter Your Choice: ";

                cin >> ch;


                // Display Trains

                if (ch == 1) {

                    tm.displayTrains();
                }


                // Search Train

                else if (ch == 2) {

                    int no;

                    cout << "\nEnter Train Number: ";
                    cin >> no;

                    tm.searchTrain(no);
                }


                else if (ch == 3) {

                    cout << "\nReturning To Main Menu...\n";
                }


                else {

                    cout << "\nInvalid Choice! Try Again.\n";
                }

            } while (ch != 3);

            break;
        }


        // Exit

        case 3:

            cout << "\nThank You For Using Railway Management System!\n";

            break;


        default:

            cout << "\nInvalid Choice! Please Try Again.\n";
        }

    } while (choice != 3);


    return 0;
}
