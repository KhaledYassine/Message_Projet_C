#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <time.h> // Required for time_t

#define MAX_PARTICIPANTS 10

typedef struct {
    char cnv_id[64];
    char name[128];
    char participants[MAX_PARTICIPANTS][32];
    int num_participants;
    time_t last_timestamp;
} Conversation;

#endif // CONVERSATION_H
