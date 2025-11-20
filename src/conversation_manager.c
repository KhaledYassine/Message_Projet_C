#include "conversation_manager.h"
#include "message.h" // Include message.h for Message struct
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

int save_conversation_metadata(const char* username, const char* cnv_id, const Conversation* conv) {
    char conv_dir[512];
    snprintf(conv_dir, sizeof(conv_dir), "user/%s/conversations/%s", username, cnv_id);

    // Create the specific conversation directory
    if (MKDIR(conv_dir) != 0) {
        // On Windows, _mkdir returns -1 if path exists. On POSIX, it's EEXIST.
        // We can ignore the error if the directory already exists, but for simplicity,
        // we'll just proceed. A more robust solution would check errno.
    }

    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "%s/meta.dat", conv_dir);

    FILE* fp = fopen(meta_path, "w");
    if (!fp) {
        perror("Failed to open meta.dat for writing");
        return -1;
    }

    fprintf(fp, "cnv_name=%s\n", conv->name);
    fprintf(fp, "last_timestamp=%lld\n", (long long)conv->last_timestamp);

    fprintf(fp, "participants=");
    for (int i = 0; i < conv->num_participants; ++i) {
        fprintf(fp, "%s%s", conv->participants[i], (i == conv->num_participants - 1) ? "" : ",");
    }
    fprintf(fp, "\n");

    fclose(fp);
    printf("Successfully saved metadata to %s\n", meta_path);
    return 0;
}

int load_conversation_metadata(const char* username, const char* cnv_id, Conversation* conv) {
    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "user/%s/conversations/%s/meta.dat", username, cnv_id);

    FILE* fp = fopen(meta_path, "r");
    if (!fp) {
        perror("Failed to open meta.dat for reading");
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        char* key = strtok(line, "=");
        char* value = strtok(NULL, ""); // Get the rest of the line

        if (!key || !value) continue;

        if (strcmp(key, "cnv_name") == 0) {
            strncpy(conv->name, value, sizeof(conv->name) - 1);
        } else if (strcmp(key, "last_timestamp") == 0) {
            conv->last_timestamp = atoll(value);
        } else if (strcmp(key, "participants") == 0) {
            int i = 0;
            char* token = strtok(value, ",");
            while (token != NULL && i < MAX_PARTICIPANTS) {
                strncpy(conv->participants[i], token, sizeof(conv->participants[i]) - 1);
                i++;
                token = strtok(NULL, ",");
            }
            conv->num_participants = i;
        }
    }

    strncpy(conv->cnv_id, cnv_id, sizeof(conv->cnv_id) - 1);

    fclose(fp);
    printf("Successfully loaded metadata from %s\n", meta_path);
    return 0;
}

int append_message_to_local_log(const char* username, const char* cnv_id, const Message* msg) {
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "user/%s/conversations/%s/messages_local.txt", username, cnv_id);

    FILE* fp = fopen(log_path, "a"); // "a" for append mode
    if (!fp) {
        perror("Failed to open messages_local.txt for appending");
        return -1;
    }

    fprintf(fp, "%s::%lld::%d::%s\n", msg->user, (long long)msg->timestamp, msg->length, msg->content);

    fclose(fp);
    printf("Successfully appended message to %s\n", log_path);
    return 0;
}

#if defined(_WIN32)
#include <windows.h>

int list_conversations(const char* username) {
    char search_path[1024];
    snprintf(search_path, sizeof(search_path), "user/%s/conversations/*", username);

    WIN32_FIND_DATA find_data;
    HANDLE h_find = FindFirstFile(search_path, &find_data);

    if (h_find == INVALID_HANDLE_VALUE) {
        printf("No conversations found for user '%s' or directory not found.\n", username);
        return -1;
    }

    printf("Conversations for %s:\n", username);
    printf("----------------------------------------\n");

    do {
        // Check if it's a directory and not "." or ".."
        if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            strcmp(find_data.cFileName, ".") != 0 &&
            strcmp(find_data.cFileName, "..") != 0)
        {
            char* cnv_id = find_data.cFileName;
            Conversation conv = {0};
            if (load_conversation_metadata(username, cnv_id, &conv) == 0) {
                printf("  ID: %s | Name: %s\n", conv.cnv_id, conv.name);
            } else {
                printf("  Could not load metadata for conversation ID: %s\n", cnv_id);
            }
        }
    } while (FindNextFile(h_find, &find_data) != 0);

    FindClose(h_find);
    printf("----------------------------------------\n");
    return 0;
}

#else

// Placeholder for POSIX/Linux implementation
int list_conversations(const char* username) {
    printf("list_conversations is not implemented for this operating system.\n");
    return -1;
}

#endif

