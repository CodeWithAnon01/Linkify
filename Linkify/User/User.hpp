#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <string>
using namespace std;

#include "../Data_Structures/LinkedList.hpp"

class Feed;
class POST;

class User{
    private:
    int UID;
    string username;
    string bio;
    LinkedList<User*> Friends;
    Feed* feed;

    public:
    User();
    
    int getID();
    void setID(int id);
    string getUserName();
    string getBio();
    void setUserName(string uname);
    void setBio(string b);

    void addFriend(User* userfriend);
    void removeFriend(User* userfriend);
    LinkedList<User*>& getFriends();

    bool isFriendWith(User* other);
    void setFeed(Feed* Feed);
    Feed* getFeed();
};

#endif