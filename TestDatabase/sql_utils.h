#include <iostream>
#include <fstream>
#include <sstream>

std::string read_sql_file(const char* file_path){
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}