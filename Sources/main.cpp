#include <iostream>
#include "Wallet.h"

using namespace std;

int main() {
    CryptoWalletSystem system;

    cout << "=== INISIALISASI SISTEM ===" << endl;

    system.tambahUser("U01", "Alice");
    system.tambahUser("U02", "Bob");

    cout << "\n=== SIMULASI TRANSAKSI ===" << endl;

    system.tambahTransaksi("U01", "TX_A1", "Deposit", 100);
    system.tambahTransaksi("U01", "TX_A2", "Buy NFT", 50);

    system.tambahTransaksi("U02", "TX_B1", "Deposit", 500);

    system.tampilkanLaporan();

    cout << "\n=== PENGUJIAN FITUR PENCARIAN & VALIDASI ===" << endl;

    system.cariDanValidasiTransaksi("U01", "TX_A1");

    system.cariDanValidasiTransaksi("U01", "TX_GHOST");

    system.cariDanValidasiTransaksi("U99", "TX_A1");

    return 0;
}
