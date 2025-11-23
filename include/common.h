#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For time_t
#include <dirent.h> // Required for DIR and dirent structures
#include <sys/stat.h> // Required for stat and mkdir

#define MAX_USERS 10
#define MAX_USERNAME_LEN 49 // 49 + null terminator
#define MAX_PASSWORD_LEN 49 // 49 + null terminator
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 255 // 255 + null terminator

#define USER_DATA_DIR "data/users/"
#define CONVERSATION_DATA_DIR "data/conversations/"

// --- Structs ---
typedef struct {
    char username[MAX_USERNAME_LEN + 1];
    char password[MAX_PASSWORD_LEN + 1]; // Storing plain text for simplicity as requested
} User;

typedef struct {
    char sender[MAX_USERNAME_LEN + 1];
    char recipient[MAX_USERNAME_LEN + 1];
    char content[MAX_MESSAGE_LEN + 1];
    int read_status; // 0 for unread, 1 for read
    time_t timestamp;
} Message;

// --- Global Data (extern declarations) ---
extern User users[MAX_USERS];
extern int user_count;
extern Message messages[MAX_MESSAGES];
extern int message_count;

extern char current_username[MAX_USERNAME_LEN + 1];
extern int logged_in;

#endif // COMMON_H
