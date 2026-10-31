#include "Trie.h"
using namespace std;

Trie::Trie() {
    root = new TrieNode(' ');
}

Trie::~Trie() {
    destroy(root);
}

// deletes every node so we don't leak memory
void Trie::destroy(TrieNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->firstChild);
    destroy(node->nextSibling);
    delete node;
}

void Trie::insert(string title, int songIndex) {
    TrieNode* current = root;

    for (int i = 0; i < title.size(); i++) {
        char c = title[i];

        // look through the children for this letter
        TrieNode* child = current->firstChild;
        while (child != nullptr && child->letter != c) {
            child = child->nextSibling;
        }

        // not found so make a new node and put it at the front
        if (child == nullptr) {
            child = new TrieNode(c);
            child->nextSibling = current->firstChild;
            current->firstChild = child;
        }

        current = child;
    }

    current->isEnd = true;
    current->songIndexes.push_back(songIndex);
}

// goes through everything below a node and saves the song indexes
void Trie::collect(TrieNode* node, vector<int>& results) {
    if (node == nullptr) {
        return;
    }
    if (node->isEnd) {
        for (int i = 0; i < node->songIndexes.size(); i++) {
            results.push_back(node->songIndexes[i]);
        }
    }
    // go down to the children, then across to the siblings
    collect(node->firstChild, results);
    collect(node->nextSibling, results);
}

vector<int> Trie::search(string prefix, int n, vector<Song>& songs) {
    vector<int> top;
    TrieNode* current = root;

    // walk down the trie following the prefix
    for (int i = 0; i < prefix.size(); i++) {
        char c = prefix[i];

        TrieNode* child = current->firstChild;
        while (child != nullptr && child->letter != c) {
            child = child->nextSibling;
        }

        if (child == nullptr) {
            return top; // no titles start with this prefix
        }
        current = child;
    }

    // collect everything under the prefix (the node itself first, then below)
    vector<int> results;
    if (current->isEnd) {
        for (int i = 0; i < current->songIndexes.size(); i++) {
            results.push_back(current->songIndexes[i]);
        }
    }
    collect(current->firstChild, results);

    // pick the top n by popularity
    while (top.size() < n && results.size() > 0) {
        int bestSpot = 0;
        for (int i = 1; i < results.size(); i++) {
            if (songs[results[i]].popularity > songs[results[bestSpot]].popularity) {
                bestSpot = i;
            }
        }
        int pick = results[bestSpot];
        results.erase(results.begin() + bestSpot);

        // skip it if we already have this same song in the list
        bool alreadyHave = false;
        for (int i = 0; i < top.size(); i++) {
            if (songs[top[i]].title == songs[pick].title &&
                songs[top[i]].artist == songs[pick].artist) {
                alreadyHave = true;
                }
        }
        if (!alreadyHave) {
            top.push_back(pick);
        }
    }

    return top;
}