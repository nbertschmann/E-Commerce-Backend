#include "Database.h"
#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include <iostream>
#include "Product.h"
#include "User.h"
#include <fstream>
#include <string>

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
    
    const char* sql_command = "SELECT * FROM USER_INFORMATION";
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

std::vector<Product> Database::loadUserCart(int user_id) {
    
    std::vector<Product> user_cart;

    const char* sql_command = "SELECT pi.PRODUCT_ID, pi.product_name, pi.price, pi.stock_quantity "
                              "FROM USER_CART uc "
                              "JOIN CART_ITEMS ci ON uc.CART_ID = ci.fk_cart_id "
                              "JOIN PRODUCT_INFORMATION pi ON pi.PRODUCT_ID = ci.fk_product_id "
                              "WHERE uc.fk_user_id = ?";

    sqlite3_stmt* stmt;

    std::cout << "Performing query..." << std::endl;
    int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
    }

    // convert user_id (int) to const char* 
    const char* user_id_char = std::to_string(user_id).c_str();
    
    sqlite3_bind_text(stmt, 1, user_id_char, -1, SQLITE_STATIC);
    
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        
        int product_id = sqlite3_column_int(stmt, 0);
        const unsigned char* product_name_char = sqlite3_column_text(stmt, 1);
        
        // convert product name from const unsigned char* to string
        std::string product_name(reinterpret_cast<const char*>(product_name_char));
        double price = sqlite3_column_double(stmt, 2);
        int stock_quantity = sqlite3_column_int(stmt, 3);
        
        Product product(product_id, product_name, price, stock_quantity);

        user_cart.push_back(product);
    }

    return user_cart;
}

void Database::addToCart(Product product, int user_id) {

    int product_id = product.getProductID();
    
    const char* sql_command_1 =  "SELECT COUNT(*) " 
                                 "FROM USER_CART uc "
                                 "JOIN CART_ITEMS ci ON uc.CART_ID = ci.fk_cart_id "
                                 "WHERE uc.fk_user_id = ? "
                                 "AND ci.fk_product_id = ?";

    sqlite3_stmt* stmt;

    std::cout << "Performing query..." << std::endl;
    int rc = sqlite3_prepare_v2(database, sql_command_1, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
        return;
    }
    
    // Bind user_id to the first placeholder
    rc = sqlite3_bind_int(stmt, 1, user_id);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to bind user_id: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Bind product_id to the second placeholder
    rc = sqlite3_bind_int(stmt, 2, product_id);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to bind product_id: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    if(sqlite3_step(stmt) == SQLITE_ROW){
        
        int count = sqlite3_column_int(stmt, 0);

        if(count > 0){
            
            std::cout << "Product exists, updating count" << std::endl;

            // const char* sql_update = "UPDATE CART_ITEMS ci "
            //                          "JOIN USER_CART uc ON ci.fk_cart_id = uc.CART_ID "
            //                          "SET ci.quantity = ci.quantity + 1 "
            //                          "WHERE uc.fk_user_id = ? "
            //                          "AND ci.fk_product_id = ?";

            const char* sql_update = "UPDATE CART_ITEMS " 
                                     "SET quantity = quantity + 1 " 
                                     "WHERE fk_cart_id = "
                                     "(SELECT CART_ID FROM USER_CART "
                                     "WHERE fk_user_id = ?) " 
                                     "AND fk_product_id = ?;";

            std::cout << "Performing query..." << std::endl;
            int rc = sqlite3_prepare_v2(database, sql_update, -1, &stmt, NULL);

            if (rc != SQLITE_OK) {
                std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
                return;
            }

            rc = sqlite3_bind_int(stmt, 1, user_id);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to bind user_id: " << sqlite3_errmsg(database) << std::endl;
                sqlite3_finalize(stmt);
                return;
            }

            // Bind product_id to the second placeholder
            rc = sqlite3_bind_int(stmt, 2, product_id);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to bind product_id: " << sqlite3_errmsg(database) << std::endl;
                sqlite3_finalize(stmt);
                return;
            }

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Execution failed: " << sqlite3_errmsg(database) << std::endl;
            } else {
                std::cout << "Record inserted successfully" << std::endl;
            }
        }

        else{
            
            std::cout << "Product does not exist, adding to table" << std::endl;

            const char* sql_command = "INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity) "
                            "VALUES("
                            "(SELECT CART_ID FROM USER_CART WHERE fk_user_id = ?),"
                            "?,"
                            "1)";

            sqlite3_stmt* stmt;

            std::cout << "Performing query..." << std::endl;
            int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);

            if (rc != SQLITE_OK) {
                std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
                return;
            }
            
            // Bind user_id to the first placeholder
            rc = sqlite3_bind_int(stmt, 1, user_id);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to bind user_id: " << sqlite3_errmsg(database) << std::endl;
                sqlite3_finalize(stmt);
                return;
            }

            // Bind product_id to the second placeholder
            rc = sqlite3_bind_int(stmt, 2, product_id);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to bind product_id: " << sqlite3_errmsg(database) << std::endl;
                sqlite3_finalize(stmt);
                return;
            }

            // Execute the statement
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Execution failed: " << sqlite3_errmsg(database) << std::endl;
            } 
            else {
                std::cout << "Record inserted successfully" << std::endl;
            }

            // Finalize the statement
            sqlite3_finalize(stmt);
        }        
    }          
}


void Database::removeFromCart(int user_id, int product_id) {}

std::string Database::fetchPassword(std::string username){
    
    const char* sql_command = "SELECT * FROM USER_INFORMATION WHERE username = ?";
    sqlite3_stmt* stmt;

    std::cout << "Performing query..." << std::endl;
    int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(database) << std::endl;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    
    if(sqlite3_step(stmt) == SQLITE_ROW){
        
        const unsigned char* password_char = sqlite3_column_text(stmt, 2);
        std::string password(reinterpret_cast<const char*>(password_char));
        std::cout<<"Password: "<<password<<std::endl;
        return password;

    }

    else{
        return "";
    }

}