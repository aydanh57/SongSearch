#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "Song.h"
using namespace std;

struct TrieNode {
    char letter;
    TrieNode* firstChild;
    TrieNode* nextSibling;
    bool isEnd;
    vector<int> songIndexes;

    TrieNode(char c) {
        letter = c;
        firstChild = nullptr;
        nextSibling = nullptr;
        isEnd = false;
    }
};

class Trie {
    TrieNode* root;
    void collect(TrieNode* node, vector<int>& results);
    void destroy(TrieNode* node);

public:
    Trie();
    ~Trie();
    void insert(string title, int songIndex);
    vector<int> search(string prefix, int n, vector<Song>& songs);
};

#endif