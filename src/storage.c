#include "storage.h"
#include "common.h"
#include "user.h" // For access to global data definitions
#include <dirent.h> // For directory operations
#include <sys/stat.h> // For stat
#include <stdio.h> // For remove
#include <string.h> // For strtok
#include "visuals.h" // For color codes

#ifdef _WIN32
#include <direct.h> // For _mkdir on Windows
#endif

// Helper function to create directories if they don't exist
void ensure_dir_exists(const char* path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
#ifdef _WIN32
        _mkdir(path); 
#else
        mkdir(path, 0700); // POSIX systems use mode
#endif
    }
}

// Helper function to construct user file path
void get_user_filepath(const char* username, char* filepath) {
    sprintf(filepath, "%s%s.user", USER_DATA_DIR, username);
}

// Helper function to construct conversation file path
void get_conversation_filepath(const char* user1, const char* user2, char* filepath) {
    // Ensure consistent filename by sorting usernames
    if (strcmp(user1, user2) < 0) {
        sprintf(filepath, "%s%s_%s.conv", CONVERSATION_DATA_DIR, user1, user2);
    } else {
        sprintf(filepath, "%s%s_%s.conv", CONVERSATION_DATA_DIR, user2, user1);
    }
}

// --- User Storage Functions ---

void load_users_from_files() {
    ensure_dir_exists(USER_DATA_DIR); // Ensure directory exists before opening
    DIR *d;
    struct dirent *dir;
    char filepath[256];
    FILE *fp;
    char username_buffer[MAX_USERNAME_LEN + 1];
    char password_buffer[MAX_PASSWORD_LEN + 1];

    user_count = 0; // Reset user count before loading

    d = opendir(USER_DATA_DIR);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Skip . and ..
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }

            // Check if it's a regular file and ends with ".user"
            if (strstr(dir->d_name, ".user") != NULL && strlen(dir->d_name) > 5) {
                // Extract username from filename (remove .user suffix)
                strncpy(username_buffer, dir->d_name, strlen(dir->d_name) - 5);
                username_buffer[strlen(dir->d_name) - 5] = '\0';

                get_user_filepath(username_buffer, filepath);
                fp = fopen(filepath, "r");
                if (fp) {
                    if (fscanf(fp, "%s", password_buffer) == 1) { // Read password
                        if (user_count < MAX_USERS) {
                            strcpy(users[user_count].username, username_buffer);
                            strcpy(users[user_count].password, password_buffer);
                            user_count++;
                        } else {
                            printf(COLOR_YELLOW "Warning: Max users reached, skipping '%s'.\n" COLOR_RESET, username_buffer);
                        }
                    }
                    fclose(fp);
                } else {
                    printf(COLOR_RED "Error: Could not open user file '%s'.\n" COLOR_RESET, filepath);
                }
            }
        }
        closedir(d);
    } else {
        printf(COLOR_YELLOW "Warning: Could not open user data directory '%s'. (May not exist or permissions issue)\n" COLOR_RESET, USER_DATA_DIR);
    }
}

void save_user_to_file(const User* user) {
    ensure_dir_exists(USER_DATA_DIR); // Ensure directory exists before writing
    char filepath[256];
    get_user_filepath(user->username, filepath);
    FILE *fp = fopen(filepath, "w"); // Overwrite if exists
    if (fp) {
        fprintf(fp, "%s\n", user->password);
        fclose(fp);
    } else {
        printf(COLOR_RED "Error: Could not save user file for '%s'.\n" COLOR_RESET, user->username);
    }
}

void delete_user_file(const char* username) {
    char filepath[256];
    get_user_filepath(username, filepath);
    if (remove(filepath) == 0) {
        printf(COLOR_GREEN "User file for '%s' deleted.\n" COLOR_RESET, username);
    } else {
        printf(COLOR_RED "Error: Could not delete user file for '%s'. (File might not exist or permissions issue)\n" COLOR_RESET, username);
    }
}

// --- Message/Conversation Storage Functions ---

