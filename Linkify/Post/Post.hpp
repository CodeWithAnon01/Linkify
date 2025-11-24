#ifndef POST_HPP
#define POST_HPP
#include <iostream>
#include <string>
using namespace std;

#include "../Data_Structures/Queue.hpp"

class User;

class POST{
    private:
    int postID;
    User* author;
    string content;
    int likes;
    Queue<string> Comments;

    public:
    POST(int pid);
    
    int getTotalComments();
    int getPID();
    int getLikes();
    User* getAuthor();
    string getContent();
    void addComment(string comment);
    void viewComments();
    void addLike();
    void setAuth(User* u);
    void setContent(string con);
    void removeLike();
    void displayPost();
};

#endif