// Helper function to parse a message line into a Message struct
// Returns 0 on success, -1 on failure
int parse_message_line(const char* line, Message* msg) {
    char temp_line[1024];
    strncpy(temp_line, line, sizeof(temp_line) - 1);
    temp_line[sizeof(temp_line) - 1] = '\0'; // Ensure null termination

    char* user_str = strtok(temp_line, "::");
    char* timestamp_str = strtok(NULL, "::");
    char* length_str = strtok(NULL, "::");
    char* content_str = strtok(NULL, "\n"); // Content is until newline

    if (!user_str || !timestamp_str || !length_str || !content_str) {
        return -1; // Parsing failed
    }

    strncpy(msg->user, user_str, sizeof(msg->user) - 1);
    msg->user[sizeof(msg->user) - 1] = '\0';
    msg->timestamp = atol(timestamp_str);
    msg->length = atoi(length_str);
    strncpy(msg->content, content_str, sizeof(msg->content) - 1);
    msg->content[sizeof(msg->content) - 1] = '\0';

    return 0;
}

// Max width for the message display box
#define MAX_DISPLAY_WIDTH 80

int read_conversation_messages(const char* username, const char* cnv_id) {
    Conversation conv = {0};
    if (load_conversation_metadata(username, cnv_id, &conv) != 0) {
        printf("Error: Could not load conversation metadata for ID %s.\n", cnv_id);
        return -1;
    }

    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "user/%s/conversations/%s/messages_local.txt", username, cnv_id);

    FILE* fp = fopen(log_path, "r");
    if (!fp) {
        printf("No messages found for conversation ID %s.\n", cnv_id);
        return 0; // Not an error if file doesn't exist yet
    }

    // Read all messages into a dynamically growing array
    Message* messages = NULL;
    int message_count = 0;
    int capacity = 10; // Initial capacity

    messages = (Message*)malloc(capacity * sizeof(Message));
    if (!messages) {
        perror("Failed to allocate memory for messages");
        fclose(fp);
        return -1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        if (message_count >= capacity) {
            capacity *= 2;
            Message* temp = (Message*)realloc(messages, capacity * sizeof(Message));
            if (!temp) {
                perror("Failed to reallocate memory for messages");
                free(messages);
                fclose(fp);
                return -1;
            }
            messages = temp;
        }
        if (parse_message_line(line, &messages[message_count]) == 0) {
            message_count++;
        }
    }
    fclose(fp);

    // Print the prestigious design
    printf("+");
    for (int i = 0; i < MAX_DISPLAY_WIDTH - 2; ++i) printf("-");
    printf("+\n");

    printf("| Conversation: %-*s |\n", MAX_DISPLAY_WIDTH - 16, conv.name);

    printf("+");
    for (int i = 0; i < MAX_DISPLAY_WIDTH - 2; ++i) printf("-");
    printf("+\n");

    int start_index = (message_count > 10) ? (message_count - 10) : 0;

    if (message_count == 0) {
        printf("| %-*s |\n", MAX_DISPLAY_WIDTH - 4, "No messages in this conversation yet.");
    } else {
        for (int i = start_index; i < message_count; ++i) {
            char time_str[20]; // YYYY-MM-DD HH:MM:SS is 19 chars + null
            struct tm *local_time = localtime(&messages[i].timestamp);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);

            char final_buffer[MAX_DISPLAY_WIDTH + 1];
            int offset = 0;
            
            // Stage 1: Print the prefix and get its length
            offset += snprintf(final_buffer, sizeof(final_buffer), "[%s] %s: ", time_str, messages[i].user);

            // Stage 2: Calculate remaining space and append content safely
            int space_left_for_content = (MAX_DISPLAY_WIDTH - 4) - offset;
            if (space_left_for_content > 0) {
                int content_len = strlen(messages[i].content);
                if (content_len > space_left_for_content) {
                    // Content needs truncation
                    if (space_left_for_content > 3) {
                        snprintf(final_buffer + offset, space_left_for_content + 1, "%.*s...", space_left_for_content - 3, messages[i].content);
                    } else {
                        // Not enough space for ellipsis, just hard truncate
                        snprintf(final_buffer + offset, space_left_for_content + 1, "%.*s", space_left_for_content, messages[i].content);
                    }
                } else {
                    // Content fits perfectly
                    snprintf(final_buffer + offset, space_left_for_content + 1, "%s", messages[i].content);
                }
            }
            
            printf("| %-*s |\n", MAX_DISPLAY_WIDTH - 4, final_buffer);
        }
    }

    printf("+");
    for (int i = 0; i < MAX_DISPLAY_WIDTH - 2; ++i) printf("-");
    printf("+\n");

    free(messages);
    return 0;
}
