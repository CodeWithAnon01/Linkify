#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <typename T>
class StackNode {
public:
    T data;
    StackNode* next;
    
    StackNode(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Stack {
private:
    StackNode<T>* top;
    int size;
    
public:
    Stack() : top(nullptr), size(0) {}
    
    ~Stack() {
        while (top != nullptr) {
            StackNode<T>* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }
    
    void push(T val) {
        StackNode<T>* newNode = new StackNode<T>(val);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    T pop() {
        if (top == nullptr) {
            throw runtime_error("Stack underflow! Cannot pop from empty stack");
        }
        
        StackNode<T>* temp = top;
        T data = temp->data;
        top = top->next;
        delete temp;
        size--;
        
        return data;
    }
    
    T peek() {
        if (top == nullptr) {
            throw runtime_error("Stack is empty! Cannot peek");
        }
        return top->data;
    }
    

    bool isEmpty() const {
        return size == 0;
    }
    

    int getSize() const {
        return size;
    }
    
  
    void clear() {
        while (top != nullptr) {
            StackNode<T>* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }
    
   
    void display() {
        if (top == nullptr) {
            cout << "Stack is empty!\n";
            return;
        }
        
        StackNode<T>* current = top;
        cout << "Stack (Top to Bottom): ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << "\n";
    }
    
    
    StackNode<T>* getTop() {
        return top;
    }
};

#endif