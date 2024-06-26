#ifndef DB_UTILS_H
#define DB_UTILS_H

#include <sqlite3.h>
#include <iostream>
#include "sql_utils.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void execute_sql_commands(sqlite3* db, const char* file_path,  std::string sql_commands) {
    char* zErrMsg = 0;
   
    int rc = sqlite3_exec(db, sql_commands.c_str(), callback, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << '\n';
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Tables created successfully" << std::endl;
    }
}

#endif