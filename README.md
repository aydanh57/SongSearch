# Song Autocomplete: Trie vs. Red-Black Tree

COP3530 Project 2, Fall 2026. Team: The Trie Hards ([Aydan Haynes], [Joshua Gabbard], [Leslie Paulsen])

A C++ console program that suggests songs as you type the beginning of a title.
It uses two data structures written from scratch (a trie and a red-black tree)
and compares them on build time, query time, and memory use.

## Data
We use the Spotify Tracks Dataset from Kaggle (about 114,000 songs):
https://www.kaggle.com/datasets/maharshipandya/-spotify-tracks-dataset

1. Download and unzip it.
2. Put the CSV in a folder named `data` in the project root and name it `dataset.csv`.

The CSV is not included in this repo because of its size.

## How to Run

### In CLion
1. Open the project folder in CLion.
2. Go to Run > Edit Configurations and set the **Working directory** to the project
   folder (the one that contains `data/` and `CMakeLists.txt`).
3. Click Run.

### From the terminal
```
mkdir build
cd build
cmake ..
make
cd ..
./build/SongSearch
```
Run it from the project folder so it can find `data/dataset.csv`.

## Menu Options
1. **Search**: type a prefix and get the top 5 songs by popularity, with query time and estimated memory.
2. **Switch structure**: toggle between the trie and the red-black tree.
3. **Compare both**: runs one prefix on both structures and checks that the results match.
4. **Run benchmark**: tests 10,000, 50,000, and all songs, and saves `build_results.csv` and `query_results.csv`.
5. **Quit**

## Files
- `main.cpp`: menu and program flow
- `Song.h`: struct for a song (title, artist, popularity)
- `DataLoader.h/.cpp`: reads and cleans the CSV
- `Trie.h/.cpp`: trie implementation
- `RBTree.h/.cpp`: red-black tree implementation
- `Benchmark.h/.cpp`: timing and memory tests

## Notes
- Titles are converted to lowercase, so searches are not case sensitive.
- Both structures are implemented without using STL containers for the search structures.
- Memory numbers are estimates (node count times node size), not exact measurements.