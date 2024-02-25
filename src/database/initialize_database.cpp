// From https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
// #include <stdio.h>
// #include <stdlib.h>
#include<iostream>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void create_table(sqlite3* db, const char* sql_commands[], int num_commands) 
{   
    char* zErrMsg = 0;
    int rc;

    for(int i=0; i<num_commands; i++)
    {
        rc = sqlite3_exec(db, sql_commands[i], callback, 0, &zErrMsg);
    
        if(rc != SQLITE_OK){
            //fprintf(stderr, "SQL error: %s\n", zErrMsg);
            std::cerr << "SQL error: " << zErrMsg << '\n';
            sqlite3_free(zErrMsg);
        } 
        else {
            //fprintf(stdout, "Table created successfully\n");
            std::cout << "Table created successfully" << std::endl;
        }
        
    }
}

int main(int argc, char* argv[])
{
    sqlite3* db;
    int rc;
   
     /* Open database */
    
    const char* db_path = "../db/E-COMMERCE-DB.db";

    rc = sqlite3_open(db_path, &db);
   
    if(rc) {
        //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        std::cerr << "Can't open database: " << std::endl << sqlite3_errmsg(db); 
        return(0);
    } 
    else {
        //fprintf(stdout, "Opened database successfully\n");
        std::cout << "Opened database successfully" << std::endl;
    }

   /* Create SQL statement */
    const char* sql_commands[] = { 
        
        "CREATE TABLE USER_INFORMATION ("  
        "USER_ID        INT         NOT NULL," 
        "username       CHAR(50)    NOT NULL," 
        "password       CHAR(50)    NOT NULL," 
        "email          CHAR(50)," 
        "PRIMARY KEY (USER_ID));",

        "CREATE TABLE PRODUCT_INORMATION ("
        "PRODUCT_ID     INT         NOT NULL,"
        "product_name   CHAR(50)    NOT NULL,"
        "price          REAL        NOT NULL,"         
        "stock_quantity REAL        NOT NULL,"      
        "PRIMARY KEY (PRODUCT_ID));",

        "CREATE TABLE USER_CART ("
        "CART_ID        INT         NOT NULL,"
        "fk_user_id     INT         NOT NULL,"
        "PRIMARY KEY (CART_ID),"
        "FOREIGN KEY (fk_user_id) REFERENCES UserInformation(USER_ID));"

        "CREATE TABLE CART_ITEMS ("
        "ITEM_ID        INT         NOT NULL,"
        "fk_cart_id     INT         NOT NULL,"
        "fk_product_id  INT         NOT NULL,"
        "quantity       INT         NOT NULL,"
        "PRIMARY KEY (ITEM_ID),"
        "FOREIGN KEY (fk_cart_id) REFERENCES UserInformation(USER_ID),"
        "FOREIGN KEY (fk_product_id) REFERENCES UserInformation(USER_ID));"

        };
    
    int num_commands = sizeof(sql_commands)/sizeof(sql_commands[0]);

    create_table(db, sql_commands, num_commands);

    sqlite3_close(db);
}