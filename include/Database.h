#include "DatabaseInterface.h"
#include <sqlite3.h>
#include <string>

class Database : public DatabaseInterface {
    public:
        Database(const char* file_name);
        ~Database();
        std::vector<Product> loadProducts() override;
        std::vector<Product> loadUserCart(int user_id) override;
        void addToCart(Product product, int user_id) override;
        void removeFromCart(int user_id, int product_id) override;
        std::string fetchPassword(std::string user_id) override;
    private:
        sqlite3 *database;
        bool fileExists(const char* fileName);
};