#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
using namespace std;

class UserAccount;
class POST;

class DatabaseManager {
private:
    const string DATABASE_PATH = "./Database/";
    const string USERS_FILE = "users.txt";
    const string POSTS_FILE = "posts.txt";           
    const string FRIENDSHIPS_FILE = "friendships.txt"; 

    

public:
    DatabaseManager();
    string getFullPath(string filename);
    bool fileExists(string filepath);
    bool createFileIfNotExists(string filepath);

    bool initializeDatabase();

    bool saveUserAccount(UserAccount* account);
    UserAccount* loadUserAccount(string username);
    bool userExists(string username);
    bool deleteUserAccount(string username);
    vector<UserAccount*> loadAllUsers();
    
   
    bool savePost(POST* post);
    vector<POST*> loadAllPosts();
    bool deletePost(int postID);
    
    bool saveFriendship(int userId1, int userId2);
    vector<pair<int, int>> loadAllFriendships();
    bool deleteFriendship(int userId1, int userId2);
};

#endif