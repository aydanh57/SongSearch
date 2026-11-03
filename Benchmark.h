#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include "Song.h"
using namespace std;

// builds both structures at 10,000, 50,000, and all songs, timing each build
// then times searches for 10 sample prefixes and averages over 50 repeats
// writes build_results.csv and query_results.csv in the working directory
void runBenchmark(vector<Song>& songs);

#endif