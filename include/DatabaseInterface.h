#include <vector>
#include <string>
#include "User.h"
#include "Product.h"
#include "Item.h"

class DatabaseInterface
{
    public:
        virtual std::vector<Product> loadProducts() = 0;
        virtual std::vector<Item> loadUserCart(int user_id) = 0;
        virtual void addToCart(int product_id, int user_id) = 0;
        virtual void removeFromCart(int user_id, int product_id) = 0;
        virtual std::string fetchPassword(std::string username) = 0;
};