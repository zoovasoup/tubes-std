#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "MerkleTree.h"

using namespace std;

struct Transaksi {
	string idTx;
	string ownerId;
	double nominal;

	Transaksi* nextTx;
};

struct User {
	string idUser;
	string nama;

	vector<Transaksi*> myTransactions; 

	User* nextUser;
};

class CryptoWalletSystem {
private:
	User* headUser;

	Transaksi* headGlobalTx;       
	MerkleNode* globalMerkleRoot;  
	string globalMerkleHash;       

	static int countUser;
	static int countTx;

	void updateGlobalMerkleTree(); 

public:
	CryptoWalletSystem();

	void tambahUser(string nama);
	void tambahTransaksi(string targetIdUser, string idTx, double nominal);
	void tampilkanLaporan(); 
	void cariDanValidasiTransaksi(string idTx); 
	User* login(string id);
	void printGlobalTree();
	void printUserTxList(string idUser);
	void printTxList();

};

#endif
