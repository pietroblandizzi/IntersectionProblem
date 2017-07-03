#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <iostream>

using namespace std;


/**
Generic thread-safe queue
to fix with better error handling
with exceptions
**/

template<class T>
class TSQueue
{
    public:
        TSQueue();
        bool empty();
        void push_back(T data);
        void push_front(T data);
        T pop();


    private:
        struct Node{
            T data;
            Node *next;
        };

        Node *frontPtr;
        Node *backPtr;
        int count;
        std::mutex qm_;

};


template<class T>
TSQueue<T>::TSQueue(): frontPtr(NULL), backPtr(NULL), count(0)
{
}

template<class T>
bool TSQueue<T>::empty(){
    std::unique_lock<std::mutex> lck(qm_);
    return(count == 0);
}

template<class T>
void TSQueue<T>::push_back(T data){
    std::unique_lock<std::mutex> lck(qm_);

    Node *newOne = new Node;
    newOne->data = std::move(data);
    newOne->next = NULL;
    if(count == 0){
        frontPtr = newOne;
    }
        else{
            backPtr->next = newOne;
        }
        backPtr = newOne;
        count++;
}

template<class T>
void TSQueue<T>::push_front(T data){
    std::unique_lock<std::mutex> lck(qm_);

    Node *newOne = new Node;
    newOne->data = std::move(data);
    newOne->next = NULL;
    if(count == 0){
        frontPtr = newOne;
        backPtr = newOne;
    }
    else{
            newOne->next = frontPtr;
            frontPtr = newOne;
        }

        count++;
}

template<class T>
T TSQueue<T>::pop(){

if (count == 0 )
    return nullptr;

    std::unique_lock<std::mutex> lck(qm_);
    Node *temp = frontPtr;
    if(frontPtr == backPtr){
        frontPtr = NULL;
        backPtr = NULL;
    }
    else{
        frontPtr = frontPtr->next;
    }
    if (count > 0) {
        count--;
    }

    if (temp == nullptr)
        return nullptr;

    return std::move(temp->data);

}



#endif // TSQUEUE_H
