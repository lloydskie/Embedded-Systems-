#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
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

string getTier(int points) {
    if (points >= 5001) return "Platinum";
    if (points >= 2001) return "Gold";
    return "Silver";
}

int getPointsForTransaction(const string& type, const string& tier) {
    if (type == "Balance Inquiry") {
        if (tier == "Silver") return 1;
        if (tier == "Gold") return 2;
        if (tier == "Platinum") return 3;
    } else if (type == "Withdrawal") {
        if (tier == "Silver") return 2;
        if (tier == "Gold") return 3;
        if (tier == "Platinum") return 5;
    } else if (type == "Deposit") {
        if (tier == "Silver") return 3;
        if (tier == "Gold") return 5;
        if (tier == "Platinum") return 8;
    } else if (type == "Fund Transfer") {
        if (tier == "Silver") return 5;
        if (tier == "Gold") return 8;
        if (tier == "Platinum") return 12;
    } else if (type == "Bill Payment") {
        if (tier == "Silver") return 10;
        if (tier == "Gold") return 15;
        if (tier == "Platinum") return 20;
    }
    return 0;
}

enum ATMType { OWN_BANK, OTHER_BANK, INTERNATIONAL };

string getATMTypeName(ATMType type) {
    if (type == OWN_BANK) return "Own Bank ATM";
    if (type == OTHER_BANK) return "Other Bank ATM";
    return "International ATM";
}

double getTransactionFee(const string& type, ATMType atmType, double amount = 0.0) {
    if (type == "Balance Inquiry") {
        if (atmType == OWN_BANK) return 0.50;
        if (atmType == OTHER_BANK) return 1.00;
        if (atmType == INTERNATIONAL) return 2.00;
    } else if (type == "Withdrawal") {
        if (atmType == OWN_BANK) return 1.50;
        if (atmType == OTHER_BANK) return 2.50;
        if (atmType == INTERNATIONAL) return 5.00 + amount * 0.03; // 3% FX markup
    } else if (type == "Deposit") {
        if (atmType == OWN_BANK) return 1.00;
        return -1.0; // Not allowed
    } else if (type == "Fund Transfer") {
        if (atmType == OWN_BANK) return 1.00;
        if (atmType == OTHER_BANK) return 2.00;
        return -1.0; // Not available
    } else if (type == "Bill Payment") {
        if (atmType == OWN_BANK) return 1.00;
        if (atmType == OTHER_BANK) return 2.00;
        return -1.0; // Not available
    }
    return 0.0;
}

void printReceipt(const User& user, const string& transaction, double amount, double prevBalance, double fee = 0.0, string atmType = "Own Bank ATM") {
    static int receiptNo = 10001; // simple incrementing receipt number
    string tier = getTier(user.rewardsPoints);
    cout << MAGENTA << R"(
   ___________________________________________
  /                                           \
 |           ATM Transaction Record           |
 |--------------------------------------------|)" << endl;
    cout << " | Date/Time: " << left << setw(33) << getDateTime() << "|" << endl;
    cout << " | Location: " << left << setw(36) << "Main Branch" << "|" << endl;
    cout << " | Terminal ID: " << left << setw(33) << "ATM-001" << "|" << endl;
    cout << " | Receipt No.: " << left << setw(33) << receiptNo++ << "|" << endl;
    cout << " | Card No.: " << left << setw(36) << user.cardNumber << "|" << endl;
    cout << " | Application ID: " << left << setw(28) << "APP-ATM-2025" << "|" << endl;
    cout << " | Application Label: " << left << setw(25) << "Debit" << "|" << endl;
    cout << " | Transaction: " << left << setw(31) << transaction << "|" << endl;
    cout << " | ATM Type: " << left << setw(36) << atmType << "|" << endl;
    cout << " | Amount: " << left << setw(39) << amount << "|" << endl;
    cout << " | Fee Charged: $" << left << setw(32) << fee << "|" << endl;
    cout << " | Current Bal.: " << left << setw(33) << user.balance << "|" << endl;
    cout << " | Available Bal.: " << left << setw(31) << user.balance << "|" << endl;
    cout << " | Rewards Points: " << left << setw(30) << user.rewardsPoints << "|" << endl;
    cout << " | Tier: " << left << setw(39) << tier << "|" << endl;
    cout << " |--------------------------------------------|" << endl;
    cout << " | Enjoy the convenience of 24/7 banking.     |" << endl;
    cout << " | Pay your bills via ATM and earn rewards!   |" << endl;
    cout << " | Visit any ATM branch to know more.         |" << endl;
    cout << " |____________________________________________|" << RESET << endl;
}

