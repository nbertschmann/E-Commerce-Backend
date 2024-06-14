cmake_minimum_required(VERSION 3.0)

# Create a library from the sqlite3 source file
add_library(sqlite3 src/sqlite3.c)

# Specify the include directories for this library
target_include_directories(sqlite3 PUBLIC include)