void load_all_messages_from_files() {
    ensure_dir_exists(CONVERSATION_DATA_DIR); // Ensure directory exists before opening
    DIR *d;
    struct dirent *dir;
    char filepath[256];
    FILE *fp;
    char line_buffer[MAX_MESSAGE_LEN + MAX_USERNAME_LEN * 2 + 50]; // Generous buffer for a line
    
    message_count = 0; // Reset message count before loading

    d = opendir(CONVERSATION_DATA_DIR);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Skip . and ..
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }

            // Check if it's a regular file and ends with ".conv"
            if (strstr(dir->d_name, ".conv") != NULL && strlen(dir->d_name) > 6) {
                sprintf(filepath, "%s%s", CONVERSATION_DATA_DIR, dir->d_name); // Correctly reconstruct path

                fp = fopen(filepath, "r");
                if (fp) {
                    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
                        char *sender, *recipient, *read_status_str, *timestamp_str, *content;
                        
                        // Create a temporary buffer for strtok as it modifies the string
                        char temp_line[sizeof(line_buffer)];
                        strcpy(temp_line, line_buffer);
                        temp_line[strcspn(temp_line, "\n")] = 0; // Remove newline from temp_line

                        sender = strtok(temp_line, " ");
                        recipient = strtok(NULL, " ");
                        read_status_str = strtok(NULL, " ");
                        timestamp_str = strtok(NULL, " ");
                        content = strtok(NULL, ""); // Get the rest of the line

                        if (sender && recipient && read_status_str && timestamp_str && content) {
                            if (message_count < MAX_MESSAGES) {
                                strcpy(messages[message_count].sender, sender);
                                strcpy(messages[message_count].recipient, recipient);
                                messages[message_count].read_status = atoi(read_status_str);
                                messages[message_count].timestamp = (time_t)atol(timestamp_str);
                                strcpy(messages[message_count].content, content);
                                message_count++;
                            } else {
                                printf(COLOR_YELLOW "Warning: Max messages reached, skipping from '%s'.\n" COLOR_RESET, filepath);
                                break;
                            }
                        }
                    }
                    fclose(fp);
                } else {
                    printf(COLOR_RED "Error: Could not open conversation file '%s'.\n" COLOR_RESET, filepath);
                }
            }
        }
        closedir(d);
    } else {
        printf(COLOR_YELLOW "Warning: Could not open conversation data directory '%s'. (May not exist or permissions issue)\n" COLOR_RESET, CONVERSATION_DATA_DIR);
    }
}


void save_message_to_file(const Message* msg) {
    ensure_dir_exists(CONVERSATION_DATA_DIR); // Ensure directory exists before writing
    char filepath[256];
    get_conversation_filepath(msg->sender, msg->recipient, filepath);
    FILE *fp = fopen(filepath, "a"); // Append to file
    if (fp) {
        fprintf(fp, "%s %s %d %ld %s\n", msg->sender, msg->recipient, msg->read_status, (long)msg->timestamp, msg->content);
        fclose(fp);
    } else {
        printf(COLOR_RED "Error: Could not save message to conversation file for '%s' and '%s'.\n" COLOR_RESET, msg->sender, msg->recipient);
    }
}

void load_conversation_from_file(const char* user1, const char* user2, Message conversation_messages[], int* conversation_count) {
    ensure_dir_exists(CONVERSATION_DATA_DIR); // Ensure directory exists
    char filepath[256];
    get_conversation_filepath(user1, user2, filepath);
    FILE *fp = fopen(filepath, "r");
    char line_buffer[MAX_MESSAGE_LEN + MAX_USERNAME_LEN * 2 + 50]; // Generous buffer for a line

    *conversation_count = 0; // Reset count for this conversation

    if (fp) {
        while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
            char *sender, *recipient, *read_status_str, *timestamp_str, *content;
            
            // Create a temporary buffer for strtok as it modifies the string
            char temp_line[sizeof(line_buffer)];
            strcpy(temp_line, line_buffer);
            temp_line[strcspn(temp_line, "\n")] = 0; // Remove newline from temp_line

            sender = strtok(temp_line, " ");
            recipient = strtok(NULL, " ");
            read_status_str = strtok(NULL, " ");
            timestamp_str = strtok(NULL, " ");
            content = strtok(NULL, ""); // Get the rest of the line

            if (sender && recipient && read_status_str && timestamp_str && content) {
                if (*conversation_count < MAX_MESSAGES) { // Assuming MAX_MESSAGES is enough for a single conversation
                    strcpy(conversation_messages[*conversation_count].sender, sender);
                    strcpy(conversation_messages[*conversation_count].recipient, recipient);
                    conversation_messages[*conversation_count].read_status = atoi(read_status_str);
                    conversation_messages[*conversation_count].timestamp = (time_t)atol(timestamp_str);
                    strcpy(conversation_messages[*conversation_count].content, content);
                    (*conversation_count)++;
                } else {
                    printf(COLOR_YELLOW "Warning: Max messages for conversation reached, some messages skipped.\n" COLOR_RESET);
                    break;
                }
            }
        }
        fclose(fp);
    } else {
        // No error if file doesn't exist, just means no conversation yet.
    }
}


// --- Generic Data Loading/Saving (wrappers) ---
void load_all_data() {
    load_users_from_files();
    load_all_messages_from_files();
    printf(COLOR_GREEN "Data loading complete.\n" COLOR_RESET);
}

void save_all_data() {
    // Current strategy: users are saved when registered/updated, messages are saved when sent.
    // So this function is mainly a placeholder or for future batch saving of other data.
    printf(COLOR_GREEN "Data saving complete (individual saves handled).\n" COLOR_RESET);
}