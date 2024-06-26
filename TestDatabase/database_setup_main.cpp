#include<iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h> 
#include <string>
#include "sql_utils.h"
#include "db_utils.h"

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

    std::string table_setup_command = read_sql_file("database_setup.sql");
    execute_sql_commands(db, db_path, table_setup_command);

    std::string table_populate_command = read_sql_file("database_populate_tables.sql");
    execute_sql_commands(db, db_path, table_populate_command);

    sqlite3_close(db);
}
