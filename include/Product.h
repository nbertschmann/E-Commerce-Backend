#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product{
    public:
        Product(int Product_ID, std::string Name, double Price, int Stock_Qty) : product_id(Product_ID), product_name(Name), price(Price), stock_qty(Stock_Qty) {}

        int getProductID() const {
            return product_id;
        }

        std::string getProductName() const {
            return product_name;
        }

        double getPrice() const {
            return price;
        }

        int getStockQty() const {
            return stock_qty;
        }

    private:
        int product_id;
        std::string product_name;
        double price;
        int stock_qty;
};

#endif