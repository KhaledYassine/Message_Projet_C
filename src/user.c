#include "user.h"
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

// Simple hash function (placeholder, as per roadmap)
long simple_hash(const char* str) {
    long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Simple weight function (as per roadmap)
long calculate_weight(const char* str) {
    long weight = 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
        weight += str[i];
    }
    return weight;
}

int create_user(const char* username, const char* password) {
    printf("Attempting to create user: %s\n", username);

    // Create base 'user' directory
    MKDIR("user");

    // Create user-specific directory
    char user_dir[256];
    snprintf(user_dir, sizeof(user_dir), "user/%s", username);
    MKDIR(user_dir);

    // Create conversations directory
    char conversations_dir[512];
    snprintf(conversations_dir, sizeof(conversations_dir), "%s/conversations", user_dir);
    MKDIR(conversations_dir);

    // 2. Calculate and save passwd_hash
    char hash_path[512];
    snprintf(hash_path, sizeof(hash_path), "%s/passwd_hash", user_dir);
    FILE* f_hash = fopen(hash_path, "w");
    if (!f_hash) {
        perror("Failed to create passwd_hash file");
        return -1;
    }
    fprintf(f_hash, "%ld", simple_hash(password));
    fclose(f_hash);

    // 3. Calculate and save passwd_weight
    char weight_path[512];
    snprintf(weight_path, sizeof(weight_path), "%s/passwd_weight", user_dir);
    FILE* f_weight = fopen(weight_path, "w");
    if (!f_weight) {
        perror("Failed to create passwd_weight file");
        return -1;
    }
    fprintf(f_weight, "%ld", calculate_weight(password));
    fclose(f_weight);

    printf("User '%s' created successfully.\n", username);
    return 0;
}

int verify_user(const char* username, const char* password) {
    // TODO: Implement user verification based on roadmap
    printf("Verifying user %s (not implemented yet).\n", username);
    return 0;
}
