cmake_minimum_required(VERSION 3.0.0)
project(MyProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the database library
add_library(database src/database/Database.cpp include/Database.h include/Product.h include/User.h include/Item.h)

# Link the sqlite3 library
target_link_libraries(database PRIVATE sqlite3)

# Include directories
target_include_directories(database PRIVATE include)

# Add the tests directory and GoogleTest
add_subdirectory(tests)
add_subdirectory(lib/googletest-1.14.0)



