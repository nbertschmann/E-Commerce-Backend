#include <string>

class Product{
    public:
        Product(int Product_ID, std::string Name, double Price) : product_id(Product_ID), name(Name), price(Price){}
    
    private:
        int product_id;
        std::string name;
        double price;

};