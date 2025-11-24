#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>
using namespace std;

#include "Authentication/userAccount.hpp"
#include "Authentication/passwordManager.hpp"
#include "Database/databaseManager.hpp"
#include "Network/Network.hpp"
#include "User/User.hpp"
#include "Post/Post.hpp"
#include "Feed/Feed.hpp"

Network network;
DatabaseManager db;
User* currentLoggedInUser = nullptr;

void showWelcomeScreen();
void showMainMenu();
void handleSignUp();
void handleLogin();
void handleViewProfile();
void handleAddFriend();
void handleCreatePost();
void handleViewFeed();
void handleViewAllUsers();
void handleGetSuggestions();
void handleViewNetworkStats();
void handleLogout();
void loadExistingUsers();
void loadExistingFriendships();
void loadExistingPosts();
void clearInputBuffer();
void pauseScreen();

int main() {
    cout << "\n+==================================+\n";
    cout << "|    WELCOME TO LINKIFY            |\n";
    cout << "|   Social Network Simulator       |\n";
    cout << "+==================================+\n";
    
    if (!db.initializeDatabase()) {
        cout << "\n[X] Error: Could not initialize database!\n";
        return 1;
    }
    
    cout << "\n[OK] Database initialized.\n";
    
    network.setDatabase(&db);
    
    loadExistingUsers();
    loadExistingFriendships();
    loadExistingPosts();
    
    showWelcomeScreen();
    
    cout << "\n\nThank you for using Linkify! Goodbye!\n";
    return 0;
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    clearInputBuffer();
}

void loadExistingUsers() {
    vector<UserAccount*> accounts = db.loadAllUsers();
    
    if (accounts.empty()) {
        cout << "[OK] No existing users found. Starting fresh.\n";
        return;
    }
    
    int maxUserID = 0;
    
    for (UserAccount* account : accounts) {
        string username = account->getUsername();
        int userID = account->getUID();
        
        if (userID > maxUserID) {
            maxUserID = userID;
        }
        
        if (network.findUserbyName(username) == nullptr) {
            network.createNewUser(username, "");
            User* user = network.findUserbyName(username);
            if (user != nullptr) {
                user->setID(userID);
            }
        }
    }
    
    network.setNextUserID(maxUserID + 1);
    cout << "[OK] Loaded " << accounts.size() << " existing users.\n";
}

void loadExistingFriendships() {
    vector<pair<int, int>> friendships = db.loadAllFriendships();
    
    if (friendships.empty()) {
        cout << "[OK] No existing friendships found.\n";
        return;
    }
    
    int loaded = 0;
    for (auto& friendship : friendships) {
        User* user1 = network.findUser(friendship.first);
        User* user2 = network.findUser(friendship.second);
        
        if (user1 != nullptr && user2 != nullptr && !user1->isFriendWith(user2)) {
            user1->addFriend(user2);
            user2->addFriend(user1);
            loaded++;
        }
    }
    
    cout << "[OK] Loaded " << loaded << " friendships.\n";
}

void loadExistingPosts() {
    ifstream file(db.getFullPath("posts.txt"));
    if (!file.is_open()) {
        cout << "[OK] No existing posts found.\n";
        return;
    }
    
    int maxPostID = 0;
    int loaded = 0;
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string postIDStr, authorIDStr, content, likesStr;
        
        if (!getline(ss, postIDStr, '|')) continue;
        if (!getline(ss, authorIDStr, '|')) continue;
        if (!getline(ss, content, '|')) continue;
        if (!getline(ss, likesStr, '|')) continue;
        
        try {
            int postID = stoi(postIDStr);
            int authorID = stoi(authorIDStr);
            int likes = stoi(likesStr);
            
            if (postID > maxPostID) maxPostID = postID;
            
            size_t pos = 0;
            while ((pos = content.find("[PIPE]", pos)) != string::npos) {
                content.replace(pos, 6, "|");
                pos += 1;
            }
            pos = 0;
            while ((pos = content.find("[NEWLINE]", pos)) != string::npos) {
                content.replace(pos, 9, "\n");
                pos += 1;
            }
            
            User* author = network.findUser(authorID);
            if (author != nullptr) {
                POST* post = new POST(postID);
                post->setAuth(author);
                post->setContent(content);
                for (int i = 0; i < likes; i++) {
                    post->addLike();
                }
                
                author->getFeed()->addPost(post);
                
                Node<User*>* friendNode = author->getFriends().getHead();
                while (friendNode != nullptr) {
                    friendNode->data->getFeed()->addPost(post);
                    friendNode = friendNode->next;
                }
                
                loaded++;
            }
        } catch (...) {
            continue;
        }
    }
    
    file.close();
    network.setNextPostID(maxPostID + 1);
    cout << "[OK] Loaded " << loaded << " posts and distributed to feeds.\n";
}

