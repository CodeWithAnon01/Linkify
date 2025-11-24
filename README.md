# 🌐 Linkify - Social Network Simulator

**Data Structures Course Project**  
**National University of Computer and Emerging Sciences (FAST-NUCES), Karachi**

**Created By:** Minhal Raza  
**Student ID:** 24K-0554  
**Section:** BSCS-3J  
**Project Supervisor:** Mubashra Fayaaz

---

## 📖 Project Overview
Linkify is a console-based social network simulator designed to demonstrate the practical application of fundamental data structures. It mimics core functionalities of major social platforms, allowing users to create accounts, connect with friends, post updates, and interact via likes and comments.

## 🧱 Data Structures Used

| Data Structure | Usage in Project | Purpose & Benefit |
|----------------|------------------|-------------------|
| **Linked List** | Storing Users and Friends Lists | Allows for dynamic memory allocation. Adding or removing users/friends is efficient (O(1) for head insertions) without the need for resizing arrays. |
| **Stack** | User Feed | Implements **LIFO (Last-In, First-Out)** logic. This ensures that the most recent posts appear at the top of the user's feed, just like real social media apps. |
| **Queue** | Post Distribution & Comments | Implements **FIFO (First-In, First-Out)** logic. Used to process global posts fairly and to store comments in chronological order. |
| **Graph** | Network Connections | The entire network is a graph where **Users are Nodes** and **Friendships are Edges**. This structure enables advanced features like finding mutual friends and generating suggestions. |

## 🔐 Security & Hashing
Security is a priority in Linkify. We do **not** store passwords in plain text.

- **Technique**: Polynomial Rolling Hash with Salt.
- **Mechanism**: 
  1. A random **Salt** is generated for every user.
  2. The salt is appended to the password.
  3. The combined string is hashed using a custom polynomial rolling hash function.
- **Benefit**: Even if the database is compromised, attackers cannot easily reverse the hashes to get the original passwords. The unique salt prevents Rainbow Table attacks.

## ❓ Q&A

**Q: Is this project a mini prototype of how real-world social media apps like Facebook and Instagram work?**

**A:** **Yes.** At its core, Linkify replicates the fundamental backend logic of these giants. 
- **Graph Theory** is used by Facebook to map billions of users and their connections.
- **Stacks/Priority Queues** are used to generate news feeds (showing you what's new or relevant).
- **Hashing** is the industry standard for securing user credentials.
While real-world apps use distributed systems, complex databases, and AI algorithms, Linkify demonstrates the **foundational data structures** that make those massive systems possible.

---

## 🚀 Features
- **User System**: Sign up, Login, Profile Management.
- **Connections**: Add/Remove friends, View mutual friends.
- **Feed**: View posts from friends (Newest first).
- **Interactions**: Like posts, **View and Add Comments**.
- **Smart Suggestions**: Get friend recommendations based on mutual connections.

## 🛠 Compilation & Run
```bash
g++ -std=c++17 -o linkify main.cpp User/User.cpp Post/Post.cpp Feed/Feed.cpp Network/Network.cpp Authentication/userAccount.cpp Authentication/passwordManager.cpp Database/databaseManager.cpp

./linkify
<img width="2816" height="1536" alt="Linkify_logo" src="https://github.com/user-attachments/assets/cd1ae1b1-94e3-4286-a2be-ba1b13615a2d" />


```
