#include <iostream>
#include <string>
using namespace std;

const int TOTAL_SEATS = 2;
const int MAX_WAITING = 5;
const int TABLE_SIZE = 101;


// ================= PASSENGER CLASS =================

class PassengerInfo
{
private:
    string name;
    int age;
    int pnr;

public:

    // Default Constructor
    PassengerInfo()
    {
        name = "";
        age = 0;
        pnr = 0;
    }

    // Parameterized Constructor
    PassengerInfo(string passengerName, int passengerAge)
    {
        name = passengerName;
        age = passengerAge;
        pnr = 0;
    }

    // Function Overloading
    void setPassenger(string passengerName)
    {
        name = passengerName;
    }

    void setPassenger(string passengerName, int passengerAge)
    {
        name = passengerName;
        age = passengerAge;
    }

    void setPNR(int passengerPNR)
    {
        pnr = passengerPNR;
    }

    string getName()
    {
        return name;
    }

    int getPNR()
    {
        return pnr;
    }

    void display()
    {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "PNR: " << pnr << endl;
    }
};


// ================= PNR GENERATION =================

int PNRnumber = 1001;

int generatePNR()
{
    int pnr = PNRnumber;
    PNRnumber++;

    return pnr;
}


// ================= WAITING QUEUE =================

class WaitingList
{
private:
    PassengerInfo waitingList[MAX_WAITING];

    int front;
    int rear;
    int count;

public:

    WaitingList()
    {
        front = 0;
        rear = -1;
        count = 0;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool isFull()
    {
        return count == MAX_WAITING;
    }

    // Enqueue
    bool enqueue(PassengerInfo passenger)
    {
        if (isFull())
        {
            return false;
        }

        rear = (rear + 1) % MAX_WAITING;
        waitingList[rear] = passenger;
        count++;

        return true;
    }

    // Dequeue
    PassengerInfo dequeue()
    {
        PassengerInfo passenger;

        if (isEmpty())
        {
            return passenger;
        }

        passenger = waitingList[front];

        front = (front + 1) % MAX_WAITING;
        count--;

        if (count == 0)
        {
            front = 0;
            rear = -1;
        }

        return passenger;
    }

    // Search and remove passenger from waiting list
    bool removeByPNR(int pnr)
    {
        PassengerInfo temp[MAX_WAITING];
        int tempCount = 0;
        bool found = false;

        while (!isEmpty())
        {
            PassengerInfo passenger = dequeue();

            if (passenger.getPNR() == pnr)
            {
                found = true;
            }
            else
            {
                temp[tempCount] = passenger;
                tempCount++;
            }
        }

        for (int i = 0; i < tempCount; i++)
        {
            enqueue(temp[i]);
        }

        return found;
    }

    void displayQueue()
    {
        if (isEmpty())
        {
            cout << "\nWaiting list is empty." << endl;
            return;
        }

        cout << "\n===== WAITING LIST =====" << endl;

        int i = front;

        for (int j = 0; j < count; j++)
        {
            cout << "\nPassenger " << j + 1 << endl;
            waitingList[i].display();

            i = (i + 1) % MAX_WAITING;
        }
    }
};


// ================= HASH TABLE =================

class HashTable
{
private:

    struct Record
    {
        int pnr;
        PassengerInfo passenger;
        int status;
        int state;
    };

    Record table[TABLE_SIZE];

public:

    HashTable()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i].state = 0;
            table[i].status = 0;
            table[i].pnr = 0;
        }
    }

    // Hash Function
    int hashFunction(int pnr)
    {
        return pnr % TABLE_SIZE;
    }

    // Insert passenger into hash table
    bool insert(PassengerInfo passenger, int status)
    {
        int index = hashFunction(passenger.getPNR());
        int firstDeleted = -1;

        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int position = (index + i) % TABLE_SIZE;

            if (table[position].state == 1 &&
                table[position].pnr == passenger.getPNR())
            {
                return false;
            }

            if (table[position].state == 2 && firstDeleted == -1)
            {
                firstDeleted = position;
            }

            if (table[position].state == 0)
            {
                if (firstDeleted != -1)
                {
                    position = firstDeleted;
                }

                table[position].pnr = passenger.getPNR();
                table[position].passenger = passenger;
                table[position].status = status;
                table[position].state = 1;

                return true;
            }
        }

        if (firstDeleted != -1)
        {
            table[firstDeleted].pnr = passenger.getPNR();
            table[firstDeleted].passenger = passenger;
            table[firstDeleted].status = status;
            table[firstDeleted].state = 1;

            return true;
        }

        return false;
    }

    // Search PNR
    int search(int pnr)
    {
        int index = hashFunction(pnr);

        for (int i = 0; i < TABLE_SIZE; i++)
        {
            int position = (index + i) % TABLE_SIZE;

            if (table[position].state == 0)
            {
                return -1;
            }

            if (table[position].state == 1 &&
                table[position].pnr == pnr)
            {
                return position;
            }
        }

        return -1;
    }

    // Delete PNR
    void remove(int pnr)
    {
        int position = search(pnr);

        if (position != -1)
        {
            table[position].state = 2;
        }
    }

    // Update passenger status
    void updateStatus(int pnr, int status)
    {
        int position = search(pnr);

        if (position != -1)
        {
            table[position].status = status;
        }
    }

    // Display passenger using PNR
    void displayPassenger(int pnr)
    {
        int position = search(pnr);

        if (position == -1)
        {
            cout << "\nPNR not found." << endl;
            return;
        }

        cout << "\n===== PASSENGER DETAILS =====" << endl;

        table[position].passenger.display();

        if (table[position].status == 1)
        {
            cout << "Status: Confirmed" << endl;
        }
        else
        {
            cout << "Status: Waiting" << endl;
        }
    }

    int getStatus(int pnr)
    {
        int position = search(pnr);

        if (position == -1)
        {
            return 0;
        }

        return table[position].status;
    }
};

