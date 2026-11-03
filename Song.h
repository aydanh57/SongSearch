#ifndef SONG_H
#define SONG_H

#include <string>
using namespace std;

// holds the info for one song from the dataset
// title is stored in lowercase so searching isn't case sensitive
struct Song {
    string title;
    string artist;
    int popularity;
};

#endif