#include "DatabaseInterface.h"
#include <sqlite3.h>

class Database : public DatabaseInterface {
    public:
        Database(const char* file_name);
        ~Database();
        std::vector<User> loadUsers() override;
        std::vector<Product> loadProducts() override;
        std::vector<Product> loadUserCart() override;
        void addToCart(Product product, int user_id) override;
        void removeFromCart(int user_id, int product_id) override;
    private:
        sqlite3 *database;
        bool fileExists(const char* fileName);
};