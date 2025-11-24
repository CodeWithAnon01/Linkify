#include "User.hpp"
#include "../Feed/Feed.hpp"

User::User() : UID(0), username(""), bio(""), feed(nullptr) {}

int User::getID(){return UID;}
string User::getUserName(){return username;}
string User::getBio(){return bio;}
void User::setID(int id){UID = id;}
void User::setUserName(string uname){username = uname;}
void User::setBio(string b){bio = b;}

void User::addFriend(User* userfriend){
    Friends.push_back(userfriend);
}

void User::removeFriend(User* userfriend){
    Friends.remove(userfriend);
}

LinkedList<User*>& User::getFriends(){
    return this->Friends;
}

bool User::isFriendWith(User* other){
    return Friends.contains(other);
}

void User::setFeed(Feed* Feed){
    this->feed = Feed;
}

Feed* User::getFeed(){
    return this->feed;
}