#ifndef _CLQUEUE_H
#define _CLQUEUE_H

template<class T>
class CLNode {

public:
    T item;
    class CLNode* next;
};

template<class T>
class CLQueue {

public:
    CLQueue();
    ~CLQueue();
    void QPush(T item);
    T QPop();
    bool isQEmpty();

private:
    class CLNode<T> *QHead;
    class CLNode<T> *QTail;
};


#endif
