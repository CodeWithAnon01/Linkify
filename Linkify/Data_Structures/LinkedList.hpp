#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node* next;
    
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;
    
public:
    LinkedList() : head(nullptr), size(0) {}
    
    ~LinkedList() {
        clear();
    }

    
    LinkedList(const LinkedList& other) : head(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

   
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
  
    void push_back(T val) {
        Node<T>* newNode = new Node<T>(val);
        
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }
    
 
    void push_front(T val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
        size++;
    }
    
    void pop_back() {
        if (head == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            size--;
            return;
        }
        
        Node<T>* current = head;
        while (current->next->next != nullptr) {
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
        size--;
    }
    
    void pop_front() {
        if (head == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
    }
    

    bool remove(T val) {
        if (head == nullptr) return false;
        
        
        if (head->data == val) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }
        

        Node<T>* current = head;
        while (current->next != nullptr) {
            if (current->next->data == val) {
                Node<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                size--;
                return true;
            }
            current = current->next;
        }
        
        return false;
    }
    
   
    bool contains(T val) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == val) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    
    T get(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of bounds");
        }
        
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    
   
    int getSize() const {
        return size;
    }
    
   
    bool isEmpty() const {
        return size == 0;
    }
    
   
    void display() {
        if (head == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        
        Node<T>* current = head;
        cout << "List: ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << "\n";
    }
    
   
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
    
    
    Node<T>* getHead() {
        return head;
    }
    
    
    Node<T>* begin() {
        return head;
    }
    
    Node<T>* end() {
        return nullptr;
    }
};

#endif