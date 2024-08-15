#include "Database.h"
#include "Product.h"
#include "User.h"
#include "Item.h"
#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class PrepareStatement{

    public:
        PrepareStatement(const char* sql_command, sqlite3* database) : stmt(nullptr){
            int rc = sqlite3_prepare_v2(database, sql_command, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                throw std::runtime_error(sqlite3_errmsg(database)); 
            }
        };

        ~PrepareStatement(){
            if (stmt){
                sqlite3_finalize(stmt);
            }  
        }

        sqlite3_stmt* get_stmt(){
            return stmt;
        }

    private:
        sqlite3_stmt* stmt;

};

bool Database::fileExists(const char* fileName) {
    return std::filesystem::exists(fileName);
}

Database::Database(const char* file_name) : database(nullptr){
    
    if (!fileExists(file_name)) {
        throw std::runtime_error("Database file does not exist: " + std::string(file_name));
    }
    
    int rc = sqlite3_open(file_name, &database);

    if (rc != SQLITE_OK) {
        std::string err_msg = "Failed to open DB" + std::string(sqlite3_errmsg(database));
        sqlite3_close(database);
        throw std::runtime_error(err_msg);
    } 
    else {
        std::cout << "Opened DB\n";
    }
}

Database::~Database(){
    if(database){
        sqlite3_close(database);
    }
}

std::vector<Product> Database::loadProducts() {
    
    std::vector<Product> product_list;
    
    const char* sql_command = "SELECT * FROM PRODUCT_INFORMATION";
                                
    PrepareStatement query1(sql_command, database);

    while ((sqlite3_step(query1.get_stmt())) == SQLITE_ROW) {
        
        int product_id = sqlite3_column_int(query1.get_stmt(), 0);
        const unsigned char* product_name_char = sqlite3_column_text(query1.get_stmt(), 1);
        
        // convert product name from const unsigned char* to string
        std::string product_name(reinterpret_cast<const char*>(product_name_char));
        double price = sqlite3_column_double(query1.get_stmt(), 2);
        int stock_quantity = sqlite3_column_int(query1.get_stmt(), 3);
        
        Product product(product_id, product_name, price, stock_quantity);

        product_list.push_back(product);
    }
    return product_list;
}

std::vector<Item> Database::loadUserCart(int user_id) {
    
    std::vector<Item> user_cart;

    const char* sql_command = "SELECT pi.PRODUCT_ID, pi.product_name, pi.price, pi.stock_quantity, ci.quantity "
                              "FROM USER_CART uc "
                              "JOIN CART_ITEMS ci ON uc.CART_ID = ci.fk_cart_id "
                              "JOIN PRODUCT_INFORMATION pi ON pi.PRODUCT_ID = ci.fk_product_id "
                              "WHERE uc.fk_user_id = ?";

    PrepareStatement query1(sql_command, database);

    // convert user_id (int) to const char* 
    const char* user_id_char = std::to_string(user_id).c_str();
    
    sqlite3_bind_int(query1.get_stmt(), 1, user_id);
    
    while ((sqlite3_step(query1.get_stmt())) == SQLITE_ROW) {
        
        int product_id = sqlite3_column_int(query1.get_stmt(), 0);
        const unsigned char* product_name_char = sqlite3_column_text(query1.get_stmt(), 1);
        
        // convert product name from const unsigned char* to string
        std::string product_name(reinterpret_cast<const char*>(product_name_char));
        double price = sqlite3_column_double(query1.get_stmt(), 2);
        int stock_quantity = sqlite3_column_int(query1.get_stmt(), 3);
        int cart_quantity = sqlite3_column_int(query1.get_stmt(), 4);
        
        Product product(product_id, product_name, price, stock_quantity);
        Item cart_item(product, cart_quantity);

        user_cart.push_back(cart_item);
    }

    return user_cart;
}

