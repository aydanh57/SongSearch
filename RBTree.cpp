#include "RBTree.h"
using namespace std;

// makes an empty tree
RBTree::RBTree() {
    root = nullptr;
}

// frees all the memory used by the tree
RBTree::~RBTree() {
    destroy(root);
}

// deletes a node and both of its subtrees
void RBTree::destroy(RBNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// moves x down to the left and its right child up into x's place
// used to rebalance the tree after an insert
void RBTree::rotateLeft(RBNode* x) {
    RBNode* y = x->right;

    // y's left subtree becomes x's right subtree
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }

    // y takes x's spot under x's parent
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // x goes down to be y's left child
    y->left = x;
    x->parent = y;
}

// mirror image of rotateLeft: moves x down to the right and its left child up
void RBTree::rotateRight(RBNode* x) {
    RBNode* y = x->left;

    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

// adds a title like a normal binary search tree (equal titles go right),
// then calls fixInsert to restore the red-black rules
// worst case O(L * log n) for title length L and n songs
void RBTree::insert(string title, int songIndex) {
    RBNode* newNode = new RBNode(title, songIndex);

    // normal BST insert, equal titles go to the right
    RBNode* parent = nullptr;
    RBNode* current = root;
    while (current != nullptr) {
        parent = current;
        if (title < current->title) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;
    if (parent == nullptr) {
        root = newNode;
    } else if (title < parent->title) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    fixInsert(newNode);
}

// fixes any red-black rule broken by an insert (a red node with a red parent)
// case 1: red uncle, recolor and move up
// case 2: node is an inside child, rotate it to the outside
// case 3: node is an outside child, recolor and rotate the grandparent
// the root is always set to black at the end
void RBTree::fixInsert(RBNode* node) {
    // only a problem if the parent is also red
    while (node != root && node->parent->isRed) {
        RBNode* parent = node->parent;
        RBNode* grandparent = parent->parent;

        if (parent == grandparent->left) {
            RBNode* uncle = grandparent->right;

            if (uncle != nullptr && uncle->isRed) {
                // case 1: uncle is red, just recolor and move up
                parent->isRed = false;
                uncle->isRed = false;
                grandparent->isRed = true;
                node = grandparent;
            } else {
                // case 2: node is an inside child, rotate it to the outside
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(node);
                    parent = node->parent;
                }
                // case 3: node is an outside child
                parent->isRed = false;
                grandparent->isRed = true;
                rotateRight(grandparent);
            }
        } else {
            // same thing but left and right swapped
            RBNode* uncle = grandparent->left;

            if (uncle != nullptr && uncle->isRed) {
                parent->isRed = false;
                uncle->isRed = false;
                grandparent->isRed = true;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(node);
                    parent = node->parent;
                }
                parent->isRed = false;
                grandparent->isRed = true;
                rotateLeft(grandparent);
            }
        }
    }
    root->isRed = false; // root is always black
}

// finds every title that starts with prefix
// compares the start of each title to the prefix and skips any side
// of the tree that can't contain a match
void RBTree::collect(RBNode* node, string prefix, vector<int>& results) {
    if (node == nullptr) {
        return;
    }

    // compare just the first part of the title to the prefix
    string start = node->title.substr(0, prefix.size());

    if (start < prefix) {
        // this title is before the prefix, only the right side could match
        collect(node->right, prefix, results);
    } else if (start > prefix) {
        // this title is after the prefix, only the left side could match
        collect(node->left, prefix, results);
    } else {
        // match, check both sides
        collect(node->left, prefix, results);
        results.push_back(node->songIndex);
        collect(node->right, prefix, results);
    }
}

// returns the indexes of up to n songs whose titles start with prefix,
// sorted from most popular to least popular
// works the same way as Trie::search so the results can be compared
vector<int> RBTree::search(string prefix, int n, vector<Song>& songs) {
    vector<int> top;
    vector<int> results;
    collect(root, prefix, results);

    // pick the top n by popularity, same as the trie
    while (top.size() < n && results.size() > 0) {
        int bestSpot = 0;
        for (int i = 1; i < results.size(); i++) {
            if (songs[results[i]].popularity > songs[results[bestSpot]].popularity) {
                bestSpot = i;
            }
        }
        int pick = results[bestSpot];
        results.erase(results.begin() + bestSpot);

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

// counts the nodes below this one and the extra space used by long titles
void RBTree::countNodes(RBNode* node, long long& nodes, long long& extra) {
    if (node == nullptr) {
        return;
    }
    nodes++;
    // short strings live inside the string object, long ones need extra space
    if (node->title.size() > 15) {
        extra += node->title.size();
    }
    countNodes(node->left, nodes, extra);
    countNodes(node->right, nodes, extra);
}

// estimates memory in bytes: node count times node size plus long title storage
// this is an estimate, not an exact measurement
long long RBTree::memoryUsed() {
    long long nodes = 0;
    long long extra = 0;
    countNodes(root, nodes, extra);
    return nodes * sizeof(RBNode) + extra;
}