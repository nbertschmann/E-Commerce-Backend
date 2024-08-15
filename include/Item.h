#ifndef ITEM_H
#define ITEM_H

#include "Product.h"

class Item {
    private:
        Product product;
        int quantity;
        
    public:

        Item(Product _product, int _quantity=0) : product(_product), quantity(_quantity) {}

        int getQuantity(){
            return quantity;
        }

        void incrementQuantity(){
            quantity++;
        }

        const Product& getProduct() const {
            return product;
        }
};

#endif