#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "MerkleTree.h"

using namespace std;

struct Transaksi {
    string idTx;
    string jenis;
    double nominal;

    Transaksi* nextTx;
};

struct User {
    string idUser;
    string nama;

    Transaksi* headTx;
    User* nextUser;

    string merkleRootHash;
    MerkleNode* merkleTreeRoot;
};


class CryptoWalletSystem {
private:
    User* headUser;

    void updateMerkleTree(User* user);

public:
    CryptoWalletSystem();

    void tambahUser(string id, string nama);
    void tambahTransaksi(string targetIdUser, string idTx, string jenis, double nominal);

    void tampilkanLaporan();

    void cariDanValidasiTransaksi(string targetIdUser, string idTx);
};

#endif
