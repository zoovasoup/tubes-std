#include "Wallet.h"
#include <iostream>
#include <iomanip>

using namespace std;

CryptoWalletSystem::CryptoWalletSystem() {
    headUser = nullptr;
}

void CryptoWalletSystem::updateMerkleTree(User* user) {
    vector<string> txHashes;
    Transaksi* curr = user->headTx;
    while (curr != nullptr) {
        string rawData = curr->idTx + curr->jenis + to_string((int)curr->nominal);
        txHashes.push_back(computeHash(rawData));
        curr = curr->nextTx;
    }

    if (!txHashes.empty()) {
        if (user->merkleTreeRoot) deleteMerkleTree(user->merkleTreeRoot);
        user->merkleTreeRoot = buildMerkleRecursive(txHashes, 0, txHashes.size() - 1);
        user->merkleRootHash = user->merkleTreeRoot->hash;
    }
}

void CryptoWalletSystem::tambahUser(string id, string nama) {
    User* newUser = new User;
    newUser->idUser = id;
    newUser->nama = nama;
    newUser->headTx = nullptr;
    newUser->nextUser = nullptr;
    newUser->merkleTreeRoot = nullptr;
    newUser->merkleRootHash = "EMPTY";

    if (!headUser) {
        headUser = newUser;
    } else {
        User* temp = headUser;
        while (temp->nextUser) temp = temp->nextUser;
        temp->nextUser = newUser;
    }
    cout << "[INFO] User dibuat: " << nama << endl;
}

void CryptoWalletSystem::tambahTransaksi(string targetIdUser, string idTx, string jenis, double nominal) {
    User* currUser = headUser;
    while (currUser && currUser->idUser != targetIdUser) {
        currUser = currUser->nextUser;
    }

    if (!currUser) {
        cout << "[ERR] User tidak ditemukan!" << endl;
        return;
    }

    Transaksi* newTx = new Transaksi;
    newTx->idTx = idTx;
    newTx->jenis = jenis;
    newTx->nominal = nominal;
    newTx->nextTx = nullptr;

    if (currUser->headTx == nullptr) {
        currUser->headTx = newTx;
    } else {
        Transaksi* t = currUser->headTx;
        while (t->nextTx) t = t->nextTx;
        t->nextTx = newTx;
    }

    updateMerkleTree(currUser);

    cout << "[SUKSES] Transaksi " << idTx << " masuk ke wallet " << currUser->nama << endl;
    cout << "         -> Security Root diperbarui: " << currUser->merkleRootHash << endl;
}

void CryptoWalletSystem::tampilkanLaporan() {
    User* u = headUser;
    cout << "\n=======================================================" << endl;
    cout << "           LAPORAN SISTEM DOMPET KRIPTO" << endl;
    cout << "=======================================================" << endl;

    while (u != nullptr) {
        cout << "USER: " << u->nama << " (ID: " << u->idUser << ")" << endl;
        cout << "ROOT HASH: " << u->merkleRootHash << endl;

        cout << "\n   [VISUALISASI MERKLE TREE USER INI]" << endl;
        if (u->merkleTreeRoot) {
            // [UPDATE] Memanggil fungsi visualisasi dari MerkleTree.cpp
            printMerkleTree(u->merkleTreeRoot);
        } else {
            cout << "   (Tree belum terbentuk / Tidak ada transaksi)" << endl;
        }

        cout << "\n   [DETAIL RIWAYAT TRANSAKSI]" << endl;
        Transaksi* t = u->headTx;
        if (!t) cout << "   (Tidak ada transaksi)" << endl;
        else {
            cout << "   ---------------------------------------" << endl;
            cout << "   | ID      | Jenis    | Nominal        |" << endl;
            cout << "   ---------------------------------------" << endl;
            while (t != nullptr) {
                cout << "   | " << left << setw(8) << t->idTx
                     << "| " << left << setw(9) << t->jenis
                     << "| " << left << setw(14) << t->nominal << " |" << endl;
                t = t->nextTx;
            }
            cout << "   ---------------------------------------" << endl;
        }
        cout << "\n-------------------------------------------------------" << endl;
        u = u->nextUser;
    }
}

void CryptoWalletSystem::cariDanValidasiTransaksi(string targetIdUser, string idTx) {
    cout << "\n--- VALIDASI TRANSAKSI: " << idTx << " (" << targetIdUser << ") ---" << endl;

    User* currUser = headUser;
    while (currUser && currUser->idUser != targetIdUser) {
        currUser = currUser->nextUser;
    }

    if (!currUser) {
        cout << "[GAGAL] User ID " << targetIdUser << " tidak ditemukan." << endl;
        return;
    }

    Transaksi* t = currUser->headTx;
    Transaksi* foundTx = nullptr;

    while (t != nullptr) {
        if (t->idTx == idTx) {
            foundTx = t;
            break;
        }
        t = t->nextTx;
    }

    if (!foundTx) {
        cout << "[GAGAL] Transaksi tidak ditemukan dalam riwayat User." << endl;
        return;
    }

    cout << "[INFO] Data ditemukan di Linked List:" << endl;
    cout << "       Nominal: " << foundTx->nominal << " | Jenis: " << foundTx->jenis << endl;

    string rawData = foundTx->idTx + foundTx->jenis + to_string((int)foundTx->nominal);
    string targetHash = computeHash(rawData);

    cout << "       Hash Data: " << targetHash << endl;
    cout << "       Memverifikasi ke Merkle Tree..." << endl;

    bool isValid = searchMerkleHash(currUser->merkleTreeRoot, targetHash);

    if (isValid) {
        cout << "[VALID] Transaksi TERVERIFIKASI! Hash ditemukan dalam struktur Tree." << endl;
    } else {
        cout << "[BAHAYA] Transaksi TIDAK VALID! Data mungkin telah dimanipulasi (Hash tidak cocok)." << endl;
    }
}
