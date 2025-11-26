#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>

using namespace std;

struct MerkleNode {
    string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(string h, MerkleNode* l = nullptr, MerkleNode* r = nullptr);
};

string computeHash(string input);

MerkleNode* buildMerkleRecursive(const vector<string>& hashes, int start, int end);

void deleteMerkleTree(MerkleNode* node);

void printMerkleTree(MerkleNode* node, int indent = 0);

bool searchMerkleHash(MerkleNode* node, string targetHash);

#endif
