#include "MerkleTree.h"
#include <functional>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

MerkleNode::MerkleNode(string h, MerkleNode* l, MerkleNode* r)
    : hash(h), left(l), right(r) {}

string computeHash(string input) {
    if (input.empty()) return "";
    hash<string> hasher;
    size_t h = hasher(input);
    return to_string(h).substr(0, 8);
}

MerkleNode* buildMerkleRecursive(const vector<string>& hashes, int start, int end) {
    if (start > end) return nullptr;

    if (start == end) {
        return new MerkleNode(hashes[start]);
    }

    int mid = (start + end) / 2;
    MerkleNode* leftChild = buildMerkleRecursive(hashes, start, mid);
    MerkleNode* rightChild = buildMerkleRecursive(hashes, mid + 1, end);

    string combined = leftChild->hash + ((rightChild) ? rightChild->hash : "");
    string parentHash = computeHash(combined);

    return new MerkleNode(parentHash, leftChild, rightChild);
}

void deleteMerkleTree(MerkleNode* node) {
    if (!node) return;
    deleteMerkleTree(node->left);
    deleteMerkleTree(node->right);
    delete node;
}

void printMerkleTree(MerkleNode* node, int indent) {
    if (node != nullptr) {
        if (indent > 0) {
            for (int i = 0; i < indent - 1; i++) cout << "   ";
            cout << "|-- ";
        }
        cout << "[" << node->hash << "]";
        if (!node->left && !node->right) cout << " (Data Hash)";
        else cout << " (Gabungan)";
        cout << endl;

        printMerkleTree(node->left, indent + 1);
        printMerkleTree(node->right, indent + 1);
    }
}

bool searchMerkleHash(MerkleNode* node, string targetHash) {
    if (node == nullptr) return false;

    if (node->hash == targetHash) return true;

    return searchMerkleHash(node->left, targetHash) || searchMerkleHash(node->right, targetHash);
}
