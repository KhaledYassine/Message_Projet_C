#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

typedef struct {
    char user[32];
    time_t timestamp;
    int length;
    char content[1024];
} Message;

#endif // MESSAGE_H