int main()
{
    PassengerInfo confirmedPassengers[TOTAL_SEATS];

    int confirmedCount = 0;

    WaitingList waitingQueue;
    HashTable hashTable;

    int choice;

    do
    {
        cout << "\n========== RAILWAY RESERVATION ==========" << endl;
        cout << "1. Book Ticket" << endl;
        cout << "2. Cancel Ticket" << endl;
        cout << "3. Display Confirmed Bookings" << endl;
        cout << "4. Display Waiting List" << endl;
        cout << "5. Search Passenger by PNR" << endl;
        cout << "6. Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;


        // ================= BOOK TICKET =================

        if (choice == 1)
        {
            string name;
            int age;

            cout << "\nEnter passenger name: ";
            cin >> name;

            cout << "Enter passenger age: ";
            cin >> age;

            // Check waiting list capacity first
            if (confirmedCount == TOTAL_SEATS &&
                waitingQueue.isFull())
            {
                cout << "\nNo seats or waiting-list spaces available." << endl;
            }
            else
            {
                PassengerInfo passenger(name, age);

                int pnr = generatePNR();
                passenger.setPNR(pnr);

                if (confirmedCount < TOTAL_SEATS)
                {
                    confirmedPassengers[confirmedCount] = passenger;
                    confirmedCount++;

                    hashTable.insert(passenger, 1);

                    cout << "\nBooking Confirmed!" << endl;
                    cout << "PNR: " << pnr << endl;
                }
                else
                {
                    if (waitingQueue.enqueue(passenger))
                    {
                        hashTable.insert(passenger, 2);

                        cout << "\nPassenger added to waiting list." << endl;
                        cout << "PNR: " << pnr << endl;
                    }
                }
            }
        }


        // ================= CANCEL TICKET =================

        else if (choice == 2)
        {
            int cancelPNR;

            cout << "\nEnter PNR to cancel: ";
            cin >> cancelPNR;

            int position = hashTable.search(cancelPNR);

            if (position == -1)
            {
                cout << "\nPNR not found." << endl;
            }
            else
            {
                int status = hashTable.getStatus(cancelPNR);

                // Cancel confirmed ticket
                if (status == 1)
                {
                    int found = -1;

                    for (int i = 0; i < confirmedCount; i++)
                    {
                        if (confirmedPassengers[i].getPNR() == cancelPNR)
                        {
                            found = i;
                            break;
                        }
                    }

                    if (found != -1)
                    {
                        cout << "\nBooking cancelled for: "
                             << confirmedPassengers[found].getName()
                             << endl;

                        for (int i = found; i < confirmedCount - 1; i++)
                        {
                            confirmedPassengers[i] = confirmedPassengers[i + 1];
                        }

                        confirmedCount--;

                        hashTable.remove(cancelPNR);

                        // Confirm first waiting passenger
                        if (!waitingQueue.isEmpty())
                        {
                            PassengerInfo nextPassenger = waitingQueue.dequeue();

                            confirmedPassengers[confirmedCount] = nextPassenger;
                            confirmedCount++;

                            hashTable.updateStatus(nextPassenger.getPNR(), 1);

                            cout << "\nWaiting passenger is now confirmed!" << endl;
                            cout << "Name: " << nextPassenger.getName() << endl;
                            cout << "PNR: " << nextPassenger.getPNR() << endl;
                        }
                    }
                }

                // Cancel waiting-list ticket
                else if (status == 2)
                {
                    if (waitingQueue.removeByPNR(cancelPNR))
                    {
                        hashTable.remove(cancelPNR);

                        cout << "\nWaiting-list ticket cancelled." << endl;
                    }
                }
            }
        }


        // ================= DISPLAY CONFIRMED =================

        else if (choice == 3)
        {
            if (confirmedCount == 0)
            {
                cout << "\nNo confirmed bookings." << endl;
            }
            else
            {
                cout << "\n===== CONFIRMED BOOKINGS =====" << endl;

                for (int i = 0; i < confirmedCount; i++)
                {
                    cout << "\nPassenger " << i + 1 << endl;
                    confirmedPassengers[i].display();
                }
            }
        }


        // ================= DISPLAY WAITING =================

        else if (choice == 4)
        {
            waitingQueue.displayQueue();
        }


        // ================= SEARCH BY PNR =================

        else if (choice == 5)
        {
            int searchPNR;

            cout << "\nEnter PNR to search: ";
            cin >> searchPNR;

            hashTable.displayPassenger(searchPNR);
        }


        // ================= EXIT =================

        else if (choice == 6)
        {
            cout << "\nProgram ended." << endl;
        }

        else
        {
            cout << "\nInvalid choice." << endl;
        }

    } while (choice != 6);

    return 0;
}