void atmMenu(int idx) {
    int choice;
    string lastTransaction = "Exit";
    double lastAmount = 0.0;
    double prevBalance = users[idx].balance;
    double lastFee = 0.0;
    string lastATMType = "Own Bank ATM";
    do {
        string tier = getTier(users[idx].rewardsPoints);
        cout << CYAN << "\n+---------------------------------------------+\n";
        cout << "|                 ATM MENU                   |\n";
        cout << "+---------------------------------------------+\n";
        cout << "| Tier: " << tier << " (" << users[idx].rewardsPoints << " pts)";
        if (tier == "Silver") cout << "  [0-2000]";
        else if (tier == "Gold") cout << "  [2001-5000]";
        else cout << "  [5001+]";
        cout << "\n+---------------------------------------------+\n" << RESET;
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
        ATMType atmType = OWN_BANK;
        if (choice >= 1 && choice <= 5) {
            cout << CYAN << "\n+---------------------------------------------+\n";
            cout << "| Select ATM Type:                          |\n";
            cout << "| 1. Own Bank ATM                           |\n";
            cout << "| 2. Other Bank ATM                         |\n";
            cout << "| 3. International ATM                      |\n";
            cout << "+---------------------------------------------+\n" << RESET;
            int atmChoice;
            cin >> atmChoice;
            if (atmChoice == 2) atmType = OTHER_BANK;
            else if (atmChoice == 3) atmType = INTERNATIONAL;
            lastATMType = getATMTypeName(atmType);
        }
        switch (choice) {
            case 1: {
                int points = getPointsForTransaction("Balance Inquiry", tier);
                double fee = getTransactionFee("Balance Inquiry", atmType);
                users[idx].rewardsPoints += points;
                users[idx].balance -= fee;
                lastFee = fee;
                cout << GREEN << "Your balance: " << users[idx].balance << RESET << endl;
                cout << MAGENTA << "Your ATM Rewards Points: " << users[idx].rewardsPoints << RESET << endl;
                cout << MAGENTA << "You earned " << points << " points for Balance Inquiry!" << RESET << endl;
                cout << RED << "Fee charged: $" << fee << RESET << endl;
                lastTransaction = "Balance Inquiry";
                lastAmount = 0.0;
                prevBalance = users[idx].balance;
                break;
            }
            case 2: {
                double amt;
                cout << YELLOW << "Enter amount to withdraw: " << RESET;
                cin >> amt;
                double fee = getTransactionFee("Withdrawal", atmType, amt);
                if (amt > 0 && amt + fee <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt + fee;
                    int points = getPointsForTransaction("Withdrawal", tier);
                    users[idx].rewardsPoints += points;
                    lastFee = fee;
                    cout << GREEN << "Withdrawal successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " points for Withdrawal! Total: " << users[idx].rewardsPoints << RESET << endl;
                    cout << RED << "Fee charged: $" << fee << RESET << endl;
                    lastTransaction = "Withdrawal";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount (including fee)." << RESET << endl;
                }
                break;
            }
            case 3: {
                double amt;
                cout << YELLOW << "Enter amount to deposit: " << RESET;
                cin >> amt;
                double fee = getTransactionFee("Deposit", atmType);
                if (fee < 0) {
                    cout << RED << "Deposit not allowed at this ATM type." << RESET << endl;
                    break;
                }
                if (amt > 0 && fee <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance += amt;
                    users[idx].balance -= fee;
                    int points = getPointsForTransaction("Deposit", tier);
                    users[idx].rewardsPoints += points;
                    lastFee = fee;
                    cout << GREEN << "Deposit successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " points for Deposit! Total: " << users[idx].rewardsPoints << RESET << endl;
                    cout << RED << "Fee charged: $" << fee << RESET << endl;
                    lastTransaction = "Deposit";
                    lastAmount = amt;
                } else {
                    cout << RED << "Invalid amount or insufficient balance for fee." << RESET << endl;
                }
                break;
            }
            case 4: {
                string toCard;
                double amt;
                cout << YELLOW << "Enter recipient card number: " << RESET;
                cin >> toCard;
                int toIdx = findUser(toCard);
                double fee = getTransactionFee("Fund Transfer", atmType);
                if (fee < 0) {
                    cout << RED << "Fund Transfer not available at this ATM type." << RESET << endl;
                    break;
                }
                if (toIdx == -1) {
                    cout << RED << "Recipient not found." << RESET << endl;
                    break;
                }
                cout << YELLOW << "Enter amount to transfer: " << RESET;
                cin >> amt;
                if (amt > 0 && amt + fee <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt + fee;
                    users[toIdx].balance += amt;
                    int points = getPointsForTransaction("Fund Transfer", tier);
                    users[idx].rewardsPoints += points;
                    lastFee = fee;
                    cout << GREEN << "Transfer successful." << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " points for Fund Transfer! Total: " << users[idx].rewardsPoints << RESET << endl;
                    cout << RED << "Fee charged: $" << fee << RESET << endl;
                    lastTransaction = "Fund Transfer";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount (including fee)." << RESET << endl;
                }
                break;
            }
            case 5: {
                double amt;
                double fee = getTransactionFee("Bill Payment", atmType);
                if (fee < 0) {
                    cout << RED << "Bill Payment not available at this ATM type." << RESET << endl;
                    break;
                }
                cout << YELLOW << "Enter bill amount to pay: " << RESET;
                cin >> amt;
                if (amt > 0 && amt + fee <= users[idx].balance) {
                    prevBalance = users[idx].balance;
                    users[idx].balance -= amt + fee;
                    int points = getPointsForTransaction("Bill Payment", tier);
                    users[idx].rewardsPoints += points;
                    lastFee = fee;
                    cout << GREEN << "Bill payment successful. New balance: " << users[idx].balance << RESET << endl;
                    cout << MAGENTA << "You earned " << points << " points for Bill Payment! Total: " << users[idx].rewardsPoints << RESET << endl;
                    cout << RED << "Fee charged: $" << fee << RESET << endl;
                    lastTransaction = "Bill Payment";
                    lastAmount = amt;
                } else {
                    cout << RED << "Insufficient balance or invalid amount (including fee)." << RESET << endl;
                }
                break;
            }
            case 6:
                cout << GREEN << "Please collect your card and receipt. Thank you!" << RESET << endl;
                printReceipt(users[idx], lastTransaction, lastAmount, prevBalance, lastFee, lastATMType);
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

