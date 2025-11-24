#include "userAccount.hpp"

PasswordManager* UserAccount::passwordManager = nullptr;

UserAccount::UserAccount(int id, string un){
    uid = id;
    uname = un;
    password_hash = "";
    password_salt = "";
    if(passwordManager == nullptr) {
        passwordManager = new PasswordManager();
    }
}

int UserAccount::getUID(){
    return uid;
}

bool UserAccount::SignUp(string pass){
    if(!passwordManager->isPasswordStrong(pass)){
        cout << "Password too weak!" << endl;
        return false;
    }
    
    if (pass.empty()) {
        cout << "Password cannot be empty!" << endl;
        return false;
    }
    
    password_salt = passwordManager->generateSalt();
    password_hash = passwordManager->hashedPassword(pass, password_salt);
    
    return true; 
}

bool UserAccount::Login(string pass){
    if (password_hash.empty() || password_salt.empty()) {
        cout << "Account not properly initialized!" << endl;
        return false;
    }
    
    if (pass.empty()) {
        cout << "Password cannot be empty!" << endl;
        return false;
    }
    
    string inputhashedPassword = passwordManager->hashedPassword(pass, password_salt);
    
    if(password_hash == inputhashedPassword){
        cout << "Login Successful!" << endl;
        return true;
    }
    else{
        cout << "Incorrect Password!" << endl;
        return false;
    }
}

string UserAccount::getUsername(){
    return uname;
}

void UserAccount::setUsername(string newuname){
    uname = newuname;
}

string UserAccount::getHashedPassword(){
    return password_hash;
}

string UserAccount::getSalt(){
    return password_salt;
}

string UserAccount::toFileFormat() {
    return uname + "|" + password_hash + "|" + password_salt + "|" + to_string(uid);
}

void UserAccount::fromFileFormat(string line) {
    if(line.empty()) {
        cout << "Error: Empty line to parse!" << endl;
        return;
    }
    
    stringstream ss(line);
    
    if (!getline(ss, uname, '|')) {
        cout << "Error: Could not parse username!" << endl;
        return;
    }
    
    if (!getline(ss, password_hash, '|')) {
        cout << "Error: Could not parse password hash!" << endl;
        return;
    }
    
    if (!getline(ss, password_salt, '|')) {
        cout << "Error: Could not parse salt!" << endl;
        return;
    }

    string idStr;
    if (!getline(ss, idStr, '|')) {
        cout << "Error: Could not parse user ID!" << endl;
        return;
    }
    
    try {
        uid = stoi(idStr);
    } catch (const exception& e) {
        cout << "Error: Invalid user ID format!" << endl;
        return;
    }
}