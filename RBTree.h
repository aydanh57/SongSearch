#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <vector>
#include "Song.h"
using namespace std;

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

class RBTree {
private:
    RBNode* root;
    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* x);
    void fixInsert(RBNode* node);
    void collect(RBNode* node, string prefix, vector<int>& results);
    void destroy(RBNode* node);

public:
    RBTree();
    ~RBTree();
    void insert(string title, int songIndex);
    vector<int> search(string prefix, int n, vector<Song>& songs);
};

#endif