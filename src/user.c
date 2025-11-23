#include "user.h"
#include "common.h" // For access to global data and MAX_ constants
#include "storage.h" // For save_user_to_file
#include "visuals.h" // For color codes

// Note: Global variable definitions have been moved to globals.c


// Finds a user by username and returns their index. Returns -1 if not found.
int find_user_index(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Registers a new user. Returns 0 on success, -1 on failure.
int register_user(const char* username, const char* password) {
    if (user_count >= MAX_USERS) {
        printf(COLOR_RED "Error: Maximum number of users reached.\n" COLOR_RESET);
        return -1;
    }
    if (find_user_index(username) != -1) {
        printf(COLOR_RED "Error: Username '%s' already exists.\n" COLOR_RESET, username);
        return -1;
    }
    if (strlen(username) > MAX_USERNAME_LEN || strlen(password) > MAX_PASSWORD_LEN) {
        printf(COLOR_RED "Error: Username or password too long (max %d characters).\n" COLOR_RESET, MAX_USERNAME_LEN);
        return -1;
    }

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    
    // Save user to file immediately after adding to memory
    save_user_to_file(&users[user_count]);

    user_count++;
    printf(COLOR_GREEN "User '%s' registered successfully.\n" COLOR_RESET, username);
    return 0;
}

// Attempts to log in a user. Returns 0 on success, -1 on failure.
int login_user(const char* username, const char* password) {
    int index = find_user_index(username);
    if (index == -1) {
        printf(COLOR_RED "Error: User '%s' not found.\n" COLOR_RESET, username);
        return -1;
    }
    if (strcmp(users[index].password, password) == 0) {
        strcpy(current_username, username);
        logged_in = 1;
        printf(COLOR_GREEN "User '%s' logged in successfully.\n" COLOR_RESET, username);
        return 0;
    } else {
        printf(COLOR_RED "Error: Incorrect password for user '%s'.\n" COLOR_RESET, username);
        return -1;
    }
}