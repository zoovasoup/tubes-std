# CryptoApp (CryptoWallet Simulation)

A command-line **Crypto Wallet simulation** implemented in **C++**, demonstrating core **data structure concepts** such as **Linked Lists**, **Binary Trees**, and **Merkle Trees** for transaction integrity verification.

This project was developed as part of the telkom university **Data Structures** 2024 course assignment, focusing on practical implementation of non-linear data structures.

---

## 📌 Features

* **User Management**

  * Register and login users
  * Stored using a **Single Linked List**

* **Transaction Ledger**

  * Global transaction list shared by all users
  * Implemented with a **Single Linked List**

* **Merkle Tree (Binary Tree)**

  * Each transaction is hashed as a leaf node
  * Parent nodes store combined hashes
  * Root hash represents global ledger integrity

* **Transaction Verification**

  * Recursive search on Merkle Tree
  * Confirms whether a transaction exists and is valid

* **CLI-based Interface**

  * Interactive command loop (REPL)
  * Simple shell-like commands

---

## 🧱 Data Structures Used

* **Array / Vector**
  * Stores transactions owned by a specific user
  * Provides fast indexed access per user

* **Single Linked List**
  * User registry
  * Global transaction ledger (append-only)

* **Binary Tree (Merkle Tree)**
  * Built recursively from transaction hashes
  * Used for global data integrity verification

---

## 🗂 Project Structure

```
.
├── CryptoApp            # Compiled binary (Linux/macOS)
├── CryptoApp.exe        # Compiled Windows executable
├── Sources/             # C++ source files
├── Headers/             # Header files
├── bin/                 # Build output
├── obj/                 # Object files
├── document.md          # Project report & explanation
├── README.md            # Project documentation
├── tubes.cbp            # Code::Blocks project file
├── tubes.depend
├── tubes.layout
└── .gitignore
```

---

## ⚙️ Build & Run

### Linux / macOS

```bash
./CryptoApp
```

### Windows

(Executable is already included in the `releases`.)

```cmd
CryptoApp.exe
```
> ⚠ Windows SmartScreen Notice
> 
> When running CryptoApp.exe, Windows may display a “Windows protected your PC” warning.
> 
> This does not indicate malware. The warning appears because the executable is not digitally signed and therefore has no established reputation with Microsoft SmartScreen. This is common for open-source and academic projects distributed via GitHub.
> 
> If you still have concerns, you are free to inspect the source code or analyze the binary using a decompiler, as the project is fully transparent and does not perform any hidden or malicious actions.
> 
> To run the application:
> - Click More info
> - Click Run anyway
> 
> The application is a simple command-line educational program and does not install system services, modify the registry, or access sensitive system resources.

## 🛠 Manual Compilation Guide

### Linux / macOS (GCC / Clang)

#### Requirements
- `g++` atau `clang++`
- Compiler support C++17

#### Compile
```bash
g++ -std=c++17 Sources/*.cpp -IHeaders -o CryptoApp
```

---

### Windows (MinGW / g++)

#### Requirements
- MinGW-w64
- `g++` available in PATH

#### Compile
```cmd
g++ -std=gnu++17 Sources\*.cpp -IHeaders -o CryptoApp.exe
```

---

## ⌨️ Available Commands

| Command           | Description                                      |
| ----------------- | ------------------------------------------------ |
| `register <name>` | Register a new user                              |
| `login <user_id>` | Login as an existing user                        |
| `tx <amount>`     | Create a transaction                             |
| `tree`            | Display global Merkle Tree (pre-order traversal) |
| `verify <tx_id>`  | Verify transaction integrity                     |
| `list`            | Show users or transactions                       |
| `exit`            | Exit the program                                 |

---

## 🧪 Example Session

```
cryptoWallet~ register Alice
[SUCCESS] User Alice with id: U1

cryptoWallet~ login U1
You are now logged in as Alice

cryptoWallet(U1-Alice)~ tx 100
[SUCCESS] Transaction has been added.

cryptoWallet(U1-Alice)~ tree
=== GLOBAL MERKLE TREE ===
Root Hash: 19283746

cryptoWallet(U1-Alice)~ verify TX-1
[VALID] Transaction confirmed in Merkle Tree
```

---

## 🎓 Academic Context

* **Course**: Data Structures
* **Topic**: Binary Tree / Merkle Tree Implementation
* **Language**: C++
* **Interface**: Command Line (CLI)

---

## 📜 License

This project is intended for **educational purposes**.
