#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct User {
	string cardNumber;
	string pin;
	double balance;
	int rewardsPoints = 0;
};

vector<User> users;

void loadUsers() {
	ifstream fin("users.txt");
	users.clear();
	User u;
	while (fin >> u.cardNumber >> u.pin >> u.balance >> u.rewardsPoints) {
		users.push_back(u);
	}
	fin.close();
}

void saveUsers() {
	ofstream fout("users.txt");
	for (const auto& u : users) {
		fout << u.cardNumber << " " << u.pin << " " << u.balance << " " << u.rewardsPoints << endl;
	}
	fout.close();
}

int findUser(const string& cardNumber) {
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i].cardNumber == cardNumber) return i;
	}
	return -1;
}

// Helper to get current date and time as string
#include <ctime>
string getDateTime() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	char buf[64];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
	return string(buf);
}

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

void printReceipt(const User& user, const string& transaction, double amount, double prevBalance) {
    static int receiptNo = 10001; // simple incrementing receipt number
    cout << MAGENTA << R"(
   ___________________________________________
  /                                           \
 |           ATM Transaction Record           |
 |--------------------------------------------|
 | Date/Time: )" << getDateTime() << R"(           |
 | Location: Main Branch                      |
 | Terminal ID: ATM-001                       |
 | Receipt No.: )" << receiptNo++ << R"(                  |
 | Card No.: )" << user.cardNumber << R"(                      |
 | Application ID: APP-ATM-2025               |
 | Application Label: Debit                   |
 | Transaction: )" << transaction << R"(                  |
 | From: ATM Machine                          |
 | Amount: )" << amount << R"(                              |
 | Current Balance: )" << user.balance << R"(                  |
 | Available Balance: )" << user.balance << R"(                |
 | Rewards Points: )" << user.rewardsPoints << R"(                   |
 |--------------------------------------------|
 | Enjoy the convenience of 24/7 banking.     |
 | Pay your bills via ATM and earn rewards!   |
 | Visit any ATM branch to know more.         |
 |____________________________________________|
    )" << RESET << endl;
}

void atmMenu(int idx) {
    int choice;
    string lastTransaction = "Exit";
    double lastAmount = 0.0;
    double prevBalance = users[idx].balance;
    do {
        cout << CYAN << "\n+---------------------------------------------+\n";
        cout << "|                 ATM MENU                   |\n";
        cout << "+---------------------------------------------+\n";
        cout << YELLOW;
        cout << "| 1. Balance Inquiry                        |\n";
        cout << "| 2. Cash Withdrawal                        |\n";
        cout << "| 3. Deposit                                |\n";
        cout << "| 4. Fund Transfer                          |\n";
        cout << "| 5. Bill Payment                           |\n";
        cout << "| 6. Exit                                   |\n";
        cout << "+---------------------------------------------+\n";
        cout << "| Select transaction:                        |\n";
        cout << "+---------------------------------------------+\n" << RESET;
        cin >> choice;
        switch (choice) {
            case 1:
                cout << GREEN << "Your balance: " << users[idx].balance << RESET << endl;
                cout << MAGENTA << "Your ATM Rewards Points: " << users[idx].rewardsPoints << RESET << endl;
                lastTransaction = "Balance Inquiry";
                lastAmount = 0.0;
                prevBalance = users[idx].balance;
                break;
            case 2: {
                double amt;
                cout << YELLOW << "Enter amount to withdraw: " << RESET;
                cin >> amt;
                if (amt > 0 && amt <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt;
                    int points = static_cast<int>(amt / 100);
                    users[idx].rewardsPoints += points;
                    cout << GREEN << "Withdrawal successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " ATM Rewards Points! Total: " << users[idx].rewardsPoints << RESET << endl;
                    lastTransaction = "Withdrawal";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount." << RESET << endl;
                }
                break;
            }
            case 3: {
                double amt;
                cout << YELLOW << "Enter amount to deposit: " << RESET;
                cin >> amt;
                if (amt > 0) {
                    prevBalance = users[idx].balance;
                    users[idx].balance += amt;
                    int points = static_cast<int>(amt / 100);
                    users[idx].rewardsPoints += points;
                    cout << GREEN << "Deposit successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " ATM Rewards Points! Total: " << users[idx].rewardsPoints << RESET << endl;
                    lastTransaction = "Deposit";
                    lastAmount = amt;
                } else {
                    cout << RED << "Invalid amount." << RESET << endl;
                }
                break;
            }
            case 4: {
                string toCard;
                double amt;
                cout << YELLOW << "Enter recipient card number: " << RESET;
                cin >> toCard;
                int toIdx = findUser(toCard);
                if (toIdx == -1) {
                    cout << RED << "Recipient not found." << RESET << endl;
                    break;
                }
                cout << YELLOW << "Enter amount to transfer: " << RESET;
                cin >> amt;
                if (amt > 0 && amt <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt;
                    users[toIdx].balance += amt;
                    int points = static_cast<int>(amt / 100);
                    users[idx].rewardsPoints += points;
                    cout << GREEN << "Transfer successful." << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " ATM Rewards Points! Total: " << users[idx].rewardsPoints << RESET << endl;
                    lastTransaction = "Fund Transfer";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount." << RESET << endl;
                }
                break;
            }
            case 5: {
                double amt;
                cout << YELLOW << "Enter bill amount to pay: " << RESET;
                cin >> amt;
                if (amt > 0 && amt <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt;
                    int points = static_cast<int>(amt / 100);
                    users[idx].rewardsPoints += points;
                    cout << GREEN << "Bill payment successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " ATM Rewards Points! Total: " << users[idx].rewardsPoints << RESET << endl;
                    lastTransaction = "Bill Payment";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount." << RESET << endl;
                }
                break;
            }
            case 6:
                cout << GREEN << "Please collect your card and receipt. Thank you!" << RESET << endl;
                printReceipt(users[idx], lastTransaction, lastAmount, prevBalance);
                break;
            default:
                cout << RED << "Invalid choice." << RESET << endl;
        }
        saveUsers();
    } while (choice != 6);
}

