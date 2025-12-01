#include "Wallet.h"
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

CryptoWalletSystem::CryptoWalletSystem() {
	headUser = nullptr;
	headGlobalTx = nullptr;     
	globalMerkleRoot = nullptr; 
	globalMerkleHash = "EMPTY";
}

int CryptoWalletSystem::countUser = 0;
int CryptoWalletSystem::countTx = 0; 

void CryptoWalletSystem::updateGlobalMerkleTree() {
	vector<string> txHashes;
	Transaksi* curr = headGlobalTx;

	while (curr != nullptr) {
		string rawData = curr->idTx + curr->ownerId + to_string((int)curr->nominal);
		txHashes.push_back(computeHash(rawData));
		curr = curr->nextTx;
	}

	if (!txHashes.empty()) {
		if (globalMerkleRoot) deleteMerkleTree(globalMerkleRoot);
		globalMerkleRoot = buildMerkleRecursive(txHashes, 0, txHashes.size() - 1);
		globalMerkleHash = globalMerkleRoot->hash;
	}
}

void CryptoWalletSystem::tambahUser(string nama) {
	countUser++;
	User* newUser = new User();

	newUser->idUser = "U" + to_string(countUser);
	newUser->nama = nama;
	newUser->nextUser = nullptr;

	if (!headUser) {
		headUser = newUser;
	} else {
		User* temp = headUser;
		while (temp->nextUser) temp = temp->nextUser;
		temp->nextUser = newUser;
	}
	cout << "[SUCCESS] User " << nama << "with id: " << newUser ->idUser << endl;
}

void CryptoWalletSystem::tambahTransaksi(string targetIdUser, string idTx, double nominal) {
	User* currUser = login(targetIdUser);
	if (!currUser) {
		cout << "[ERROR] User not found" << endl;
		return;
	}

	Transaksi* newTx = new Transaksi;
	countTx++;

	if(idTx == "AUTO") {
		newTx->idTx = "TX-" + to_string(countTx);
	} else {
		newTx->idTx = idTx;
	}

	newTx->ownerId = currUser->idUser; 
	newTx->nominal = nominal;
	newTx->nextTx = nullptr;

	if (headGlobalTx == nullptr) {
		headGlobalTx = newTx;
	} else {
		Transaksi* t = headGlobalTx;
		while (t->nextTx) t = t->nextTx;
		t->nextTx = newTx;
	}

	currUser->myTransactions.push_back(newTx); 

	updateGlobalMerkleTree();
	cout << "[SUCCESS] Transaction has been added. (New hash block: " << globalMerkleHash << ")" << endl;
}

void CryptoWalletSystem::cariDanValidasiTransaksi(string idTx) {
	Transaksi* t = headGlobalTx;
	Transaksi* foundTx = nullptr;

	while (t != nullptr) {
		if (t->idTx == idTx) {
			foundTx = t;
			break;
		}
		t = t->nextTx;
	}

	if (!foundTx) {
		cout << "Tx not found in Global Ledger." << endl;
		return;
	}

	string rawData = foundTx->idTx + foundTx->ownerId + to_string((int)foundTx->nominal);
	string targetHash = computeHash(rawData);

	bool isValid = searchMerkleHash(globalMerkleRoot, targetHash);

	if (isValid) {
		cout << "Transaction Found in Global Block:" << endl;
		cout << "   ID      : " << foundTx->idTx << endl;
		cout << "   Owner   : " << foundTx->ownerId << endl;
		cout << "   Amount  : " << foundTx->nominal << endl;
		cout << "   Hash    : " << targetHash << endl;
		cout << "   Status  :  [VALID] Confirmed in Global Tree"<< endl;
	} else {
		cout << "[FALSE] Not valid in Global Tree" << endl;
	}
}

void CryptoWalletSystem::printGlobalTree() {
	cout << "=== GLOBAL MERKLE TREE ===" << endl;
	cout << "Root Hash: " << globalMerkleHash << endl;
	if (globalMerkleRoot) {
		printMerkleTree(globalMerkleRoot);
	} else {
		cout << "(Empty Tree)" << endl;
	}
}

void CryptoWalletSystem::printTxList() {
	Transaksi* tx = headGlobalTx;

	if (tx == nullptr) {
		cout << "[INFO] Global Ledger kosong (Belum ada transaksi)." << endl;
		return;
	}

	cout << "\n======= GLOBAL TRANSACTION LEDGER =======" << endl;
	cout << "-------------------------------------------" << endl;
	cout << " | " << left << setw(12) << "Tx ID"
		<< " | " << left << setw(8) << "Owner"
		<< " | " << left << setw(12) << "Amount" << " |" << endl;
	cout << "-------------------------------------------" << endl;

	while (tx != nullptr) {
		cout << " | " << left << setw(12) << tx->idTx
			<< " | " << left << setw(8) << tx->ownerId
			<< " | " << left << setw(12) << tx->nominal << " |" << endl;
		tx = tx->nextTx;
	}
	cout << "-------------------------------------------" << endl;
}


void CryptoWalletSystem::printUserTxList(string id) {
    User* u = headUser;
    
    while (u != nullptr) {
        if (u->idUser == id) break; 
        u = u->nextUser;
    }

    if (u == nullptr) {
        cout << "User ID " << id << " not found." << endl;
        return;
    }

    cout << "=== Transaction List for " << u->nama << " (" << u->idUser << ") ===" << endl;
    cout << "--------------------------------" << endl;
    cout << " ID          | Amount" << endl;
    cout << "--------------------------------" << endl;

    if (u->myTransactions.empty()) {
        cout << " (No transactions)" << endl;
    } else {
        int i = 0;
        while (i < u->myTransactions.size()) {
            Transaksi* tx = u->myTransactions[i];
            
            cout << " " << left << setw(12) << tx->idTx 
                 << "| " << tx->nominal << endl;
            
            i++;
        }
    }
    cout << "--------------------------------" << endl;
}

void CryptoWalletSystem::tampilkanLaporan() {
	User* u = headUser;
	cout << "\nGLOBAL LEDGER REPORT" << endl;
	cout << "------------------------------------------------" << endl;

	while (u != nullptr) {
		cout << "User: " << u->nama << " (" << u->idUser << ")" << endl;
		cout << "Owned Transactions (References to Global List):" << endl;

		if (u->myTransactions.empty()) {
			cout << "   (No transactions)" << endl;
		} else {
			for (Transaksi* tx : u->myTransactions) {
				cout << "   -> [" << tx->idTx << "] " << ": " << tx->nominal << endl;
			}
		}
		cout << endl;
		u = u->nextUser;
	}
	cout << "Global Root Hash: " << globalMerkleHash << endl;
}

User* CryptoWalletSystem::login(string id){
	User* temp = headUser;
	while (temp){
		if (temp->idUser == id){
			return temp;
		}
		temp = temp->nextUser;
	}
	return nullptr;
}
