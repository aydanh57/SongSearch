#include "DataLoader.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <cctype>
using namespace std;

// splits one line of the csv into separate fields
// commas inside quotes are kept as part of the field
vector<string> splitLine(string line) {
    vector<string> fields;
    string current = "";
    bool inQuotes = false;

    for (int i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == '"') {
            // two quotes in a row inside quotes means one actual quote
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(current);
            current = "";
        } else {
            current += c;
        }
    }
    fields.push_back(current);
    return fields;
}

// returns a copy of s with every letter changed to lowercase
string makeLower(string s) {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

// opens the csv, finds the track_name, artists, and popularity columns
// from the header, and makes a Song for each valid row
// rows with a missing title or a bad popularity number are skipped
// returns an empty vector if the file can't be opened
vector<Song> loadSongs(string path) {
    vector<Song> songs;
    ifstream file(path);

    if (!file.is_open()) {
        cout << "Could not open " << path << endl;
        return songs;
    }

    // first line is the column names
    string line;
    getline(file, line);
    vector<string> header = splitLine(line);

    int titleCol = -1;
    int artistCol = -1;
    int popCol = -1;
    for (int i = 0; i < header.size(); i++) {
        if (header[i] == "track_name") {
            titleCol = i;
        } else if (header[i] == "artists") {
            artistCol = i;
        } else if (header[i] == "popularity") {
            popCol = i;
        }
    }

    if (titleCol == -1 || artistCol == -1 || popCol == -1) {
        cout << "Could not find the columns we need" << endl;
        return songs;
    }

    unordered_set<string> seen; // just for removing duplicate titles

    while (getline(file, line)) {
        vector<string> fields = splitLine(line);

        // skip lines that got cut off or are broken
        if (fields.size() <= titleCol || fields.size() <= artistCol || fields.size() <= popCol) {
            continue;
        }

        string title = makeLower(fields[titleCol]);
        if (title == "") {
            continue;
        }
        seen.insert(title);

        Song s;
        s.title = title;
        s.artist = fields[artistCol];
        try {
            s.popularity = stoi(fields[popCol]);
        } catch (...) {
            continue; // popularity wasn't a number, skip this row
        }
        songs.push_back(s);
    }

    file.close();
    return songs;
}