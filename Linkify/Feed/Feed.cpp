#include "Feed.hpp"
#include "../Post/Post.hpp"
#include "../User/User.hpp"


Feed::Feed() : owner(nullptr) {}

void Feed::addPost(POST* post){
    if(recentPosts.getSize() >= maxFeedSize){
        recentPosts.pop();
    }
    recentPosts.push(post);
}

void Feed::viewFeed(){
    if(recentPosts.isEmpty()) {
        cout << "Your feed is empty!\n";
        return;
    }
    
    cout << "\n===== YOUR FEED =====\n";

    StackNode<POST*>* current = recentPosts.getTop();
    int count = 1;
    
    while(current != nullptr) {
        POST* post = current->data;
        
        cout << "\n[Post " << count << "]\n";
        cout << "Author: " << post->getAuthor()->getUserName() << "\n";
        cout << "Content: " << post->getContent() << "\n";
        cout << "Likes: " << post->getLikes() << "\n";
        cout << "Comments: " << post->getTotalComments() << "\n";
        
        current = current->next;
        count++;
    }
    
    cout << "\n====================\n";
}

void Feed::refreshFeed(){
    recentPosts.clear();
    cout << "Feed refreshed!\n";
}

void Feed::removeOldPosts(){
    while(recentPosts.getSize() > maxFeedSize){
        recentPosts.pop();
    }
}

bool Feed::isEmpty() {
    return recentPosts.isEmpty();
}

StackNode<POST*>* Feed::getTopNode() {
    return recentPosts.getTop();
}