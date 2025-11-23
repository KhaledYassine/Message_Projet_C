#include "ui.h"
#include "common.h" // For const char* username
#include "visuals.h" // For color codes

void show_login_prompt() {
    printf(TEXT_BOLD COLOR_CYAN "Welcome to the C Messaging App\n" COLOR_RESET);
    printf(COLOR_CYAN "----------------------------------------\n" COLOR_RESET);
    printf(TEXT_BOLD "Menu:\n" COLOR_RESET);
    printf(COLOR_YELLOW "  1. Register\n" COLOR_RESET);
    printf(COLOR_YELLOW "  2. Login\n" COLOR_RESET);
    printf(COLOR_YELLOW "  3. Exit\n" COLOR_RESET);
    printf(COLOR_CYAN "----------------------------------------\n" COLOR_RESET);
}

void show_loggedin_prompt(const char* username) {
    printf(TEXT_BOLD COLOR_GREEN "Logged in as: %s\n" COLOR_RESET, username);
    printf(COLOR_GREEN "----------------------------------------\n" COLOR_RESET);
    printf(TEXT_BOLD "Menu:\n" COLOR_RESET);
    printf(COLOR_YELLOW "  1. Send Message\n" COLOR_RESET);
    printf(COLOR_YELLOW "  2. Check New Notifications\n" COLOR_RESET);
    printf(COLOR_YELLOW "  3. View a Conversation\n" COLOR_RESET);
    printf(COLOR_YELLOW "  4. List All Users\n" COLOR_RESET);
    printf(COLOR_YELLOW "  5. List My Conversations\n" COLOR_RESET);
    printf(COLOR_YELLOW "  6. Logout\n" COLOR_RESET);
    printf(COLOR_YELLOW "  7. Exit\n" COLOR_RESET); // Exit is always an option
    printf(COLOR_GREEN "----------------------------------------\n" COLOR_RESET);
}

