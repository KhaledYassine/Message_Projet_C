#include "send.h"
#include "common.h"
#include "user.h" // For find_user_index
#include "storage.h" // For save_message_to_file
#include "visuals.h" // For color codes

// Sends a message from current_username to recipient. Returns 0 on success, -1 on failure.
int send_message(const char* recipient, const char* content) {
    if (find_user_index(recipient) == -1) {
        printf(COLOR_RED "Error: Recipient '%s' does not exist.\n" COLOR_RESET, recipient);
        return -1;
    }
    if (strlen(content) > MAX_MESSAGE_LEN) {
        printf(COLOR_RED "Error: Message too long (max %d characters).\n" COLOR_RESET, MAX_MESSAGE_LEN);
        return -1;
    }

    // Create a message struct
    Message msg;
    strcpy(msg.sender, current_username);
    strcpy(msg.recipient, recipient);
    strcpy(msg.content, content);
    msg.read_status = 0; // New messages are always unread
    msg.timestamp = time(NULL); // Set current time

    // Save to file
    save_message_to_file(&msg);
    
    // Also add to in-memory message list
    if(message_count < MAX_MESSAGES){
        messages[message_count] = msg;
        message_count++;
    }

    printf(COLOR_GREEN "Message sent to '%s'.\n" COLOR_RESET, recipient);
    return 0;
}