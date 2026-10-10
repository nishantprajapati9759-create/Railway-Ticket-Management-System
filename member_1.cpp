#include <iostream>
#include <string>
#include <list>
using namespace std;

class Person {
protected:
    string name;
    int age;
    string gender;

public:
    Person(string n = "", int a = 0 , string g = "") {
        name = n;
        age = a;
        gender = g;
    }
};

class Passenger : public Person {
private:
    int passengerID;

public:
    Passenger(int id, string n, int a, string g = "")
        : Person(n, a, g) {
        passengerID = id;
    }

    int getID() const {
        return passengerID;
    }

    void display() const {
        cout << "ID: " << passengerID ;
        cout << ", Name: " << name ;
        cout << ", Age: " << age ;
        cout << ", Gender: " << gender << '\n';
    }
};

class PassengerManagement {
private:
    list<Passenger> passengers;

public:
   
    void addPassenger(int id, string name, int age, string gender = "") {
        passengers.emplace_back(id, name, age, gender);
        cout << "Passenger added!\n";
    }


    void displayPassengers() {
    for (auto it = passengers.begin();
         it != passengers.end();
         it++) {

        it->display();
    }
}

    void searchPassenger(int id)
{
    for (auto it = passengers.begin();
         it != passengers.end(); it++)
    {
        if (it->getID() == id)
        {
            cout << "Passenger Found!\n";
            it->display();
            return;
        }
    }

    cout << "Passenger Not Found!\n";
}

   void deletePassenger(int id)
{
    for (auto it = passengers.begin();
         it != passengers.end(); it++)
    {
        if (it->getID() == id)
        {
            passengers.erase(it);
            cout << "Passenger Deleted Successfully!\n";
            return;
        }
    }

    cout << "Passenger Not Found!\n";
}
};

int main() {
    PassengerManagement pm;

    pm.addPassenger(101, "Aditiya Rathi", 25, "Male");
    pm.addPassenger(102, "Aastha", 19, "Female");
    pm.addPassenger(103, "dhruvvvvvvvv", 22, "Male");

    cout << "\nAll Passengers:\n";
    pm.displayPassengers();

    cout << "\nSearching ID 102:\n";
    pm.searchPassenger(102);

    cout << "\nDeleting ID 101:\n";
    pm.deletePassenger(101);

    cout << "\nUpdated Passenger List:\n";
    pm.displayPassengers();

    return 0;
}
