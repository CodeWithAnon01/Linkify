#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

template <typename T>
class QueueNode {
public:
    T data;
    QueueNode* next;
    
    QueueNode(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;
    int size;
    
public:
   
    Queue() : front(nullptr), rear(nullptr), size(0) {}
    
    ~Queue() {
        while (front != nullptr) {
            QueueNode<T>* temp = front;
            front = front->next;
            delete temp;
        }
        front = nullptr;
        rear = nullptr;
        size = 0;
    }
    
    void enqueue(T val) {
        QueueNode<T>* newNode = new QueueNode<T>(val);
        
        if (rear == nullptr) {
        
            front = newNode;
            rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }
    
    T dequeue() {
        if (front == nullptr) {
            throw runtime_error("Queue underflow! Cannot dequeue from empty queue");
        }
        
        QueueNode<T>* temp = front;
        T data = temp->data;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        
        return data;
    }
    
    T peek() {
        if (front == nullptr) {
            throw runtime_error("Queue is empty! Cannot peek");
        }
        return front->data;
    }
    
    T peekRear() {
        if (rear == nullptr) {
            throw runtime_error("Queue is empty! Cannot peek rear");
        }
        return rear->data;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    int getSize() const {
        return size;
    }
    
    void clear() {
        while (front != nullptr) {
            QueueNode<T>* temp = front;
            front = front->next;
            delete temp;
        }
        front = nullptr;
        rear = nullptr;
        size = 0;
    }
    

    void display() {
        if (front == nullptr) {
            cout << "Queue is empty!\n";
            return;
        }
        
        QueueNode<T>* current = front;
        cout << "Queue (Front to Rear): ";
        while (current != nullptr) {
            cout << current->data;
            if (current->next != nullptr) {
                cout << " -> ";
            }
            current = current->next;
        }
        cout << "\n";
    }
    
    
    QueueNode<T>* getFront() {
        return front;
    }
    
   
    QueueNode<T>* getRear() {
        return rear;
    }
};

#endif