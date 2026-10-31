#include "Benchmark.h"
#include "Trie.h"
#include "RBTree.h"
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

void runBenchmark(vector<Song>& songs) {
    ofstream buildFile("build_results.csv");
    ofstream queryFile("query_results.csv");
    buildFile << "size,structure,build_ms,memory_mb" << endl;
    queryFile << "size,structure,prefix,prefix_length,avg_query_ms" << endl;

    int sizes[3] = {10000, 50000, (int)songs.size()};
    string prefixes[10] = {"a", "l", "go", "lo", "goo", "lov", "good", "love", "happy", "summer"};
    int repeats = 50;

    for (int s = 0; s < 3; s++) {
        int size = sizes[s];
        cout << "Testing size " << size << endl;

        // ---- build the trie ----
        Trie* trie = new Trie();
        steady_clock::time_point start = steady_clock::now();
        for (int i = 0; i < size; i++) {
            trie->insert(songs[i].title, i);
        }
        steady_clock::time_point end = steady_clock::now();
        double trieBuild = duration<double, milli>(end - start).count();
        double trieMem = trie->memoryUsed() / 1000000.0;
        buildFile << size << ",trie," << trieBuild << "," << trieMem << endl;

        // ---- build the red-black tree ----
        RBTree* tree = new RBTree();
        start = steady_clock::now();
        for (int i = 0; i < size; i++) {
            tree->insert(songs[i].title, i);
        }
        end = steady_clock::now();
        double treeBuild = duration<double, milli>(end - start).count();
        double treeMem = tree->memoryUsed() / 1000000.0;
        buildFile << size << ",rbtree," << treeBuild << "," << treeMem << endl;

        // ---- time the searches ----
        int total = 0; // just so the searches actually get used
        for (int p = 0; p < 10; p++) {
            string prefix = prefixes[p];

            start = steady_clock::now();
            for (int r = 0; r < repeats; r++) {
                vector<int> results = trie->search(prefix, 5, songs);
                total += results.size();
            }
            end = steady_clock::now();
            double trieQuery = duration<double, milli>(end - start).count() / repeats;
            queryFile << size << ",trie," << prefix << "," << prefix.size() << "," << trieQuery << endl;

            start = steady_clock::now();
            for (int r = 0; r < repeats; r++) {
                vector<int> results = tree->search(prefix, 5, songs);
                total += results.size();
            }
            end = steady_clock::now();
            double treeQuery = duration<double, milli>(end - start).count() / repeats;
            queryFile << size << ",rbtree," << prefix << "," << prefix.size() << "," << treeQuery << endl;
        }

        cout << "  trie build " << trieBuild << " ms, " << trieMem << " MB" << endl;
        cout << "  tree build " << treeBuild << " ms, " << treeMem << " MB" << endl;

        delete trie;
        delete tree;
    }

    buildFile.close();
    queryFile.close();
    cout << "Done. Saved build_results.csv and query_results.csv" << endl;
}