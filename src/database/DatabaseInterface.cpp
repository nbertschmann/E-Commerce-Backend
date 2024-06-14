#include <vector>
#include <string>

class User{
    
    public:
        User(int User_ID, std::string Username, std::string Password, std::string Email) : user_id(User_ID), username(Username), password(Password), email(Email){} 

    private:
        int user_id;
        std::string username;
        std::string password;
        std::string email;
};

class Product{

};

class DatabaseInterface
{
    public:
        virtual std::vector<User> loadUsers() = 0;

};