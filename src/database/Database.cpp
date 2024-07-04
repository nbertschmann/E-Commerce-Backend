#include "Database.h"
#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include <iostream>
#include "Product.h"
#include "User.h"
#include <fstream>

bool Database::fileExists(const char* fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

Database::Database(const char* file_name) : database(nullptr) {
    
    if (!fileExists(file_name)) {
        std::cerr << "Database file does not exist: " << file_name << std::endl;
        return;
    }
    
    int rc = sqlite3_open(file_name, &database);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(database) << std::endl;
        database = nullptr; // Ensure database is nullptr if open failed
    } else {
        std::cout << "Opened DB\n";
    }
}

Database::~Database(){
    if(database){
        sqlite3_close(database);
    }
}

std::vector<User> Database::loadUsers() {
    
    std::vector<User> user_list;
    
    const char* sql_command = "SELECT * FROM PRODUCT_INFORMATION";
    sqlite3_stmt* stmt;

    std::cout << "Performing query..." << std::endl;
    int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
        return user_list;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        
        const unsigned char *username_char = sqlite3_column_text(stmt, 1);
        std::string username(reinterpret_cast<const char*>(username_char));

        const unsigned char *password_char = sqlite3_column_text(stmt, 2);
        std::string password(reinterpret_cast<const char*>(password_char));

        const unsigned char *email_char = sqlite3_column_text(stmt, 3);
        std::string email(reinterpret_cast<const char*>(email_char));

        User user(user_id, username, password, email);

        user_list.push_back(user);
    }

    sqlite3_finalize(stmt);
    return user_list;

}

std::vector<Product> Database::loadProducts() {
    
    std::vector<Product> product_list;
    
    const char* sql_command = "SELECT * FROM PRODUCT_INFORMATION";
    sqlite3_stmt* stmt;

    std::cout << "Performing query..." << std::endl;
    int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
        return product_list;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        int product_id = sqlite3_column_int(stmt, 0);
        const unsigned char* product_name_char = sqlite3_column_text(stmt, 1);
        
        // convert product name from const unsigned char* to string
        std::string product_name(reinterpret_cast<const char*>(product_name_char));
        double price = sqlite3_column_double(stmt, 2);
        int stock_quantity = sqlite3_column_int(stmt, 3);
        
        Product product(product_id, product_name, price, stock_quantity);

        product_list.push_back(product);
    }

    sqlite3_finalize(stmt);
    return product_list;
}

std::vector<Product> Database::loadUserCart() {
    std::vector<Product> test_product_list;
    return test_product_list;
}

void Database::addToCart(Product product, int user_id) {}

void Database::removeFromCart(int user_id, int product_id) {}