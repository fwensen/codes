#include<iostream>
#include"CLQueue.h"
using namespace std;

template<typename T>
CLQueue<T>::CLQueue() {
    QHead = NULL;
    QTail = NULL;
}


template<typename T>
CLQueue<T>::~CLQueue() {
    
    while(QHead) {
        class CLNode<T> *tmp = QHead;
        QHead = QHead->next;
        delete tmp;
    }
}
  
template<typename T>
void CLQueue<T>::QPush(T item) {
    
    if (isQEmpty()) {
        QHead = new class CLNode<T>;
        QHead->item = item;
        QTail = QHead;
        return;
    }
    class CLNode<T> *nNode = new class CLNode<T>;
    nNode->item = item;
    nNode->next = NULL;
    QTail->next = nNode;
    QTail = nNode;

}

template<typename T> 
T CLQueue<T>::QPop() {
    
    if (isQEmpty()) {
        return NULL;
    }

    T item = QHead->item;
    class CLNode<T> *tmp = QHead;
    QHead = QHead->next;
    delete tmp;
    return item;
}


template<typename T>
bool CLQueue<T>::isQEmpty() {
    
    return QHead ? false : true;
}
