#include "view.h"
#include "common.h"
#include "user.h" // For find_user_index
#include "storage.h" // For file loading/saving

// Static helper function, only visible within this file
static void rewrite_conversation_file(const char* user1, const char* user2, const Message conversation[], int count) {
    char filepath[256];
    get_conversation_filepath(user1, user2, filepath);
    FILE *fp = fopen(filepath, "w"); // "w" to overwrite
    if (fp) {
        for (int i = 0; i < count; i++) {
            fprintf(fp, "%s %s %d %ld %s\n", conversation[i].sender, conversation[i].recipient, conversation[i].read_status, (long)conversation[i].timestamp, conversation[i].content);
        }
        fclose(fp);
    } else {
        printf("Error: Could not rewrite conversation file for '%s' and '%s'.\n", user1, user2);
    }
}

// Lists all registered users
void list_users() {
     printf("Registered Users:\n");
    for (int i = 0; i < user_count; i++) {
        printf("- %s\n", users[i].username);
    }
    if (user_count == 0) {
        printf("No users registered yet.\n");
    }
}

// Displays messages for the current_username and marks them as read
void check_notification() {
    printf("--- Notifications for %s ---\n", current_username);
    int found_new_messages = 0;
    
    char partners[MAX_USERS][MAX_USERNAME_LEN + 1];
    int partner_count = 0;
    
    for (int i = 0; i < message_count; i++) {
        const char* other_person = NULL;
        if (strcmp(messages[i].sender, current_username) == 0) {
            other_person = messages[i].recipient;
        } else if (strcmp(messages[i].recipient, current_username) == 0) {
            other_person = messages[i].sender;
        }

        if (other_person) {
            int already_listed = 0;
            for (int j = 0; j < partner_count; j++) {
                if (strcmp(partners[j], other_person) == 0) {
                    already_listed = 1;
                    break;
                }
            }
            if (!already_listed) {
                strcpy(partners[partner_count], other_person);
                partner_count++;
            }
        }
    }

    for (int i = 0; i < partner_count; i++) {
        Message conversation_buffer[MAX_MESSAGES];
        int conv_count = 0;
        int needs_rewrite = 0;
        
        load_conversation_from_file(current_username, partners[i], conversation_buffer, &conv_count);

        for (int j = 0; j < conv_count; j++) {
            if (strcmp(conversation_buffer[j].recipient, current_username) == 0 && conversation_buffer[j].read_status == 0) {
                char* time_str = ctime(&conversation_buffer[j].timestamp);
                time_str[strcspn(time_str, "\n")] = 0;

                printf("From %s at %s: %s\n", conversation_buffer[j].sender, time_str, conversation_buffer[j].content);
                
                conversation_buffer[j].read_status = 1;
                needs_rewrite = 1;
                found_new_messages = 1;
            }
        }

        if (needs_rewrite) {
            rewrite_conversation_file(current_username, partners[i], conversation_buffer, conv_count);
        }
    }

    if (!found_new_messages) {
        printf("No new notifications.\n");
    }
    printf("---------------------------\n");
}

// Displays conversation with a specific user
void check_conve(const char* conv_name) {
    if (find_user_index(conv_name) == -1) {
        printf("Error: User '%s' does not exist.\n", conv_name);
        return;
    }
    
    printf("--- Conversation with %s ---\n", conv_name);
    Message conversation_buffer[MAX_MESSAGES];
    int conv_count = 0;

    load_conversation_from_file(current_username, conv_name, conversation_buffer, &conv_count);

    if (conv_count > 0) {
        for (int i = 0; i < conv_count; i++) {
            char* time_str = ctime(&conversation_buffer[i].timestamp);
            time_str[strcspn(time_str, "\n")] = 0;
            
            printf("[%s] %s: %s\n", time_str, conversation_buffer[i].sender, conversation_buffer[i].content);
        }
    } else {
        printf("No conversation history with %s.\n", conv_name);
    }
    printf("----------------------------\n");
}

// Lists all unique conversation partners for the current user
void list_conv() {
    printf("--- Conversations for %s ---\n", current_username);
    char partners[MAX_USERS][MAX_USERNAME_LEN + 1];
    int partner_count = 0;
    
    for (int i = 0; i < message_count; i++) {
        const char* other_person = NULL;
        if (strcmp(messages[i].sender, current_username) == 0) {
            other_person = messages[i].recipient;
        } else if (strcmp(messages[i].recipient, current_username) == 0) {
            other_person = messages[i].sender;
        }

        if (other_person) {
            int already_listed = 0;
            for (int j = 0; j < partner_count; j++) {
                if (strcmp(partners[j], other_person) == 0) {
                    already_listed = 1;
                    break;
                }
            }
            if (!already_listed) {
                strcpy(partners[partner_count], other_person);
                partner_count++;
            }
        }
    }
    
    if (partner_count > 0) {
        for (int i = 0; i < partner_count; i++) {
            printf("- %s\n", partners[i]);
        }
    } else {
        printf("No active conversations.\n");
    }
    printf("----------------------------\n");
}

