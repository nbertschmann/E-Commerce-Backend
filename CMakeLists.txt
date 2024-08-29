# cmake_minimum_required(VERSION 3.0.0)
# project(MyProject)

# # Add the executable for database setup
# add_library(database src/database/Database.cpp)

# # Link the sqlite3 library
# target_link_libraries(database PRIVATE sqlite3)

# # add the tests directory
# add_subdirectory(tests)
# add_subdirectory(lib/googletest-1.14.0)

# target_include_directories(database PRIVATE include)

#############################################################################################################

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

#************************************************************************************************************************************

# cmake_minimum_required(VERSION 3.14)
# project(MyProject)

# # GoogleTest requires at least C++14
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Enable testing
# enable_testing()

# # Include FetchContent module
# include(FetchContent)

# # Download and configure GoogleTest version 1.15.2 using FetchContent
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
# )

# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# # Make GoogleTest available
# FetchContent_MakeAvailable(googletest)

# # Add the database library
# add_library(database 
#     src/database/Database.cpp 
#     include/Database.h 
#     include/Product.h 
#     include/User.h 
#     include/Item.h
# )

# # Link the sqlite3 library
# target_link_libraries(database PRIVATE sqlite3)

# # Include directories for the database library
# target_include_directories(database PRIVATE include)

# # Add the test executable for DatabaseTests
# add_executable(DatabaseTests tests/DatabaseTest.cpp)

# # Include directories for DatabaseTests
# target_include_directories(DatabaseTests PRIVATE include)

# # Link GoogleTest and the database library to the tests
# target_link_libraries(DatabaseTests PRIVATE gtest gtest_main database)

# # Register the tests with CTest
# add_test(NAME DatabaseTests COMMAND DatabaseTests)