void Database::addToCart(Product product, int user_id) {

    int product_id = product.getProductID();
    
    const char* sql_command =  "SELECT COUNT(*) " 
                               "FROM USER_CART uc "
                               "JOIN CART_ITEMS ci ON uc.CART_ID = ci.fk_cart_id "
                               "WHERE uc.fk_user_id = ? "
                               "AND ci.fk_product_id = ?";

    PrepareStatement stmt_query(sql_command, database);

    // Bind user_id to the first placeholder
    sqlite3_bind_int(stmt_query.get_stmt(), 1, user_id);

    // Bind product_id to the second placeholder
    sqlite3_bind_int(stmt_query.get_stmt(), 2, product_id);

    if(sqlite3_step(stmt_query.get_stmt()) == SQLITE_ROW){
        
        int count = sqlite3_column_int(stmt_query.get_stmt(), 0);

        if(count > 0){
            
            std::cout << "Product Exists, Updating Count" << std::endl;

            const char* sql_command = "UPDATE CART_ITEMS " 
                                     "SET quantity = quantity + 1 " 
                                     "WHERE fk_cart_id = "
                                     "(SELECT CART_ID FROM USER_CART "
                                     "WHERE fk_user_id = ?) " 
                                     "AND fk_product_id = ?;";
            
            PrepareStatement stmt_update(sql_command, database);
            
            // Bind user_id to the second placeholder
            sqlite3_bind_int(stmt_update.get_stmt(), 1, user_id);
    
            // Bind product_id to the second placeholder
            sqlite3_bind_int(stmt_update.get_stmt(), 2, product_id);
   
            int rc = sqlite3_step(stmt_update.get_stmt());
            if (rc != SQLITE_DONE) {
                throw std::runtime_error(sqlite3_errmsg(database));
            }
            else{
                std::cout << "Count Updated Successfully";
            }
        }

        else{
            
            std::cout << "Product Does Not Exist, Adding to Table" << std::endl;

            const char* sql_command = "INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity) "
                            "VALUES("
                            "(SELECT CART_ID FROM USER_CART WHERE fk_user_id = ?),"
                            "?,"
                            "1)";

            PrepareStatement stmt_insert(sql_command, database);
            
            // Bind user_id to the first placeholder
            sqlite3_bind_int(stmt_insert.get_stmt(), 1, user_id);

            // Bind product_id to the second placeholder
            sqlite3_bind_int(stmt_insert.get_stmt(), 2, product_id);

            // Execute the statement
            int rc = sqlite3_step(stmt_insert.get_stmt());
            if (rc != SQLITE_DONE) {
                throw std::runtime_error(sqlite3_errmsg(database));
            }
            else{
                std::cout << "Product Inserted Successfully!";
            }
        }        
    }          
}

void Database::removeFromCart(int user_id, int product_id) {
    
    const char* sql_command = "DELETE FROM CART_ITEMS "
                              "WHERE fk_cart_id = (SELECT CART_ID FROM USER_CART WHERE fk_user_id = ?) "
                              "AND fk_product_id = ?";
    
    PrepareStatement stmt_delete(sql_command, database);

    sqlite3_bind_int(stmt_delete.get_stmt(), 1, user_id);
    sqlite3_bind_int(stmt_delete.get_stmt(), 2, product_id);

    int rc = sqlite3_step(stmt_delete.get_stmt());
    if (rc != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(database));
    }
    else{
        std::cout << "Cart Item Deleted Successfully!";
    }
}

std::string Database::fetchPassword(std::string username){
    
    const char* sql_command = "SELECT * FROM USER_INFORMATION WHERE username = ?";
    PrepareStatement stmt_insert(sql_command, database);

    sqlite3_bind_text(stmt_insert.get_stmt(), 1, username.c_str(), -1, SQLITE_STATIC);
    
    if(sqlite3_step(stmt_insert.get_stmt()) == SQLITE_ROW){
        
        const unsigned char* password_char = sqlite3_column_text(stmt_insert.get_stmt(), 2);
        std::string password(reinterpret_cast<const char*>(password_char));
        std::cout<<"Password: "<<password<<std::endl;
        return password;
    }

    else{
        return "";
    }
}

void Database::executeQuery(const char* sql_query){
    
    PrepareStatement stmt_query(sql_query, database);

    int rc = sqlite3_step(stmt_query.get_stmt());
    if (rc != SQLITE_DONE) {
        throw std::runtime_error(sqlite3_errmsg(database));
    }
    else{
        std::string query_str(sql_query);
        std::cout << "Executed Statement Successfully: " + query_str << std::endl;
    }
}