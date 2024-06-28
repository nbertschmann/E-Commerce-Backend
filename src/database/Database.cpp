#include "Database.h"
#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include <iostream>
#include "Product.h"
#include "User.h"

Database::Database(const char* file_name) : database(nullptr) {
    
    sqlite3_open(file_name, &database);

    if (database == NULL) {
        printf("Failed to open DB\n");
    }
}

// Database::~Database(){
//     sqlite3_close(database);
// }

std::vector<User> Database::loadUsers() {}

std::vector<Product> Database::loadProducts() {
    
    std::vector<Product> product_list;
    
    const char* sql_command = "SELECT * FROM USERS";
    sqlite3_stmt* stmt;

    printf("Performing query...\n");
    sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);
    
    printf("Got results:\n");
    int rc;
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        int product_id = sqlite3_column_int(stmt, 0);
        const unsigned char* product_name_char = sqlite3_column_text(stmt, 1);
        
        // convert product name from const unsigned char* to string
        std::string product_name(reinterpret_cast<const char*>(product_name_char));
        double price = sqlite3_column_double(stmt, 2);
        int stock_quantity = sqlite3_column_int(stmt, 3);
        
        Product product(product_id, product_name, price, stock_quantity);

        product_list.push_back(product);
        
        printf("\n");
    }

    return product_list;

}

std::vector<Product> Database::loadUserCart() {}

void Database::addToCart(Product product, int user_id) {}

void Database::removeFromCart(int user_id, int product_id) {}