#include "DatabaseManager.hpp"
#include "../Authentication/userAccount.hpp"
#include "../Post/Post.hpp"
#include "../User/User.hpp"

string DatabaseManager::getFullPath(string filename) {
    return DATABASE_PATH + filename;
}

bool DatabaseManager::fileExists(string filepath) {
    ifstream f(filepath);
    return f.good();
}

bool DatabaseManager::createFileIfNotExists(string filepath) {
    if (!fileExists(filepath)) {
        ofstream f(filepath);
        if (!f.is_open()) return false;
        f.close();
    }
    return true;
}

DatabaseManager::DatabaseManager() {}

bool DatabaseManager::initializeDatabase() {
    if (!filesystem::exists(DATABASE_PATH)) {
        if (!filesystem::create_directory(DATABASE_PATH))
            return false;
    }
    

    bool users = createFileIfNotExists(getFullPath(USERS_FILE));
    bool posts = createFileIfNotExists(getFullPath(POSTS_FILE));
    bool friends = createFileIfNotExists(getFullPath(FRIENDSHIPS_FILE));
    
    return users && posts && friends;
}


bool DatabaseManager::saveUserAccount(UserAccount* account) {
    if (userExists(account->getUsername()))
        return false;

    ofstream file(getFullPath(USERS_FILE), ios::app);
    if (!file.is_open()) return false;

    file << account->toFileFormat() << "\n";
    file.close();
    return true;
}

UserAccount* DatabaseManager::loadUserAccount(string username) {
    ifstream file(getFullPath(USERS_FILE));
    if (!file.is_open()) return nullptr;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name;
        getline(ss, name, '|');
        if (name == username) {
            UserAccount* acc = new UserAccount(0, "");
            acc->fromFileFormat(line);
            file.close();
            return acc;
        }
    }
    file.close();
    return nullptr;
}

bool DatabaseManager::userExists(string username) {
    ifstream file(getFullPath(USERS_FILE));
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        if (line.rfind(username + "|", 0) == 0) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool DatabaseManager::deleteUserAccount(string username) {
    ifstream file(getFullPath(USERS_FILE));
    if (!file.is_open()) return false;

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        if (line.rfind(username + "|", 0) != 0)
            lines.push_back(line);
    }
    file.close();

    ofstream out(getFullPath(USERS_FILE), ios::trunc);
    if (!out.is_open()) return false;

    for (string& l : lines)
        out << l << "\n";

    out.close();
    return true;
}

vector<UserAccount*> DatabaseManager::loadAllUsers() {
    vector<UserAccount*> users;
    ifstream file(getFullPath(USERS_FILE));
    if (!file.is_open()) return users;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        UserAccount* acc = new UserAccount(0, "");
        acc->fromFileFormat(line);
        users.push_back(acc);
    }

    file.close();
    return users;
}


bool DatabaseManager::savePost(POST* post) {
    if (post == nullptr || post->getAuthor() == nullptr) return false;
    
    ofstream file(getFullPath(POSTS_FILE), ios::app);
    if (!file.is_open()) return false;
    

    string content = post->getContent();
    size_t pos = 0;
    while ((pos = content.find('|', pos)) != string::npos) {
        content.replace(pos, 1, "[PIPE]");
        pos += 6;
    }
    pos = 0;
    while ((pos = content.find('\n', pos)) != string::npos) {
        content.replace(pos, 1, "[NEWLINE]");
        pos += 9;
    }
    
    file << post->getPID() << "|"
         << post->getAuthor()->getID() << "|"
         << content << "|"
         << post->getLikes() << "\n";
    
    file.close();
    return true;
}

vector<POST*> DatabaseManager::loadAllPosts() {
    vector<POST*> posts;
    ifstream file(getFullPath(POSTS_FILE));
    if (!file.is_open()) return posts;
    
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
           
            int likes = stoi(likesStr);
        
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
            
            POST* post = new POST(postID);
            post->setContent(content);
            
            for (int i = 0; i < likes; i++) {
                post->addLike();
            }
            
            posts.push_back(post);
        } catch (const exception& e) {
            continue;
        }
    }
    
    file.close();
    return posts;
}

bool DatabaseManager::deletePost(int postID) {
    ifstream file(getFullPath(POSTS_FILE));
    if (!file.is_open()) return false;
    
    vector<string> lines;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string pidStr;
        getline(ss, pidStr, '|');
        
        try {
            if (stoi(pidStr) != postID) {
                lines.push_back(line);
            }
        } catch (...) {
            lines.push_back(line);
        }
    }
    file.close();
    
    ofstream out(getFullPath(POSTS_FILE), ios::trunc);
    if (!out.is_open()) return false;
    
    for (string& l : lines)
        out << l << "\n";
    
    out.close();
    return true;
}



bool DatabaseManager::saveFriendship(int userId1, int userId2) {
   
    if (userId1 > userId2) swap(userId1, userId2);
    
    vector<pair<int, int>> friendships = loadAllFriendships();
    for (auto& f : friendships) {
        if (f.first == userId1 && f.second == userId2)
            return false; 
    }
    
    ofstream file(getFullPath(FRIENDSHIPS_FILE), ios::app);
    if (!file.is_open()) return false;
    
    file << userId1 << "|" << userId2 << "\n";
    file.close();
    return true;
}

vector<pair<int, int>> DatabaseManager::loadAllFriendships() {
    vector<pair<int, int>> friendships;
    ifstream file(getFullPath(FRIENDSHIPS_FILE));
    if (!file.is_open()) return friendships;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id1Str, id2Str;
        
        if (!getline(ss, id1Str, '|')) continue;
        if (!getline(ss, id2Str, '|')) continue;
        
        try {
            int id1 = stoi(id1Str);
            int id2 = stoi(id2Str);
            friendships.push_back({id1, id2});
        } catch (...) {
            continue;
        }
    }
    
    file.close();
    return friendships;
}

bool DatabaseManager::deleteFriendship(int userId1, int userId2) {
    if (userId1 > userId2) swap(userId1, userId2);
    
    ifstream file(getFullPath(FRIENDSHIPS_FILE));
    if (!file.is_open()) return false;
    
    vector<string> lines;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string id1Str, id2Str;
        getline(ss, id1Str, '|');
        getline(ss, id2Str, '|');
        
        try {
            int id1 = stoi(id1Str);
            int id2 = stoi(id2Str);
            
            if (!(id1 == userId1 && id2 == userId2)) {
                lines.push_back(line);
            }
        } catch (...) {
            lines.push_back(line);
        }
    }
    file.close();
    
    ofstream out(getFullPath(FRIENDSHIPS_FILE), ios::trunc);
    if (!out.is_open()) return false;
    
    for (string& l : lines)
        out << l << "\n";
    
    out.close();
    return true;
}