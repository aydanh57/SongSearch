# Song Autocomplete: Trie vs. Red-Black Tree

COP3530 Project 2, Fall 2026  
Team: The Trie Hards (Aydan Haynes, Joshua Gabbard, Leslie Paulsen)

This project is a C++ program that lets you search for songs by typing the beginning of a title. I built two data structures, a trie and a red-black tree, and used them to see how they compare when searching for songs. I looked at things like build time, search time, and memory usage.

## Data

I used the Spotify Tracks Dataset from Kaggle, which has about 114,000 songs.

https://www.kaggle.com/datasets/maharshipandya/-spotify-tracks-dataset

To use the dataset:
1. Download and unzip the dataset.
2. Create a folder called `data` in the main project folder.
3. Put the CSV file in that folder and rename it to `dataset.csv`.

The dataset isn't included in this repo because the file is too large.

## How to Run

### Using CLion
1. Open the project folder in CLion.
2. Go to Run > Edit Configurations.
3. Set the **Working directory** to the main project folder (the one with `data/` and `CMakeLists.txt`).
4. Click Run.

## Menu Options

1. **Search:** Enter the beginning of a song title to get the top 5 matching songs based on popularity. It also shows the search time and estimated memory usage.
2. **Switch structure:** Switch between the trie and red-black tree.
3. **Compare both:** Search the same prefix using both structures and check if they return the same results.
4. **Run benchmark:** Test the structures with 10,000 songs, 50,000 songs, and the full dataset. The results are saved in `build_results.csv` and `query_results.csv`.
5. **Quit:** Exit the program.

## Project Files

- `main.cpp`: Runs the program and handles the menu.
- `Song.h`: Contains the song information (title, artist, and popularity).
- `DataLoader.h/.cpp`: Loads and cleans the CSV data.
- `Trie.h/.cpp`: Contains my trie implementation.
- `RBTree.h/.cpp`: Contains my red-black tree implementation.
- `Benchmark.h/.cpp`: Handles the timing and memory tests.

## Notes

- Song titles are converted to lowercase, so searches are not case sensitive.
- I wrote the trie and the red-black tree myself, including insertion, rebalancing, and prefix search. I only used `std::vector` and `std::string` to store songs and results.
- Memory usage is estimated by multiplying the number of nodes by the size of each node, so it may not be exact.
