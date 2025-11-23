#include "command_handler.h"
#include "common.h"
#include "user.h" // For register_user, login_user
#include "send.h" // For send_message
#include "view.h" // For check_notification, check_conve, list_conv, list_users
#include "ui.h" // For show_loggedin_prompt, show_login_prompt
#include "storage.h" // For save_all_data
#include "visuals.h" // For color codes
#include "interactive.h" // For interactive functions

// Note: globals are extern from common.h and defined in globals.c

int handle_command(char* input_buffer) {
    char temp_buffer[256];
    strcpy(temp_buffer, input_buffer); // Work on a copy as strtok modifies the string

    // Remove trailing newline if present
    temp_buffer[strcspn(temp_buffer, "\n")] = 0;

    // Convert input to integer
    int choice = atoi(temp_buffer);

    if (choice == 0 && strcmp(temp_buffer, "0") != 0) { // If atoi returns 0, but input was not "0"
        printf(COLOR_RED "Invalid input. Please enter a number.\n" COLOR_RESET);
        return 0;
    }

    if (logged_in) {
        // Logged-in menu choices
        switch (choice) {
            case 1: // Send Message
                interactive_send();
                break;
            case 2: // Check New Notifications
                check_notification();
                break;
            case 3: // View a Conversation
                interactive_check_conve();
                break;
            case 4: // List All Users
                list_users();
                break;
            case 5: // List My Conversations
                list_conv();
                break;
            case 6: // Logout
                logged_in = 0;
                current_username[0] = '\0'; // Clear username
                printf(COLOR_GREEN "Logged out successfully.\n" COLOR_RESET);
                break;
            case 7: // Exit
                printf("Exiting application.\n");
                save_all_data();
                return 1; // Signal main loop to exit
            default:
                printf(COLOR_RED "Invalid choice. Please enter a number from the menu.\n" COLOR_RESET);
                break;
        }
    } else {
        // Logged-out menu choices
        switch (choice) {
            case 1: // Register
                interactive_register();
                break;
            case 2: // Login
                interactive_login();
                break;
            case 3: // Exit
                printf("Exiting application.\n");
                save_all_data();
                return 1; // Signal main loop to exit
            default:
                printf(COLOR_RED "Invalid choice. Please enter a number from the menu.\n" COLOR_RESET);
                break;
        }
    }
    printf("\n"); // Add a newline for better readability between commands
    return 0; // Continue main loop
}
