#include <iostream>
#include <vector>
using namespace std;

class SeatManager {
private:
    // Fixed coach layout: 5 rows and 4 seats per row
    int seats[5][4];

    // Stores seats selected during a booking
    vector<int> selectedSeats;

public:
    // Default Constructor
    SeatManager() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                seats[i][j] = 0;  // 0 = Available
            }
        }

        cout << "Seat Manager initialized.\n";
    }

    // Display all seats
    void displaySeats() {
        cout << "\n------ COACH SEAT LAYOUT ------\n";
        cout << "A = Available, X = Booked\n\n";

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                int seatNumber = i * 4 + j + 1;

                cout << seatNumber << "(";
                if (seats[i][j] == 0)
                    cout << "A";
                else
                    cout << "X";

                cout << ")\t";
            }
            cout << endl;
        }
    }

    // Check seat availability
    void checkAvailability(int seatNumber) {
        if (seatNumber < 1 || seatNumber > 20) {
            cout << "Invalid seat number!\n";
            return;
        }

        int row = (seatNumber - 1) / 4;
        int col = (seatNumber - 1) % 4;

        if (seats[row][col] == 0)
            cout << "Seat " << seatNumber << " is available.\n";
        else
            cout << "Seat " << seatNumber << " is already booked.\n";
    }

    // Book a seat
    void bookSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > 20) {
            cout << "Invalid seat number!\n";
            return;
        }

        int row = (seatNumber - 1) / 4;
        int col = (seatNumber - 1) % 4;

        if (seats[row][col] == 1) {
            cout << "Seat is already booked!\n";
            return;
        }

        seats[row][col] = 1;
        selectedSeats.push_back(seatNumber);

        cout << "Seat " << seatNumber
             << " booked successfully!\n";
    }

    // Cancel a booking
    void cancelSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > 20) {
            cout << "Invalid seat number!\n";
            return;
        }

        int row = (seatNumber - 1) / 4;
        int col = (seatNumber - 1) % 4;

        if (seats[row][col] == 0) {
            cout << "Seat is already available!\n";
            return;
        }

        seats[row][col] = 0;

        // Remove the seat from the vector
        for (int i = 0; i < (int)selectedSeats.size(); i++) {
            if (selectedSeats[i] == seatNumber) {
                selectedSeats.erase(selectedSeats.begin() + i);
                break;
            }
        }

        cout << "Booking for seat " << seatNumber
             << " cancelled successfully!\n";
    }

    // Display available seats
    void showAvailableSeats() {
        cout << "\nAvailable seats: ";

        bool found = false;

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                if (seats[i][j] == 0) {
                    cout << i * 4 + j + 1 << " ";
                    found = true;
                }
            }
        }

        if (!found)
            cout << "None";

        cout << endl;
    }

    // Destructor
    ~SeatManager() {
        cout << "\nSeat Manager object destroyed.\n";
    }
};

int main() {
    SeatManager manager;
    int choice, seatNumber;

    do {
        cout << "\n===== RAILWAY SEAT MANAGEMENT =====\n";
        cout << "1. Display Seat Layout\n";
        cout << "2. Book a Seat\n";
        cout << "3. Cancel a Booking\n";
        cout << "4. Check Seat Availability\n";
        cout << "5. Show Available Seats\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.displaySeats();
                break;

            case 2:
                cout << "Enter seat number (1-20): ";
                cin >> seatNumber;
                manager.bookSeat(seatNumber);
                break;

            case 3:
                cout << "Enter seat number to cancel: ";
                cin >> seatNumber;
                manager.cancelSeat(seatNumber);
                break;

            case 4:
                cout << "Enter seat number to check: ";
                cin >> seatNumber;
                manager.checkAvailability(seatNumber);
                break;

            case 5:
                manager.showAvailableSeats();
                break;

            case 6:
                cout << "Exiting Seat Management System.\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}
