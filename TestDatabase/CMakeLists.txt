cmake_minimum_required(VERSION 3.0.0)
project(database_setup)

# Add the SQLite subdirectory and specify the binary directory
add_subdirectory(../lib/sqlite ${CMAKE_BINARY_DIR}/sqlite)

# Add the executable for database setup
add_executable(database_setup database_setup.cpp)

# Link the sqlite3 library
target_link_libraries(database_setup PRIVATE sqlite3)

# Include the directory containing sqlite3.h
target_include_directories(database_setup PRIVATE ../lib/sqlite/include)