void showWelcomeScreen() {
    while (true) {
        cout << "\n+==================================+\n";
        cout << "|   1. Sign Up (New Account)       |\n";
        cout << "|   2. Login (Existing Account)    |\n";
        cout << "|   3. Exit                        |\n";
        cout << "+==================================+\n";
        cout << "Choose option: ";
        
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "[X] Invalid input! Please enter a number.\n";
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: handleSignUp(); break;
            case 2: handleLogin(); break;
            case 3: return;
            default: cout << "[X] Invalid choice! Please try again.\n";
        }
    }
}

void handleSignUp() {
    cout << "\n+==================================+\n";
    cout << "|         SIGN UP                  |\n";
    cout << "+==================================+\n";
    
    string username, password, bio;
    
    cout << "\nEnter username: ";
    getline(cin, username);
    
    if (username.empty()) {
        cout << "[X] Username cannot be empty!\n";
        return;
    }
    
    if (db.userExists(username)) {
        cout << "[X] Username already taken!\n";
        return;
    }
    
    cout << "Enter password (min 6 chars, 1 letter, 1 number): ";
    getline(cin, password);
    
    cout << "Enter bio (optional): ";
    getline(cin, bio);
    
    int newUserID = network.getNextUserID();
    UserAccount* account = new UserAccount(newUserID, username);
    
    if (!account->SignUp(password)) {
        cout << "[X] Account creation failed!\n";
        delete account;
        return;
    }
    
    if (!db.saveUserAccount(account)) {
        cout << "[X] Error saving account to database!\n";
        delete account;
        return;
    }
    
    if (!network.createNewUser(username, bio)) {
        cout << "[X] Error creating user profile!\n";
        return;
    }
    
    cout << "\n[OK] Account created successfully!\n";
    cout << "Your User ID: " << newUserID << "\n";
    cout << "You can now login.\n";
}

void handleLogin() {
    cout << "\n+==================================+\n";
    cout << "|         LOGIN                    |\n";
    cout << "+==================================+\n";
    
    string username, password;
    
    cout << "\nEnter username: ";
    getline(cin, username);
    
    if (username.empty()) {
        cout << "[X] Username cannot be empty!\n";
        return;
    }
    
    if (!db.userExists(username)) {
        cout << "[X] User not found!\n";
        return;
    }
    
    cout << "Enter password: ";
    getline(cin, password);
    
    UserAccount* account = db.loadUserAccount(username);
    if (account == nullptr) {
        cout << "[X] Error loading account!\n";
        return;
    }
    
    if (!account->Login(password)) {
        cout << "[X] Login failed!\n";
        delete account;
        return;
    }
    
    currentLoggedInUser = network.findUserbyName(username);
    if (currentLoggedInUser == nullptr) {
        cout << "[X] Error: User profile not found!\n";
        delete account;
        return;
    }
    
    cout << "\n[OK] Login successful!\n";
    cout << "Welcome, " << username << "!\n";
    
    showMainMenu();
    currentLoggedInUser = nullptr;
}

