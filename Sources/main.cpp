#include <iostream>
#include <sstream>
#include <vector>
#include "Wallet.h"

using namespace std;

// Helper to split string by spaces
vector<string> parseCommand(string input) {
	vector<string> args;
	stringstream ss(input);
	string word;
	while (ss >> word) {
		args.push_back(word);
	}
	return args;
}

int main() {
	CryptoWalletSystem system;
	User* loggedInUser = nullptr;
	string inputLine;

	system.tambahUser("Alice"); 
	system.tambahUser("Bob");   
	system.tambahTransaksi("U2", "AUTO", 500);

	while (true) {
		if (loggedInUser) {
			cout << "\ncryptoWallet(" << loggedInUser->idUser << ")~ ";
		} else {
			cout << "\ncryptoWallet~ ";
		}

		if (!getline(cin, inputLine)) break; 
		if (inputLine.empty()) continue;

		vector<string> args = parseCommand(inputLine);
		if (args.empty()) continue;

		string cmd = args[0];

		if (cmd == "exit" || cmd == "quit") {
			cout << "Bye." << endl;
			break;
		} 
		else if (cmd == "help") {
			cout << "Available commands:\n";
			cout << "  register <name>          : Create new user\n";
			cout << "  login <id>               : Login as user\n";
			cout << "  logout                   : Logout current user\n";
			cout << "  list                     : Show all users\n";
			cout << "  tree                     : Show global tree representation\n";
			cout << "  tx <amount>              : Add transaction (need login)\n";
			cout << "  verify <txid>            : Validate a transaction\n";
			cout << "  list_tx                  : show all transaction for current user\n";
		}
		else if (cmd == "register") {
			if (args.size() < 2) cout << "Error: missing name. Usage: register <name>" << endl;
			else system.tambahUser(args[1]);
		}
		else if (cmd =="list_tx"){
			if (!loggedInUser) {
				cout << "Error: You must be logged in to see the user transaction." << endl;
				continue;
			}
			system.printUserTxList(loggedInUser->idUser);
		}
		else if (cmd == "list") {
			system.tampilkanLaporan();
		}
		else if (cmd == "login") {
			if (args.size() < 2) {
				cout << "Error: missing ID. Usage: login <id>" << endl;
				continue;
			}
			User* found = system.login(args[1]);
			if (found) {
				loggedInUser = found;
				cout << "You are now logged in as " << found->nama << endl;
			} else {
				cout << "Login failed: User ID not found." << endl;
			}
		}
		else if (cmd == "logout") {
			loggedInUser = nullptr;
		}
		else if (cmd == "tree") {
			system.printGlobalTree();
		}
		else if (cmd == "verify") {
			if (args.size() < 2) {
				cout << "Error: usage -> verify <tx_id>" << endl;
				continue;
			}
			system.cariDanValidasiTransaksi(args[1]);
		}
		else if (cmd == "tx") {
			if (!loggedInUser) {
				cout << "Error: You must be logged in to make transactions." << endl;
				continue;
			}
			if (args.size() < 2) {
				cout << "Error: usage -> tx <amount>" << endl;
				continue;
			}
			double amount = stod(args[1]);

			system.tambahTransaksi(loggedInUser->idUser, "AUTO", amount);
		}
		else if (cmd == "verify") {
			if (args.size() < 3) {
				cout << "Error: usage -> verify <tx_id>" << endl;
				continue;
			}
			system.cariDanValidasiTransaksi(args[1]);
		}
		else {
			cout << "Unknown command: " << cmd << endl;
		}
	}

	return 0;
}
