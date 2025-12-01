#include "Wallet.h"
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
    cout << "CREATE USER " << nama << " OK. ID: " << newUser->idUser << endl;
}

void CryptoWalletSystem::tambahTransaksi(string targetIdUser, string idTx, double nominal) {
    User* currUser = login(targetIdUser);
    if (!currUser) {
        cout << "[ERR] User not found" << endl;
        return;
    }

    Transaksi* newTx = new Transaksi;
    countTx++;

    if(idTx == "AUTO") {
        newTx->idTx = "TX_GL_" + to_string(countTx);
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
    cout << "INSERT GLOBAL 0 1 (Block Hash: " << globalMerkleHash << ")" << endl;
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
        cout << "   Status  : VALID (Confirmed in Global Tree)" << endl;
    } else {
        cout << "FALSE (Not valid in Global Tree)" << endl;
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
