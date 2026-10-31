cmake_minimum_required(VERSION 4.1)
project(SongSearch)

set(CMAKE_CXX_STANDARD 20)

add_executable(SongSearch main.cpp DataLoader.cpp Trie.cpp)