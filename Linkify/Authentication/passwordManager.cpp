#include "passwordManager.hpp"
#include <ctime>

    string PasswordManager::generateSalt(){
        string characterpool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
         srand(time(0));

    string result = "";

    for (int i = 0; i < saltLength; i++) {
        int index = rand() % characterpool.size();
        result += characterpool[index];
    }

    return result;
    }
    string PasswordManager:: hashedPassword(string password, string salt){
        long long int hash = 0;
        long long prime_power = 1;
        string combinedPass = password+salt;
        for(size_t i = 0; i<combinedPass.length(); i++){ 
        hash = (hash + int(combinedPass[i]) * prime_power) % MOD;
        prime_power= (prime_power*prime)%MOD;
        }
        return to_string(hash);
     }

    bool PasswordManager:: verifyPassword(string input, string stored_salt, string stored_hash){
        return hashedPassword(input,stored_salt)==stored_hash;
    }

    bool PasswordManager::isPasswordStrong(string password) {
    if (password.length() < 6){
        return false;}

    bool hasDigit = false;
    bool hasLetter = false;

    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        if (isalpha(c)) hasLetter = true;
    }

    return hasDigit && hasLetter;
}
