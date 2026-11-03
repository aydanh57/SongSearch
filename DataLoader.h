#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include "Song.h"
using namespace std;

// reads the csv file at path and returns every song in it as a vector
vector<Song> loadSongs(string path);

#endif