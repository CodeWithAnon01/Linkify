#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../Data_Structures/Stack.hpp"
#include "../Data_Structures/LinkedList.hpp"
#include "../Data_Structures/Queue.hpp"
#include "../User/User.hpp"
#include "../Post/Post.hpp"
#include "../Feed/Feed.hpp"

class DatabaseManager;
template<typename T> class Node;

class Network{
    private:
    LinkedList<User*> users;
    Queue<POST*> posts;
    int nextUserID = 1;
    int nextPostID = 1;
    User* currentLoggedInUser = nullptr;
    DatabaseManager* db = nullptr;
    
    int countMutualFriends(User* user1, User* user2);

    public:
    struct FriendSuggestion {
        User* suggestedUser;
        int mutualCount;
        FriendSuggestion(User* user, int count) 
            : suggestedUser(user), mutualCount(count) {}
    };
    
  
    void setDatabase(DatabaseManager* database);    
    User* findUserbyName(string uname);
    User* findUser(int userId);
    bool createNewUser(string uname, string bio);

    void connectUsers(int userId1, int userId2);
    LinkedList<User*> getMutualFriends(int userId);
    vector<FriendSuggestion> getSuggestionsSorted(int userId, int limit = 5);
    
    bool createPost(int uID, string text);
    void processGlobalPostsQueue();
    
    void displayAllUsers();
    void displayNetworkStats();
    Node<User*>* getAllUsersHead();
    
    User* getCurrentUser();
    void setCurrentUser(User* user);
    
    int getNextUserID();
    int getNextPostID();
    void setNextUserID(int id);
    void setNextPostID(int id);
    
    bool createAccount(string username, string password);
    bool loginUser(string username, string password);
};

#endif