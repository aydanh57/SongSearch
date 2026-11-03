#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <vector>
#include "Song.h"
using namespace std;

// one node in the red-black tree, holds a song title and its position in the songs vector
// isRed is the node's color, used to keep the tree balanced
struct RBNode {
    string title;
    int songIndex;
    bool isRed;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(string t, int index) {
        title = t;
        songIndex = index;
        isRed = true; // new nodes start red
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

// red-black tree that keeps song titles in sorted order and stays balanced
// so inserts take O(log n) steps
class RBTree {
private:
    RBNode* root;
    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* x);
    void fixInsert(RBNode* node);
    void collect(RBNode* node, string prefix, vector<int>& results);
    void destroy(RBNode* node);
    void countNodes(RBNode* node, long long& nodes, long long& extra);

public:
    RBTree();
    ~RBTree();
    void insert(string title, int songIndex);
    vector<int> search(string prefix, int n, vector<Song>& songs);
    long long memoryUsed();
};

#endif