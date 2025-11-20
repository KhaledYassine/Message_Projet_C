#ifndef USER_H
#define USER_H

typedef struct {
    char username[32];
    char passwd_hash[256]; // Sized for SHA256 later
    long passwd_weight;
} User;

// Function declarations
int create_user(const char* username, const char* password);
int verify_user(const char* username, const char* password);

#endif // USER_H
