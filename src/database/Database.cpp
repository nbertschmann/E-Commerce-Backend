#include "Database.h"

std::vector<User> Database::loadUsers() {}
std::vector<Product> Database::loadProduct() {}
std::vector<Product> Database::loadUserCart() {}
void Database::addToCart(Product product, int user_id) {}
void Database::removeFromCart(int user_id, int product_id) {}