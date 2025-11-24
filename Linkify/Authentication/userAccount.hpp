#ifndef USERACCOUNT_HPP
#define USERACCOUNT_HPP
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

#include "passwordManager.hpp"
class UserAccount{
    private:
    int uid;
    string uname;
    string password_hash;
    string password_salt;
    static PasswordManager* passwordManager;
    public:
    UserAccount(int id,string un);
    string getUsername();
    void setUsername(string newuname);
    int getUID();
    bool SignUp(string pass);
    bool Login(string pass);
    string getHashedPassword();
    string getSalt();
    string toFileFormat();
    void fromFileFormat(string line);

    
};




#endif