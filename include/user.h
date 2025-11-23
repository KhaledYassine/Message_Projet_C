#ifndef USER_H
#define USER_H

#include "common.h"

// Function declarations for user management
int find_user_index(const char* username);
int register_user(const char* username, const char* password);
int login_user(const char* username, const char* password);

#endif // USER_H
