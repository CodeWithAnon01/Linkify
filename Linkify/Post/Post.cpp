#include "Post.hpp"
#include "../User/User.hpp"

POST::POST(int pid) : postID(pid), author(nullptr), content(""), likes(0) {}

int POST::getPID(){
    return postID;
}

int POST::getLikes(){
    return likes;
}

User* POST::getAuthor(){
    return author;
}

string POST::getContent(){
    return content;
}

void POST::addComment(string comment){
    Comments.enqueue(comment);
}

void POST::viewComments(){
    cout << "\n";
    
    if(Comments.isEmpty()) {
        cout << "+========================================+\n";
        cout << "|      No comments yet!                  |\n";
        cout << "|   Be the first to comment!             |\n";
        cout << "+========================================+\n";
        return;
    }
    
    cout << "+========================================+\n";
    cout << "|            COMMENTS                    |\n";
    cout << "|  Total: " << Comments.getSize() << " comment(s)              |\n";
    cout << "+========================================+\n\n";
    
    QueueNode<string>* current = Comments.getFront();
    int commentNumber = 1;
    
    while(current != nullptr) {
        cout << "+----------------------------------------+\n";
        cout << "| Comment " << commentNumber << ":\n";
        cout << "| " << current->data << "\n";
        cout << "+----------------------------------------+\n\n";
        
        current = current->next;
        commentNumber++;
    }
    
    cout << "----------------------------------------\n";
    cout << "Press Enter to return to post menu...\n";
    cin.ignore(10000, '\n'); 
}

void POST::addLike(){
    likes++;
}

void POST::removeLike(){
    likes--;
}

void POST::displayPost(){
    int choice;
    cout << "POST-" << postID << "\nAuthor: " << author->getUserName() 
         << "\nPost content: " << content << "\nLikes: " << likes 
         << "\nTotal-comments: " << Comments.getSize() 
         << "\nEnter 1 if you wanna see the comments: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            viewComments();
            break;
        default:
            cout << "\nInvalid Choice" << endl;
            return;
    }
}

void POST::setAuth(User* u){
    author = u;
}

void POST::setContent(string con){
    content = con;
}

int POST::getTotalComments(){
    return Comments.getSize();
}