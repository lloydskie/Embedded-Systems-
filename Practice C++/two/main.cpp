
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct User {
	string cardNumber;
	string pin;
	double balance;
};

vector<User> users;

void loadUsers() {
	ifstream fin("users.txt");
	users.clear();
	User u;
	while (fin >> u.cardNumber >> u.pin >> u.balance) {
		users.push_back(u);
	}
	fin.close();
}

void saveUsers() {
	ofstream fout("users.txt");
	for (const auto& u : users) {
		fout << u.cardNumber << " " << u.pin << " " << u.balance << endl;
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

void printReceipt(const User& user, const string& transaction, double amount, double prevBalance) {
	static int receiptNo = 10001; // simple incrementing receipt number
	cout << "\n==============================\n";
	cout << "        ATM Transaction Record\n";
	cout << "Date/Time: " << getDateTime() << endl;
	cout << "Location: Main Branch\n";
	cout << "Terminal ID: ATM-001\n";
	cout << "Receipt No.: " << receiptNo++ << endl;
	cout << "Card No.: " << user.cardNumber << endl;
	cout << "Application ID: APP-ATM-2025\n";
	cout << "Application Label: Debit\n";
	cout << "Transaction: " << transaction << endl;
	cout << "From: ATM Machine\n";
	cout << "Amount: " << amount << endl;
	cout << "Current Balance: " << user.balance << endl;
	cout << "Available Balance: " << user.balance << endl;
	cout << "==============================\n";
	cout << "Enjoy the convenience of 24/7 banking.\n";
	cout << "Pay your bills via ATM and earn ATM rewards points!\n";
	cout << "Visit any ATM branch to know more.\n";
	cout << "==============================\n\n";
}

void atmMenu(int idx) {
	int choice;
	string lastTransaction = "Exit";
	double lastAmount = 0.0;
	double prevBalance = users[idx].balance;
	do {
		cout << "\nATM Menu:\n";
		cout << "1. Balance Inquiry\n";
		cout << "2. Cash Withdrawal\n";
		cout << "3. Deposit\n";
		cout << "4. Fund Transfer\n";
		cout << "5. Bill Payment\n";
		cout << "6. Exit\n";
		cout << "Select transaction: ";
		cin >> choice;
		switch (choice) {
			case 1:
				cout << "Your balance: " << users[idx].balance << endl;
				lastTransaction = "Balance Inquiry";
				lastAmount = 0.0;
				prevBalance = users[idx].balance;
				break;
			case 2: {
				double amt;
				cout << "Enter amount to withdraw: ";
				cin >> amt;
				if (amt > 0 && amt <= users[idx].balance) {
					prevBalance = users[idx].balance;
					users[idx].balance -= amt;
					cout << "Withdrawal successful. New balance: " << users[idx].balance << endl;
					lastTransaction = "Withdrawal";
					lastAmount = amt;
				} else {
					cout << "Insufficient balance or invalid amount.\n";
				}
				break;
			}
			case 3: {
				double amt;
				cout << "Enter amount to deposit: ";
				cin >> amt;
				if (amt > 0) {
					prevBalance = users[idx].balance;
					users[idx].balance += amt;
					cout << "Deposit successful. New balance: " << users[idx].balance << endl;
					lastTransaction = "Deposit";
					lastAmount = amt;
				} else {
					cout << "Invalid amount.\n";
				}
				break;
			}
			case 4: {
				string toCard;
				double amt;
				cout << "Enter recipient card number: ";
				cin >> toCard;
				int toIdx = findUser(toCard);
				if (toIdx == -1) {
					cout << "Recipient not found.\n";
					break;
				}
				cout << "Enter amount to transfer: ";
				cin >> amt;
				if (amt > 0 && amt <= users[idx].balance) {
					prevBalance = users[idx].balance;
					users[idx].balance -= amt;
					users[toIdx].balance += amt;
					cout << "Transfer successful.\n";
					lastTransaction = "Fund Transfer";
					lastAmount = amt;
				} else {
					cout << "Insufficient balance or invalid amount.\n";
				}
				break;
			}
			case 5: {
				double amt;
				cout << "Enter bill amount to pay: ";
				cin >> amt;
				if (amt > 0 && amt <= users[idx].balance) {
					prevBalance = users[idx].balance;
					users[idx].balance -= amt;
					cout << "Bill payment successful. New balance: " << users[idx].balance << endl;
					lastTransaction = "Bill Payment";
					lastAmount = amt;
				} else {
					cout << "Insufficient balance or invalid amount.\n";
				}
				break;
			}
			case 6:
				cout << "Please collect your card and receipt. Thank you!\n";
				printReceipt(users[idx], lastTransaction, lastAmount, prevBalance);
				break;
			default:
				cout << "Invalid choice.\n";
		}
		saveUsers();
	} while (choice != 6);
}

int main() {
	loadUsers();
	cout << "Welcome to the ATM Machine!\n";
	if (users.empty()) {
		cout << "No users found. Would you like to create a new bank account? (y/n): ";
		char ch;
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			User newUser;
			cout << "Enter new card number: ";
			cin >> newUser.cardNumber;
			cout << "Set your PIN: ";
			cin >> newUser.pin;
			cout << "Enter initial deposit: ";
			cin >> newUser.balance;
			users.push_back(newUser);
			saveUsers();
			cout << "Account created successfully! Please restart and login.\n";
		} else {
			cout << "Exiting.\n";
		}
		return 0;
	}

	string cardNumber, pin;
	cout << "Insert your card (enter card number): ";
	cin >> cardNumber;
	int idx = findUser(cardNumber);
	if (idx == -1) {
		cout << "Card not recognized.\n";
		cout << "1. Register as a new user\n";
		cout << "2. Exit the system\n";
		cout << "Select an option: ";
		int choice;
		cin >> choice;
		if (choice == 1) {
			User newUser;
			newUser.cardNumber = cardNumber;
			cout << "Set your PIN: ";
			cin >> newUser.pin;
			cout << "Enter initial deposit: ";
			cin >> newUser.balance;
			users.push_back(newUser);
			saveUsers();
			cout << "Account created successfully! Please restart and login.\n";
		} else {
			cout << "Exiting.\n";
		}
		return 0;
	}
	cout << "Enter PIN: ";
	cin >> pin;
	if (users[idx].pin != pin) {
		cout << "Incorrect PIN.\n";
		return 0;
	}
	atmMenu(idx);
	return 0;
}

