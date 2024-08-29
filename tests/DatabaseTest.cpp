#include "gtest/gtest.h"
#include "Database.h"
#include "Product.h"
#include "Item.h"
#include "User.h"
#include <vector>
#include <iostream>
#include <string>

class DatabaseTest : public testing::Test {
    protected:
        
        Database* db;

        void SetUp() override {

            // Create instance of 'Database' class that stores SQLite DB in memory (rather than on disk)
            db = new Database(":memory:");

            // Create the USER_INFORMATION table
            db->executeQuery("CREATE TABLE USER_INFORMATION ("
                            "USER_ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "username TEXT NOT NULL, "
                            "password TEXT NOT NULL, "
                            "email TEXT);");

            // Create the PRODUCT_INFORMATION table
            db->executeQuery("CREATE TABLE PRODUCT_INFORMATION ("
                            "PRODUCT_ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "product_name TEXT NOT NULL, "
                            "price REAL NOT NULL, "
                            "stock_quantity INT NOT NULL);");

            // Create the USER_CART table
            db->executeQuery("CREATE TABLE USER_CART ("
                            "CART_ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "fk_user_id INT NOT NULL UNIQUE, "
                            "FOREIGN KEY (fk_user_id) REFERENCES USER_INFORMATION(USER_ID));");

            // Create the CART_ITEMS table
            db->executeQuery("CREATE TABLE CART_ITEMS ("
                            "ITEM_ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "fk_cart_id INT NOT NULL, "
                            "fk_product_id INT NOT NULL, "
                            "quantity INT NOT NULL, "
                            "FOREIGN KEY (fk_cart_id) REFERENCES USER_CART(CART_ID), "
                            "FOREIGN KEY (fk_product_id) REFERENCES PRODUCT_INFORMATION(PRODUCT_ID));");

            // Insert Products into PRODUCT_INFORMATION table
            db->executeQuery("INSERT INTO PRODUCT_INFORMATION (product_name, price, stock_quantity) VALUES ('Milk', 5.99, 7);");
            db->executeQuery("INSERT INTO PRODUCT_INFORMATION (product_name, price, stock_quantity) VALUES ('Eggs', 3.99, 20);");

            // Insert Users into USER_INFORMATION table
            db->executeQuery("INSERT INTO USER_INFORMATION (username, password, email) VALUES ('Bob Smith', 'bobby76', 'bob.smith@hotmail.com');");
            db->executeQuery("INSERT INTO USER_INFORMATION (username, password, email) VALUES ('Sam Melrose', 'IloveSQL22', 'sam_melrose@gmail.com');");    

            // Insert Cart Associated with Users
            db->executeQuery("INSERT INTO USER_CART (fk_user_id) VALUES ((SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Bob Smith'));");
            db->executeQuery("INSERT INTO USER_CART (fk_user_id) VALUES ((SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Sam Melrose'));");

            // Add Products to 'Bob Smith' Cart
            db->executeQuery("INSERT INTO CART_ITEMS (fk_cart_id, fk_product_id, quantity)" 
                             "VALUES ((SELECT CART_ID FROM USER_CART WHERE fk_user_id = (SELECT USER_ID FROM USER_INFORMATION WHERE username = 'Bob Smith')), "
                             "(SELECT PRODUCT_ID FROM PRODUCT_INFORMATION WHERE product_name = 'Milk'), 2);");
    }

    void Teardown(){
        delete db;
    }
};

TEST_F(DatabaseTest, LoadProducts){

    std::vector<Product> products = db->loadProducts();

    ASSERT_EQ(products.at(0).getProductID(), 1) << "Expected Product ID to be 1 but got " << products.at(0).getProductID();
    ASSERT_EQ(products.at(0).getProductName(), "Milk") << "Expected Product Name to be 'Milk' but got " << products.at(0).getProductName();
    ASSERT_DOUBLE_EQ(products.at(0).getProductPrice(), 5.99) << "Expected Product Price to be 5.99 but got " << products.at(0).getProductPrice();
    ASSERT_EQ(products.at(0).getStockQty(), 7) << "Expected Product Stock Quantity to be 7 but got "  << products.at(0).getStockQty();

    ASSERT_EQ(products.at(1).getProductID(), 2) << "Expected Product ID to be 2 but got " << products.at(0).getProductID();
    ASSERT_EQ(products.at(1).getProductName(), "Eggs") << "Expected Product Name to be 'Eggs' but got " << products.at(0).getProductName();
    ASSERT_DOUBLE_EQ(products.at(1).getProductPrice(), 3.99) << "Expected Product Price to be 3.99 but got " << products.at(0).getProductPrice();
    ASSERT_EQ(products.at(1).getStockQty(), 20) << "Expected Product Stock Quantity to be 20 but got "  << products.at(0).getStockQty();
}

TEST_F(DatabaseTest, LoadUserCart){

    int user_id = 1; // User: Bob Smith
    std::vector<Item> cart_items = db->loadUserCart(user_id);

    ASSERT_EQ(cart_items.at(0).getProduct().getProductID(), 1) << "Expected Product ID to be 1 but got " << cart_items.at(0).getProduct().getProductID();
    ASSERT_EQ(cart_items.at(0).getProduct().getProductName(), "Milk") << "Expected Product Name to be 'Milk' but got " << cart_items.at(0).getProduct().getProductName();
    ASSERT_DOUBLE_EQ(cart_items.at(0).getProduct().getProductPrice(), 5.99) << "Expected Product Price to be 5.99 but got " << cart_items.at(0).getProduct().getProductPrice();
    ASSERT_EQ(cart_items.at(0).getProduct().getStockQty(), 7) << "Expected Product Stock Quantity to be 7 but got " << cart_items.at(0).getProduct().getStockQty();    
}

