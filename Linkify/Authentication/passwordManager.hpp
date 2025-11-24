#ifndef PASSWORDMANAGER_HPP
#define PASSWORDMANAGER_HPP
#include <iostream>
using namespace std;
#include <string>
class PasswordManager{
    private:
    const int prime = 31;
    const long long MOD = 1e9+7;
    const int saltLength = 8;

    public:
    string generateSalt();
    string hashedPassword(string password, string salt);
    bool verifyPassword(string input, string stored_salt, string stored_hash);
    bool isPasswordStrong(string password);
};
#endif