int main() {
	loadUsers();
	// 3D ASCII Art ATM Welcome with color
	cout << CYAN << R"(
   ___________________________________________
  /                                           \
 |   _____   _______   __  __   _____   _____  |
 |  |  _  | |__   __| |  \/  | |  _  | |  _  | |
 |  | |_| |    | |    | |\/| | | |_| | | |_| | |
 |  |  _  |    | |    | |  | | |  _  | |  _  | |
 |  | | | |    | |    | |  | | | | | | | | | | |
 |  |_| |_|    |_|    |_|  |_| |_| |_| |_| |_| |
 |                                             |
 |         WELCOME TO ATM MACHINE              |
 |_____________________________________________|
    )" << RESET << endl;

    if (users.empty()) {
        cout << YELLOW << "\n+---------------------------------------------+\n";
        cout << "| No users found. Would you like to create   |\n";
        cout << "| a new bank account? (y/n):                |\n";
        cout << "+---------------------------------------------+\n" << RESET;
        char ch;
        cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            User newUser;
            cout << YELLOW << "\n+---------------------------------------------+\n";
            cout << "| Enter new card number:                    |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            cin >> newUser.cardNumber;
            cout << YELLOW << "\n+---------------------------------------------+\n";
            cout << "| Set your PIN:                             |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            cin >> newUser.pin;
            cout << YELLOW << "\n+---------------------------------------------+\n";
            cout << "| Enter initial deposit:                    |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            cin >> newUser.balance;
            newUser.rewardsPoints = static_cast<int>(newUser.balance / 100);
            users.push_back(newUser);
            saveUsers();
            cout << GREEN << "\n+---------------------------------------------+\n";
            cout << "| Account created successfully!              |\n";
            cout << "| Please restart and login.                  |\n";
            cout << "+---------------------------------------------+\n" << RESET;
        } else {
            cout << RED << "\n+---------------------------------------------+\n";
            cout << "| Exiting.                                  |\n";
            cout << "+---------------------------------------------+\n" << RESET;
        }
        return 0;
    }

    string cardNumber, pin;
    cout << YELLOW << "\n+---------------------------------------------+\n";
    cout << "| Insert your card (enter card number):      |\n";
    cout << "+---------------------------------------------+\n" << RESET;
    cin >> cardNumber;
    int idx = findUser(cardNumber);
    if (idx == -1) {
        cout << RED << "\n+---------------------------------------------+\n";
        cout << "| Card not recognized.                      |\n";
        cout << "| 1. Register as a new user                 |\n";
        cout << "| 2. Exit the system                        |\n";
        cout << "| Select an option:                         |\n";
        cout << "+---------------------------------------------+\n" << RESET;
        int choice;
        cin >> choice;
        if (choice == 1) {
            User newUser;
            newUser.cardNumber = cardNumber;
            cout << YELLOW << "\n+---------------------------------------------+\n";
            cout << "| Set your PIN:                             |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            cin >> newUser.pin;
            cout << YELLOW << "\n+---------------------------------------------+\n";
            cout << "| Enter initial deposit:                    |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            cin >> newUser.balance;
            newUser.rewardsPoints = static_cast<int>(newUser.balance / 100);
            users.push_back(newUser);
            saveUsers();
            cout << GREEN << "\n+---------------------------------------------+\n";
            cout << "| Account created successfully!              |\n";
            cout << "| Please restart and login.                  |\n";
            cout << "+---------------------------------------------+\n" << RESET;
        } else {
            cout << RED << "\n+---------------------------------------------+\n";
            cout << "| Exiting.                                  |\n";
            cout << "+---------------------------------------------+\n" << RESET;
        }
        return 0;
    }
    cout << YELLOW << "\n+---------------------------------------------+\n";
    cout << "| Enter PIN:                                |\n";
    cout << "+---------------------------------------------+\n" << RESET;
    cin >> pin;
    if (users[idx].pin != pin) {
        cout << RED << "\n+---------------------------------------------+\n";
        cout << "| Incorrect PIN.                            |\n";
        cout << "+---------------------------------------------+\n" << RESET;
        return 0;
    }
    atmMenu(idx);
    return 0;
}

