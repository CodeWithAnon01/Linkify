#ifndef FEED_HPP
#define FEED_HPP
#include <iostream>
#include <string>
using namespace std;

#include "../Data_Structures/Stack.hpp"

class User;
class POST;

class Feed{
    private:
    User* owner;
    Stack<POST*> recentPosts;
    int maxFeedSize = 10;

    public:
    Feed(); 
    
    void addPost(POST* post);
    void viewFeed();
    void refreshFeed();
    void removeOldPosts();
    bool isEmpty(); 
    StackNode<POST*>* getTopNode(); 
};

#endif