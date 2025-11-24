#include "Network.hpp"
#include "../Database/DatabaseManager.hpp"

void Network::setDatabase(DatabaseManager* database) {
    db = database;
}

bool Network::createNewUser(string uname, string bio){
    if(findUserbyName(uname)!=nullptr){
        return false;
    }
    User* user = new User;
    user->setUserName(uname);
    user->setBio(bio);
    user->setID(nextUserID);
    Feed* feed = new Feed;
    user->setFeed(feed);
    users.push_back(user);
    nextUserID++;
    return true;
}

User* Network::findUser(int userId) {
    Node<User*>* current = users.getHead();

    while (current != nullptr) {
        if (current->data->getID() == userId) {
            return current->data;
        }
        current = current->next;
    }

    return nullptr; 
}

User* Network::findUserbyName(string uname) {
    Node<User*>* current = users.getHead();

    while (current != nullptr) {
        if (current->data->getUserName() == uname) {
            return current->data;
        }
        current = current->next;
    }

    return nullptr; 
}

void Network::connectUsers(int userId1, int userId2) {
    User* user1 = Network::findUser(userId1);
    User* user2 = Network::findUser(userId2);

    if (user1 == nullptr || user2 == nullptr) {
        cout << "User not found!\n";
        return;
    }
    
    if (userId1 == userId2) {
        cout << "Cannot add yourself as friend!\n";
        return;
    }
    
    if(user1->isFriendWith(user2)){
        return;
    }

    user1->addFriend(user2);
    user2->addFriend(user1);

    
    if (db != nullptr) {
        db->saveFriendship(userId1, userId2);
    }

    cout << "Users connected successfully!\n";
}

bool Network::createPost(int uID, string text){
    User* user = findUser(uID);
    if (user == nullptr) {
        cout << "User not found!\n";
        return false;
    }
    
    if (text.empty()) {
        cout << "Write something to post!!\n";
        return false;
    }
   
    POST* post = new POST(nextPostID);
    post->setAuth(user);
    post->setContent(text);
    posts.enqueue(post);
    
    
    if (db != nullptr) {
        db->savePost(post);
    }
    
    nextPostID++;
    return true;
}

void Network::processGlobalPostsQueue(){
    while(!posts.isEmpty()){
        POST* posttemp = posts.dequeue();
        User* postAuth = posttemp->getAuthor();
        Feed* authorFeed = postAuth->getFeed();
        authorFeed->addPost(posttemp);
        
        LinkedList<User*> authorFriends = postAuth->getFriends();
        Node<User*>* current = authorFriends.getHead();
        while(current!=nullptr){
            Feed* friendsFeed = current->data->getFeed();
            friendsFeed->addPost(posttemp);
            current = current->next;
        }
    }
}

void Network::displayAllUsers() {
    cout << "\n===== ALL USERS =====\n";
    Node<User*>* current = users.getHead();
    int count = 0;
    while(current != nullptr) {
        User* user = current->data;
        cout << "User-" << count+1 << ": " << user->getUserName() 
             << " (ID: " << user->getID() << ")\n";
        cout << "   Bio: " << user->getBio() << "\n";
        cout << "   Friends: " << user->getFriends().getSize() << "\n";
        current = current->next;
        count++;
    }
    cout << "====================\n\n";
}

void Network::displayNetworkStats() {
    cout << "\n===== NETWORK STATISTICS =====\n";
    cout << "Total Users: " << users.getSize() << "\n";
    cout << "Total Posts Created: " << nextPostID - 1 << "\n";
    cout << "Posts in Queue: " << posts.getSize() << "\n";
    cout << "==============================\n\n";
}

Node<User*>* Network::getAllUsersHead() {
    return users.getHead();
}

int Network::countMutualFriends(User* user1, User* user2) {
    if (user1 == nullptr || user2 == nullptr) return 0;
    
    int count = 0;
    LinkedList<User*> user1Friends = user1->getFriends();
    Node<User*>* current = user1Friends.getHead();
    
    while (current != nullptr) {
        if (user2->isFriendWith(current->data)) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

LinkedList<User*> Network::getMutualFriends(int userId) {
    LinkedList<User*> candidates;
    User* user = findUser(userId);
    
    if (user == nullptr) return candidates;
    
    LinkedList<User*> userFriends = user->getFriends();
    Node<User*>* friendNode = userFriends.getHead();
    
    while (friendNode != nullptr) {
        User* friendUser = friendNode->data;
        LinkedList<User*> friendOfFriendList = friendUser->getFriends();
        Node<User*>* fofNode = friendOfFriendList.getHead();
        
        while (fofNode != nullptr) {
            User* potentialFriend = fofNode->data;
            
            if (potentialFriend->getID() != userId && 
                !user->isFriendWith(potentialFriend) &&
                !candidates.contains(potentialFriend)) {
                
                candidates.push_back(potentialFriend);
            }
            
            fofNode = fofNode->next;
        }
        
        friendNode = friendNode->next;
    }
    
    return candidates;
}

vector<Network::FriendSuggestion> Network::getSuggestionsSorted(int userId, int limit) {
    vector<FriendSuggestion> suggestions;
    User* user = findUser(userId);
    
    if (user == nullptr) return suggestions;
    
    LinkedList<User*> candidates = getMutualFriends(userId);
    Node<User*>* current = candidates.getHead();
    
    while (current != nullptr) {
        int mutualCount = countMutualFriends(user, current->data);
        if (mutualCount > 0) {
            suggestions.push_back(FriendSuggestion(current->data, mutualCount));
        }
        current = current->next;
    }
    
    sort(suggestions.begin(), suggestions.end(), 
         [](const FriendSuggestion& a, const FriendSuggestion& b) {
        return a.mutualCount > b.mutualCount;
    });
    
    if (suggestions.size() > (size_t)limit) {
        suggestions.erase(suggestions.begin() + limit, suggestions.end());
    }
    
    return suggestions;
}

User* Network::getCurrentUser() {
    return currentLoggedInUser;
}

void Network::setCurrentUser(User* user) {
    currentLoggedInUser = user;
}

int Network::getNextUserID() {
    return nextUserID;
}

int Network::getNextPostID() {
    return nextPostID;
}

void Network::setNextUserID(int id) {
    nextUserID = id;
}

void Network::setNextPostID(int id) {
    nextPostID = id;
}

bool Network::createAccount(string username, string password) {
    if (findUserbyName(username) != nullptr) {
        cout << "Username already taken!\n";
        return false;
    }
    
    if (!createNewUser(username, "")) {
        return false;
    }
    
    return true;
}

bool Network::loginUser(string username, string password) {
    User* user = findUserbyName(username);
    if (user == nullptr) {
        return false;
    }
    
    setCurrentUser(user);
    return true;
}