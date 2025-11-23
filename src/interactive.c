#include "interactive.h"
#include "common.h"
#include "user.h" // For register_user, login_user, find_user_index
#include "send.h" // For send_message
#include "view.h" // For list_users, check_conve
#include "visuals.h" // For color codes


// Helper function to get a string input from the user
static void get_string_input(const char* prompt, char* buffer, int buffer_size) {
    printf("%s", prompt);
    if (fgets(buffer, buffer_size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
    } else {
        buffer[0] = 0; // Clear buffer on error
    }
}

// Helper function to get a numeric choice from the user
static int get_numeric_choice(const char* prompt) {
    char input_str[16];
    get_string_input(prompt, input_str, sizeof(input_str));
    return atoi(input_str);
}


// --- Interactive Flows ---

void interactive_register() {
    char username[MAX_USERNAME_LEN + 1];
    char password[MAX_PASSWORD_LEN + 1];
    char confirm_password[MAX_PASSWORD_LEN + 1];

    printf(COLOR_YELLOW "\n--- Register New User ---\n" COLOR_RESET);
    get_string_input("Enter username: ", username, sizeof(username));
    get_string_input("Enter password: ", password, sizeof(password));
    get_string_input("Confirm password: ", confirm_password, sizeof(confirm_password));

    if (strlen(username) == 0 || strlen(password) == 0 || strlen(confirm_password) == 0) {
        printf(COLOR_RED "Error: All fields are required.\n" COLOR_RESET);
        return;
    }

    if (strcmp(password, confirm_password) != 0) {
        printf(COLOR_RED "Error: Passwords do not match.\n" COLOR_RESET);
        return;
    }

    register_user(username, password);
}

void interactive_login() {
    char username[MAX_USERNAME_LEN + 1];
    char password[MAX_PASSWORD_LEN + 1];

    printf(COLOR_YELLOW "\n--- Login ---\n" COLOR_RESET);
    get_string_input("Enter username: ", username, sizeof(username));
    get_string_input("Enter password: ", password, sizeof(password));

    if (strlen(username) == 0 || strlen(password) == 0) {
        printf(COLOR_RED "Error: Both username and password are required.\n" COLOR_RESET);
        return;
    }

    login_user(username, password);
}

// Helper function to list users and get a numeric selection
// Returns 0 on success, -1 on error. Selected username is copied to selected_username.
int select_user_from_list(const char* prompt_message, char selected_username[]) {
    if (user_count == 0) {
        printf(COLOR_YELLOW "No users registered yet.\n" COLOR_RESET);
        return -1;
    }

    printf(COLOR_CYAN "%s\n" COLOR_RESET, prompt_message);
    printf(COLOR_CYAN "----------------------------------------\n" COLOR_RESET);
    for (int i = 0; i < user_count; i++) {
        printf(COLOR_YELLOW "%d. %s\n" COLOR_RESET, i + 1, users[i].username);
    }
    printf(COLOR_CYAN "----------------------------------------\n" COLOR_RESET);

    int choice = get_numeric_choice(COLOR_YELLOW "Your choice: " COLOR_RESET);

    if (choice > 0 && choice <= user_count) {
        strcpy(selected_username, users[choice - 1].username);
        return 0; // Success
    } else {
        printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        return -1; // Error
    }
}


void interactive_send() {
    char recipient[MAX_USERNAME_LEN + 1];
    char message_content[MAX_MESSAGE_LEN + 1];

    printf(COLOR_YELLOW "\n--- Send Message ---\n" COLOR_RESET);
    if (select_user_from_list("Who would you like to send a message to?", recipient) != 0) {
        return; // User selection failed or cancelled
    }
    
    get_string_input("Enter your message: ", message_content, sizeof(message_content));

    if (strlen(message_content) == 0) {
        printf(COLOR_RED "Error: Message cannot be empty.\n" COLOR_RESET);
        return;
    }

    send_message(recipient, message_content);
}

void interactive_check_conve() {
    char conv_partner[MAX_USERNAME_LEN + 1];

    printf(COLOR_YELLOW "\n--- View Conversation ---\n" COLOR_RESET);
    if (select_user_from_list("View conversation with whom?", conv_partner) != 0) {
        return; // User selection failed or cancelled
    }

    check_conve(conv_partner);
}

void interactive_logout() {
    // This function simply calls the logout logic and prints a message.
    // The actual state change (logged_in = 0) is handled in command_handler.
    printf(COLOR_GREEN "Logged out successfully.\n" COLOR_RESET);
}