void showMainMenu() {
    while (currentLoggedInUser != nullptr) {
        cout << "\n+==================================+\n";
        cout << "|         MAIN MENU                |\n";
        cout << "|   1. View My Profile             |\n";
        cout << "|   2. Add Friend                  |\n";
        cout << "|   3. Create Post                 |\n";
        cout << "|   4. View My Feed                |\n";
        cout << "|   5. View All Users              |\n";
        cout << "|   6. Get Friend Suggestions      |\n";
        cout << "|   7. View Network Stats          |\n";
        cout << "|   8. Logout                      |\n";
        cout << "+==================================+\n";
        cout << "Choose option: ";
        
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "[X] Invalid input!\n";
            continue;
        }
        clearInputBuffer();
        
        switch(choice) {
            case 1: handleViewProfile(); break;
            case 2: handleAddFriend(); break;
            case 3: handleCreatePost(); break;
            case 4: handleViewFeed(); break;
            case 5: handleViewAllUsers(); break;
            case 6: handleGetSuggestions(); break;
            case 7: handleViewNetworkStats(); break;
            case 8: handleLogout(); return;
            default: cout << "[X] Invalid choice!\n";
        }
    }
}

void handleViewProfile() {
    if (currentLoggedInUser == nullptr) {
        cout << "[X] Error: User not found!\n";
        return;
    }
    
    cout << "\n+==================================+\n";
    cout << "|       MY PROFILE                 |\n";
    cout << "+==================================+\n";
    
    cout << "\nUsername: " << currentLoggedInUser->getUserName() << "\n";
    cout << "User ID: " << currentLoggedInUser->getID() << "\n";
    
    string bio = currentLoggedInUser->getBio();
    cout << "Bio: " << (bio.empty() ? "(No bio set)" : bio) << "\n";
    
    cout << "Total Friends: " << currentLoggedInUser->getFriends().getSize() << "\n";
    
    if (currentLoggedInUser->getFriends().getSize() > 0) {
        cout << "\nYour Friends:\n";
        Node<User*>* current = currentLoggedInUser->getFriends().getHead();
        int count = 1;
        while (current != nullptr) {
            cout << "  " << count << ". " << current->data->getUserName() << "\n";
            current = current->next;
            count++;
        }
    } else {
        cout << "\nYou don't have any friends yet!\n";
    }
    
    pauseScreen();
}

void handleAddFriend() {
    if (currentLoggedInUser == nullptr) return;
    
    cout << "\n+==================================+\n";
    cout << "|       ADD FRIEND                 |\n";
    cout << "+==================================+\n";
    
    cout << "\nEnter friend's username: ";
    string friendName;
    getline(cin, friendName);
    
    if (friendName.empty()) {
        cout << "[X] Username cannot be empty!\n";
        return;
    }
    
    User* friendUser = network.findUserbyName(friendName);
    if (friendUser == nullptr) {
        cout << "[X] User not found!\n";
        return;
    }
    
    if (currentLoggedInUser->getID() == friendUser->getID()) {
        cout << "[X] Cannot add yourself as friend!\n";
        return;
    }
    
    if (currentLoggedInUser->isFriendWith(friendUser)) {
        cout << "[X] Already friends!\n";
        return;
    }
    
    network.connectUsers(currentLoggedInUser->getID(), friendUser->getID());
    cout << "\n[OK] " << friendName << " added as friend!\n";
}

void handleCreatePost() {
    if (currentLoggedInUser == nullptr) return;
    
    cout << "\n+==================================+\n";
    cout << "|       CREATE POST                |\n";
    cout << "+==================================+\n";
    
    cout << "\nWhat's on your mind?\n";
    string content;
    getline(cin, content);
    
    if (content.empty()) {
        cout << "[X] Post cannot be empty!\n";
        return;
    }
    
    if (network.createPost(currentLoggedInUser->getID(), content)) {
        network.processGlobalPostsQueue();
        cout << "\n[OK] Post created and distributed!\n";
    } else {
        cout << "[X] Failed to create post!\n";
    }
}

