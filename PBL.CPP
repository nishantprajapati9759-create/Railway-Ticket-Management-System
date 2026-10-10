#include <iostream>
#include <string>
using namespace std;

const int TOTAL_SEATS = 2;
const int MAX_WAITING = 5;


// ************* PASSENGER'S INFORMATION CLASS **************

class PassengerInfo
{
private:
    string name;
    int age;
    int pnr;

public:

    // Constructor 1
    PassengerInfo()
    {
        name = "";
        age = 0;
        pnr = 0;
    }

    // Constructor 2 - Constructor Overloading
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


// ************ PNR GENERATION ************

int PNRnumber = 1001;

int generatePNR()
{
    int pnr = PNRnumber;
    PNRnumber++;

    return pnr;
}


// ************ WAITING QUEUE ************

class WaitingList
{
private:
    PassengerInfo waitingList[MAX_WAITING];

    int front;
    int rear;

public:

    WaitingList()
    {
        front = -1;
        rear = -1;
    }

    bool isEmpty()
    {
        if (front == -1)
            return true;
        else
            return false;
    }

    bool isFull()
    {
        if (rear == MAX_WAITING - 1)
            return true;
        else
            return false;
    }

    // Enqueue
    void enqueue(PassengerInfo passenger)
    {
        if (isFull())
        {
            cout << "Waiting list is full." << endl;
            return;
        }

        if (front == -1)
        {
            front = 0;
        }

        rear++;
        waitingList[rear] = passenger;

        cout << "PassengerInfo added to waiting list." << endl;
    }

    // Dequeue
    PassengerInfo dequeue()
    {
        PassengerInfo emptyPassenger;

        if (isEmpty())
        {
            return emptyPassenger;
        }

        PassengerInfo passenger = waitingList[front];

        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
        {
            front++;
        }

        return passenger;
    }

    void displayQueue()
    {
        if (isEmpty())
        {
            cout << "Waiting list is empty." << endl;
            return;
        }

        cout << "\n***** WAITING LIST *****" << endl;

        for (int i = front; i <= rear; i++)
        {
            cout << "\nPassenger " << i - front + 1 << endl;
            waitingList[i].display();
        }
    }
};


// ************ MAIN ************

int main()
{
    PassengerInfo confirmedList[TOTAL_SEATS];

    int bookedSeats = 0;

    WaitingList waitingQueue;

    int choice;

    do
    {
        cout << "\n********** RAILWAY RESERVATION **********" << endl;
        cout << "1. Book Ticket" << endl;
        cout << "2. Cancel Ticket" << endl;
        cout << "3. Display Confirmed Bookings" << endl;
        cout << "4. Display Waiting List" << endl;
        cout << "5. Exit" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;


        // ************ BOOK TICKET ************

        if (choice == 1)
        {
            string name;
            int age;

            cout << "\nEnter passenger name: ";
            cin >> name;

            cout << "Enter passenger age: ";
            cin >> age;

            PassengerInfo passenger(name, age);

            int pnr = generatePNR();
            passenger.setPNR(pnr);


            // Check seat availability
            if (bookedSeats < TOTAL_SEATS)
            {
                confirmedList[bookedSeats] = passenger;
                bookedSeats++;

                cout << "\nBooking Confirmed!" << endl;
                cout << "PNR: " << pnr << endl;
            }
            else
            {
                waitingQueue.enqueue(passenger);

                cout << "No seats available." << endl;
                cout << "PassengerInfo added to waiting list." << endl;
                cout << "PNR: " << pnr << endl;
            }
        }


        // ************ CANCEL TICKET ************

        else if (choice == 2)
        {
            int cancelPNR;

            cout << "\nEnter PNR to cancel: ";
            cin >> cancelPNR;

            int found = -1;

            // Find confirmed passenger
            for (int i = 0; i < bookedSeats; i++)
            {
                if (confirmedList[i].getPNR() == cancelPNR)
                {
                    found = i;
                    break;
                }
            }


            if (found == -1)
            {
                cout << "PNR not found in confirmed bookings." << endl;
            }
            else
            {
                cout << "\nBooking cancelled for: "
                     << confirmedList[found].getName()
                     << endl;


                // Move remaining passengers one position left
                for (int i = found; i < bookedSeats - 1; i++)
                {
                    confirmedList[i] = confirmedList[i + 1];
                }

                bookedSeats--;


                // Check waiting list
                if (!waitingQueue.isEmpty())
                {
                    PassengerInfo nextPassenger = waitingQueue.dequeue();

                    confirmedList[bookedSeats] = nextPassenger;
                    bookedSeats++;

                    cout << "\nWaiting passenger is now confirmed!" << endl;
                    cout << "Name: "
                         << nextPassenger.getName()
                         << endl;

                    cout << "PNR: "
                         << nextPassenger.getPNR()
                         << endl;
                }
            }
        }


        // ************ DISPLAY CONFIRMED BOOKINGS ************

        else if (choice == 3)
        {
            if (bookedSeats == 0)
            {
                cout << "\nNo confirmed bookings." << endl;
            }
            else
            {
                cout << "\n***** CONFIRMED BOOKINGS *****" << endl;

                for (int i = 0; i < bookedSeats; i++)
                {
                    cout << "\nPassenger " << i + 1 << endl;
                    confirmedList[i].display();
                }
            }
        }


        // ************ DISPLAY WAITING LIST ************

        else if (choice == 4)
        {
            waitingQueue.displayQueue();
        }


        // ************ EXIT ************

        else if (choice == 5)
        {
            cout << "\nProgram ended." << endl;
        }


        else
        {
            cout << "\nInvalid choice." << endl;
        }

    } while (choice != 5);


    return 0;
}