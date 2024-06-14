#include<iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void execute_sql_commands(sqlite3* db, const char* sql_commands) {
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql_commands, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << '\n';
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Tables created successfully" << std::endl;
    }
}

std::string read_sql_file(const char* file_path){
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[])
{
    sqlite3* db;
    int rc;
   
     /* Open database */
    const char* db_path = "data/test_database.db";

    rc = sqlite3_open(db_path, &db);
   
    if(rc) {
        std::cerr << "Can't open database: " << std::endl << sqlite3_errmsg(db); 
        return(0);
    } 
    else {
        
        std::cout << "Opened database successfully" << std::endl;
    }

    std::string sql_commands = read_sql_file("database_setup.sql");

    execute_sql_commands(db, sql_commands.c_str());

    sqlite3_close(db);
}