// Add new product to Cart and verify that it is added
TEST_F(DatabaseTest, AddToCartNew){

    int user_id = 2; // User: Sam Melrose
    int product_id = 1; // Product: Milk
    
    db->addToCart(product_id, user_id);
    std::vector<Item> cart_items = db->loadUserCart(user_id);

    // Verify that the cart has 1 item
    ASSERT_EQ(cart_items.size(), 1) << "Expected Cart to contain 1 Item but got " << cart_items.size();
        
    ASSERT_EQ(cart_items.at(0).getProduct().getProductID(), 1);
    ASSERT_EQ(cart_items.at(0).getProduct().getProductName(), "Milk");
    ASSERT_DOUBLE_EQ(cart_items.at(0).getProduct().getProductPrice(), 5.99);
    ASSERT_EQ(cart_items.at(0).getProduct().getStockQty(), 7);    
    
    // Verify the quantity of the product in the cart
    ASSERT_EQ(cart_items.at(0).getQuantity(), 1) << "Expected quantity of Product in Cart to be 1 but got " << cart_items.at(0).getQuantity();
}

// Add existing product to Cart and verify that quantity increments
TEST_F(DatabaseTest, AddToCartExisting){

    int product_id = 2; // Product: Eggs
    int user_id = 2; // User: Sam Melrose

    db->addToCart(product_id, user_id);
    db->addToCart(product_id, user_id);

    std::vector<Item> cart_items = db->loadUserCart(user_id);

    // Verify that the cart only has 1 item
    ASSERT_EQ(cart_items.size(), 1) << "Expected quantity of Product in Cart to be 1 but got " << cart_items.at(0).getQuantity();

    // Verify the product in the cart
    ASSERT_EQ(cart_items.at(0).getProduct().getProductID(), 2);
    ASSERT_EQ(cart_items.at(0).getProduct().getProductName(), "Eggs");
    ASSERT_DOUBLE_EQ(cart_items.at(0).getProduct().getProductPrice(), 3.99);
    ASSERT_EQ(cart_items.at(0).getProduct().getStockQty(), 20);    
    
    // Verify the quantity of the product in the cart
    ASSERT_EQ(cart_items.at(0).getQuantity(), 2) << "Expected quantity of product in Cart to be 2 but got " << cart_items.at(0).getQuantity();
}

TEST_F(DatabaseTest, RemoveFromCartDecrement){

    int user_id = 1; // User: Bob Smith
    int product_id = 1; // Product: Milk

    std::vector<Item> cart_items = db->loadUserCart(user_id);

    // Verify initial state
    ASSERT_EQ(cart_items.size(), 1) << "Expected Cart size to be 1 but got " << cart_items.at(0).getQuantity();
    ASSERT_EQ(cart_items.at(0).getProduct().getProductID(), 1);
    ASSERT_EQ(cart_items.at(0).getProduct().getProductName(), "Milk");
    ASSERT_DOUBLE_EQ(cart_items.at(0).getProduct().getProductPrice(), 5.99);
    ASSERT_EQ(cart_items.at(0).getProduct().getStockQty(), 7);    
    ASSERT_EQ(cart_items.at(0).getQuantity(), 2) << "Expected quantity in cart to be 2 but got " << cart_items.at(0).getQuantity();

    db->removeFromCart(user_id, product_id);

    cart_items = db->loadUserCart(user_id);

    // Verify the cart still has the item and its quantity has decreased by 1
    ASSERT_EQ(cart_items.size(), 1) << "Expected Cart size to be 1 but got " << cart_items.at(0).getQuantity();
    ASSERT_EQ(cart_items.at(0).getQuantity(), 1) << "Expected quantity in cart to be 1 but got " << cart_items.at(0).getQuantity();;
}

TEST_F(DatabaseTest, RemoveFromCartDelete){
    
    int user_id = 1; // User: Bob Smith
    int product_id = 1; // Product: Milk

    std::vector<Item> cart_items = db->loadUserCart(user_id);

    // Verify initial state
    ASSERT_EQ(cart_items.size(), 1) << "Expected Cart size to be 1 but got " << cart_items.at(0).getQuantity();
    ASSERT_EQ(cart_items.at(0).getProduct().getProductID(), 1);
    ASSERT_EQ(cart_items.at(0).getProduct().getProductName(), "Milk");
    ASSERT_DOUBLE_EQ(cart_items.at(0).getProduct().getProductPrice(), 5.99);
    ASSERT_EQ(cart_items.at(0).getProduct().getStockQty(), 7);    
    ASSERT_EQ(cart_items.at(0).getQuantity(), 2) << "Expected quantity in cart to be 2 but got " << cart_items.at(0).getQuantity();

    db->removeFromCart(user_id, product_id);
    db->removeFromCart(user_id, product_id);

    cart_items = db->loadUserCart(user_id);

    // Verify that item is no longer in cart
    ASSERT_TRUE(cart_items.empty()) << "Expected cart to be empty after fully removing Item from Cart";
}

TEST_F(DatabaseTest, FetchPassword){

    std::string password = db->fetchPassword("Bob Smith");

    ASSERT_EQ(password, "bobby76") << "Expected Password to be bobby76 but got: " << password;
}