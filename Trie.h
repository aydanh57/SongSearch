#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "Song.h"
using namespace std;

// one node in the trie, stores a single letter
// children are kept as a linked list (firstChild, then nextSibling)
// songIndexes holds the positions of songs whose title ends at this node
struct TrieNode {
    char letter;
    TrieNode* firstChild;
    TrieNode* nextSibling;
    bool isEnd;
    vector<int> songIndexes; // can hold several songs with the same title

    // trie that stores song titles letter by letter so we can quickly find
    // every title that starts with a given prefix
    TrieNode(char c) {
        letter = c;
        firstChild = nullptr;
        nextSibling = nullptr;
        isEnd = false;
    }
};

class Trie {
private:
    TrieNode* root;
    void collect(TrieNode* node, vector<int>& results);
    void destroy(TrieNode* node);
    void countNodes(TrieNode* node, long long& nodes, long long& indexes);

public:
    Trie();
    ~Trie();
    void insert(string title, int songIndex);
    vector<int> search(string prefix, int n, vector<Song>& songs);
    long long memoryUsed();
};

#endif