#include<iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h> 
#include <string>
#include "sql_utils.h"
#include "db_utils.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
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

    execute_sql_commands(db, db_path);

    sqlite3_close(db);
}