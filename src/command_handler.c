#include "command_handler.h"
#include "common.h"
#include "user.h" // For register_user, login_user
#include "send.h" // For send_message
#include "view.h" // For check_notification, check_conve, list_conv, list_users
#include "ui.h" // For show_loggedin_prompt, show_login_prompt
#include "storage.h" // For save_all_data

// Note: globals are extern from common.h and defined in globals.c

int handle_command(char* input_buffer) {
    char temp_buffer[256];
    strcpy(temp_buffer, input_buffer); // Work on a copy as strtok modifies the string

    char *command = strtok(temp_buffer, " ");
    char *arg1, *arg2, *arg3;

    if (command == NULL) {
        return 0; // Empty input, continue
    }

    if (strcmp(command, "exit") == 0) {
        printf("Exiting application.\n");
        save_all_data(); // Save data before exiting
        return 1; // Signal main loop to exit
    }

    if (logged_in) {
        // Logged-in commands
        if (strcmp(command, "logout") == 0) {
            logged_in = 0;
            current_username[0] = '\0'; // Clear username
            printf("Logged out successfully.\n");
        } else if (strcmp(command, "send") == 0) {
            arg1 = strtok(NULL, " "); // recipient
            char* message_content_start = strtok(NULL, ""); // Rest of the line is content

            if (arg1 != NULL && message_content_start != NULL) {
                send_message(arg1, message_content_start);
            } else {
                printf("Usage: send <recipient> <message>\n");
            }
        } else if (strcmp(command, "check_notification") == 0) {
            check_notification();
        } else if (strcmp(command, "check_conve") == 0) {
            arg1 = strtok(NULL, " "); // conv_name (other user)
            if (arg1 != NULL && strtok(NULL, " ") == NULL) {
                check_conve(arg1);
            } else {
                printf("Usage: check_conve <conv_name>\n");
            }
        } else if (strcmp(command, "list_users") == 0) {
             list_users();
        } else if (strcmp(command, "list_conv") == 0) {
            list_conv();
        } else {
            printf("Unknown command: %s\n", command);
        }
    } else {
        // Not logged-in commands
        if (strcmp(command, "register") == 0) {
            arg1 = strtok(NULL, " "); // username
            arg2 = strtok(NULL, " "); // password
            arg3 = strtok(NULL, " "); // confirm password

            if (arg1 != NULL && arg2 != NULL && arg3 != NULL && strtok(NULL, " ") == NULL) {
                if (strcmp(arg2, arg3) == 0) {
                    register_user(arg1, arg2);
                } else {
                    printf("Error: Passwords do not match.\n");
                }
            } else {
                printf("Usage: register <username> <passwd> <passwd>\n");
            }
        } else if (strcmp(command, "login") == 0) {
            arg1 = strtok(NULL, " "); // username
            arg2 = strtok(NULL, " "); // password

            if (arg1 != NULL && arg2 != NULL && strtok(NULL, " ") == NULL) {
                login_user(arg1, arg2);
            } else {
                printf("Usage: login <username> <passwd>\n");
            }
        } else {
            printf("Unknown command or not logged in: %s\n", command);
        }
    }
    printf("\n"); // Add a newline for better readability between commands
    return 0; // Continue main loop
}
