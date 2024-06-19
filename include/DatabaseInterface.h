#include <vector>
#include <string>
#include "User.h"
#include "Product.h"

class DatabaseInterface
{
    public:
        virtual std::vector<User> loadUsers() = 0;
        virtual std::vector<Product> loadProduct() = 0;
        virtual std::vector<Product> loadUserCart() = 0;
        virtual void addToCart(Product product, int user_id) = 0;
        virtual void removeFromCart(int user_id, int product_id) = 0;
};