# if(BUILD_TESTING)
#     # add the main file DatabaseTest.cpp as executable, plus the source files that DatabaseTest.cpp depends on
#     add_executable(DatabaseTest DatabaseTest.cpp)

#     # Link the test executable with the Database library and sqlite3
#     target_include_directories(DatabaseTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/sqlite/include)
    
#     # Link the GoogleTest libraries
#     target_link_libraries(DatabaseTest PRIVATE GTest::gtest_main)

#     # Enable testing
#     enable_testing()

#     # Discover and register all tests in DatabaseTest
#     include(GoogleTest)
#     gtest_discover_tests(DatabaseTest)

#     # Alternatively we can use:
#     #add_test(NAME DatabaseTest COMMAND DatabaseTest)
# endif()

#################################################################################################################

# # add the main file DatabaseTest.cpp as executable, plus the source files that DatabaseTest.cpp depends on
# add_executable(DatabaseTest DatabaseTest.cpp)

# # Link the test executable with the Database library and sqlite3
# target_include_directories(DatabaseTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/sqlite/include)

# # Link the GoogleTest libraries
# target_link_libraries(DatabaseTest PRIVATE database sqlite3 GTest::gtest_main)

# # Enable testing
# enable_testing()

# # Discover and register all tests in DatabaseTest
# include(GoogleTest)
# gtest_discover_tests(DatabaseTest)

# # Alternatively we can use:
# #add_test(NAME DatabaseTest COMMAND DatabaseTest)

##################################################################################################################

# Add the main test file as executable
add_executable(DatabaseTest DatabaseTest.cpp)

# Include directories for Database and SQLite
target_include_directories(DatabaseTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/sqlite/include)

# Link the test executable with the Database library, sqlite3, and GoogleTest
target_link_libraries(DatabaseTest PRIVATE database sqlite3 GTest::gtest_main)

# Enable testing
enable_testing()

# Discover and register all tests in DatabaseTest
include(GoogleTest)
gtest_discover_tests(DatabaseTest)
