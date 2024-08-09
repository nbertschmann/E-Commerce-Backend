#ifndef USER_H
#define USER_H

#include <string>

class User{
    
    public:
        User(int User_ID, std::string Username, std::string Password, std::string Email) : user_id(User_ID), username(Username), password(Password), email(Email){} 

        int getUserId(){
            return user_id;
        }

        std::string getUsername(){
            return username;
        }

        std::string getUserPassword(){
            return password;
        }

        std::string getUserEmail(){
            return email;
        }

    private:
        int user_id;
        std::string username;
        std::string password;
        std::string email;
};

#endif