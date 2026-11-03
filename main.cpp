#include <iostream>
#include <chrono>
#include "DataLoader.h"
#include "Trie.h"
#include "RBTree.h"
#include "Benchmark.h"
using namespace std;
using namespace chrono;

// returns the user's input in lowercase to match the stored titles
string lowerCase(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

// prints the numbered list of songs for a set of results, or "No matches"
void printResults(vector<int>& results, vector<Song>& songs) {
    if (results.size() == 0) {
        cout << "No matches" << endl;
    }
    for (int i = 0; i < results.size(); i++) {
        Song s = songs[results[i]];
        cout << i + 1 << ". " << s.title << " - " << s.artist
             << " (popularity " << s.popularity << ")" << endl;
    }
}

// loads the songs, builds the trie and red-black tree once, then
// shows a menu to search, switch structures, compare both, or run the benchmark
int main() {
    vector<Song> songs = loadSongs("data/dataset.csv");
    cout << "Loaded " << songs.size() << " songs" << endl;

    // build both structures once at the start
    Trie trie;
    RBTree tree;
    for (int i = 0; i < songs.size(); i++) {
        trie.insert(songs[i].title, i);
        tree.insert(songs[i].title, i);
    }
    double trieMem = trie.memoryUsed() / 1000000.0;
    double treeMem = tree.memoryUsed() / 1000000.0;
    cout << "Both structures built" << endl;

    string current = "trie"; // which structure the search uses

    while (true) {
        cout << endl;
        cout << "=== Song Autocomplete ===" << endl;
        cout << "Current structure: " << current << endl;
        cout << "1. Search" << endl;
        cout << "2. Switch structure (trie / red-black tree)" << endl;
        cout << "3. Compare both on one prefix" << endl;
        cout << "4. Run benchmark" << endl;
        cout << "5. Quit" << endl;
        cout << "Choice: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            cout << "Type a prefix: ";
            string prefix;
            getline(cin, prefix);
            prefix = lowerCase(prefix);

            vector<int> results;
            steady_clock::time_point start = steady_clock::now();
            if (current == "trie") {
                results = trie.search(prefix, 5, songs);
            } else {
                results = tree.search(prefix, 5, songs);
            }
            steady_clock::time_point end = steady_clock::now();
            double ms = duration<double, milli>(end - start).count();

            printResults(results, songs);
            cout << "Query time: " << ms << " ms" << endl;
            if (current == "trie") {
                cout << "Estimated memory: " << trieMem << " MB" << endl;
            } else {
                cout << "Estimated memory: " << treeMem << " MB" << endl;
            }
        } else if (choice == "2") {
            if (current == "trie") {
                current = "red-black tree";
            } else {
                current = "trie";
            }
            cout << "Switched to " << current << endl;
        } else if (choice == "3") {
            cout << "Type a prefix: ";
            string prefix;
            getline(cin, prefix);
            prefix = lowerCase(prefix);

            steady_clock::time_point start = steady_clock::now();
            vector<int> trieResults = trie.search(prefix, 5, songs);
            steady_clock::time_point end = steady_clock::now();
            double trieMs = duration<double, milli>(end - start).count();

            start = steady_clock::now();
            vector<int> treeResults = tree.search(prefix, 5, songs);
            end = steady_clock::now();
            double treeMs = duration<double, milli>(end - start).count();

            cout << "-- Trie (" << trieMs << " ms, " << trieMem << " MB) --" << endl;
            printResults(trieResults, songs);
            cout << "-- Red-Black Tree (" << treeMs << " ms, " << treeMem << " MB) --" << endl;
            printResults(treeResults, songs);

            // check that both gave the same popularity numbers
            bool same = (trieResults.size() == treeResults.size());
            for (int i = 0; i < trieResults.size() && same; i++) {
                if (songs[trieResults[i]].popularity != songs[treeResults[i]].popularity) {
                    same = false;
                }
            }
            if (same) {
                cout << "Both structures returned matching results" << endl;
            } else {
                cout << "MISMATCH between the two structures" << endl;
            }
        } else if (choice == "4") {
            runBenchmark(songs);
        } else if (choice == "5") {
            break;
        } else {
            cout << "Not a valid choice" << endl;
        }
    }
    return 0;
}