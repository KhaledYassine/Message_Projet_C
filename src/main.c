#include "common.h"
#include "user.h"
#include "send.h" // No longer directly used, but potentially for consistency check
#include "view.h" // No longer directly used, but potentially for consistency check
#include "ui.h"
#include "storage.h"
#include "command_handler.h" // New include


int main() {
    char input_buffer[256];
    
    // Load initial data 
    load_all_data();

    while (1) {
        if (logged_in) {
            show_loggedin_prompt(current_username);
        } else {
            show_login_prompt();
        }

        printf("> ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            break;
        }

        // Remove trailing newline
        input_buffer[strcspn(input_buffer, "\n")] = 0;

        // Handle the command via the command handler
        if (handle_command(input_buffer) == 1) { // If handle_command signals to exit
            break;
        }
    }

    return 0;
}