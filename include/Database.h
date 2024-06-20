#include "DatabaseInterface.h"

class Database : public DatabaseInterface {
    public:
        std::vector<User> loadUsers() override;
        std::vector<Product> loadProduct() override;
        std::vector<Product> loadUserCart() override;
        void addToCart(Product product, int user_id) override;
        void removeFromCart(int user_id, int product_id) override;
};