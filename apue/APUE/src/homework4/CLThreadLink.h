#ifndef _THREADLINK_H
#define _THREADLINK_H
#include<string>

typedef struct _Link CLThreadLink;
struct _Link {

    std::string ThreadName;
    CLThreadLink *Next;
};

#endif
