#include "DatabaseInterface.h"
#include "Item.h"
#include "Product.h"
#include <sqlite3.h>
#include <string>

class Database : public DatabaseInterface {
    public:
        Database(const char* file_name);
        ~Database();
        std::vector<Product> loadProducts() override;
        std::vector<Item> loadUserCart(int user_id) override;
        void addToCart(int product_id, int user_id) override;
        void removeFromCart(int user_id, int product_id) override;
        std::string fetchPassword(std::string username) override;
        
        // Method to execute arbitrary SQL queries.
        void executeQuery(const char* query);
    private:
        sqlite3* database;
        bool fileExists(const char* fileName);
};