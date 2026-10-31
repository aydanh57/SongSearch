#include <iostream>
#include "DataLoader.h"
#include "Trie.h"
#include "RBTree.h"
using namespace std;

int main() {
    vector<Song> songs = loadSongs("data/dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;

    Trie trie;
    RBTree tree;
    for (int i = 0; i < songs.size(); i++) {
        trie.insert(songs[i].title, i);
        tree.insert(songs[i].title, i);
    }
    cout << "Both built" << endl;

    string prefix;
    while (true) {
        cout << "Type a prefix (or quit): ";
        getline(cin, prefix);
        if (prefix == "quit") {
            break;
        }

        vector<int> trieResults = trie.search(prefix, 5, songs);
        vector<int> treeResults = tree.search(prefix, 5, songs);

        cout << "-- Trie --" << endl;
        for (int i = 0; i < trieResults.size(); i++) {
            Song s = songs[trieResults[i]];
            cout << s.title << " - " << s.artist << " (" << s.popularity << ")" << endl;
        }
        cout << "-- Red-Black Tree --" << endl;
        for (int i = 0; i < treeResults.size(); i++) {
            Song s = songs[treeResults[i]];
            cout << s.title << " - " << s.artist << " (" << s.popularity << ")" << endl;
        }

        // check if the popularity numbers match
        bool same = (trieResults.size() == treeResults.size());
        for (int i = 0; i < trieResults.size() && same; i++) {
            if (songs[trieResults[i]].popularity != songs[treeResults[i]].popularity) {
                same = false;
            }
        }
        if (same) {
            cout << "Both match" << endl;
        } else {
            cout << "MISMATCH" << endl;
        }
    }
    return 0;
}z