void handleViewFeed() {
    if (currentLoggedInUser == nullptr) return;
    
    Feed* feed = currentLoggedInUser->getFeed();
    if (feed == nullptr) {
        cout << "[X] Error: Feed not found!\n";
        return;
    }
    
    if (feed->isEmpty()) {
        cout << "\nYour feed is empty!\n";
        pauseScreen();
        return;
    }
    
    cout << "\n===== YOUR FEED =====\n";
    
    StackNode<POST*>* current = feed->getTopNode();
    vector<POST*> posts;
    
    int count = 1;
    while (current != nullptr) {
        posts.push_back(current->data);
        POST* post = current->data;
        
        cout << "\n[Post " << count << "] (ID: " << post->getPID() << ")\n";
        cout << "Author: " << post->getAuthor()->getUserName() << "\n";
        cout << "Content: " << post->getContent() << "\n";
        cout << "Likes: " << post->getLikes() << "\n";
        cout << "Comments: " << post->getTotalComments() << "\n";
        
        current = current->next;
        count++;
    }
    
    cout << "\n====================\n";
    cout << "\n1. Like a post\n";
    cout << "2. Comment on a post\n";
    cout << "3. View comments on a post\n";
    cout << "4. Back to menu\n";
    cout << "Choose option: ";
    
    int choice;
    if (!(cin >> choice)) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (choice == 1) {
        cout << "Enter post number to like: ";
        int postNum;
        if (cin >> postNum && postNum > 0 && postNum <= (int)posts.size()) {
            posts[postNum-1]->addLike();
            cout << "[OK] Post liked!\n";
        }
        clearInputBuffer();
    } else if (choice == 2) {
        cout << "Enter post number to comment on: ";
        int postNum;
        if (cin >> postNum && postNum > 0 && postNum <= (int)posts.size()) {
            clearInputBuffer();
            cout << "Enter your comment: ";
            string comment;
            getline(cin, comment);
            if (!comment.empty()) {
                posts[postNum-1]->addComment(comment);
                cout << "[OK] Comment added!\n";
            }
        } else {
            clearInputBuffer();
        }
    } else if (choice == 3) {
        cout << "Enter post number to view comments: ";
        int postNum;
        if (cin >> postNum && postNum > 0 && postNum <= (int)posts.size()) {
            clearInputBuffer();
            posts[postNum-1]->viewComments();
        } else {
            clearInputBuffer();
            cout << "[X] Invalid post number!\n";
        }
    }
}

void handleViewAllUsers() {
    cout << "\n===== ALL USERS =====\n";
    Node<User*>* current = network.getAllUsersHead();
    int count = 0;
    
    while(current != nullptr) {
        User* user = current->data;
        cout << "\nUser-" << count+1 << ": " << user->getUserName() 
             << " (ID: " << user->getID() << ")\n";
        
        string bio = user->getBio();
        cout << "   Bio: " << (bio.empty() ? "(No bio set)" : bio) << "\n";
        cout << "   Friends: " << user->getFriends().getSize() << "\n";
        
        current = current->next;
        count++;
    }
    
    cout << "\n====================\n";
    pauseScreen();
}

void handleGetSuggestions() {
    if (currentLoggedInUser == nullptr) return;
    
    cout << "\n+==================================+\n";
    cout << "|    FRIEND SUGGESTIONS            |\n";
    cout << "+==================================+\n";
    
    vector<Network::FriendSuggestion> suggestions = 
        network.getSuggestionsSorted(currentLoggedInUser->getID(), 5);
    
    if (suggestions.size() == 0) {
        cout << "\n[X] No suggestions available!\n";
        return;
    }
    
    cout << "\nSuggested friends (ranked by mutual connections):\n\n";
    
    for (size_t i = 0; i < suggestions.size(); i++) {
        cout << (i+1) << ". " << suggestions[i].suggestedUser->getUserName();
        cout << " (" << suggestions[i].mutualCount << " mutual friend";
        cout << (suggestions[i].mutualCount > 1 ? "s" : "") << ")\n";
    }
    
    cout << "\nWould you like to add someone? (Enter number or 0 to skip): ";
    int choice;
    if (!(cin >> choice)) {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (choice > 0 && choice <= (int)suggestions.size()) {
        User* suggestedUser = suggestions[choice-1].suggestedUser;
        network.connectUsers(currentLoggedInUser->getID(), suggestedUser->getID());
        cout << "\n[OK] Added " << suggestedUser->getUserName() << " as friend!\n";
    }
}

void handleViewNetworkStats() {
    network.displayNetworkStats();
    pauseScreen();
}

void handleLogout() {
    cout << "\nLogging out...\n";
    cout << "[OK] Goodbye!\n";
    currentLoggedInUser = nullptr;
}