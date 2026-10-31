#include <iostream>
#include "DataLoader.h"
#include "Trie.h"
using namespace std;

int main() {
    vector<Song> songs = loadSongs("data/dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;

    Trie trie;
    for (int i = 0; i < songs.size(); i++) {
        trie.insert(songs[i].title, i);
    }
    cout << "Trie built" << endl;

    string prefix;
    while (true) {
        cout << "Type a prefix (or quit): ";
        getline(cin, prefix);
        if (prefix == "quit") {
            break;
        }

        vector<int> matches = trie.search(prefix, 5, songs);
        if (matches.size() == 0) {
            cout << "No matches" << endl;
        }
        for (int i = 0; i < matches.size(); i++) {
            Song s = songs[matches[i]];
            cout << s.title << " - " << s.artist << " (" << s.popularity << ")" << endl;
        }
    }
    return 0;
}