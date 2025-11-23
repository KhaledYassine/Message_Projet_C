#include "ui.h"
#include "common.h" // For const char* username

void show_login_prompt() {
    printf("Welcome to the C Messaging App\n");
    printf("----------------------------------------\n");
    printf("Commands:\n");
    printf("  register <username> <passwd> <passwd>\n");
    printf("  login <username> <passwd>\n");
    printf("  exit\n");
    printf("----------------------------------------\n");
}

void show_loggedin_prompt(const char* username) {
    printf("Logged in as: %s\n", username);
    printf("----------------------------------------\n");
    printf("Commands:\n");
    printf("  send <recipient> <message>\n");
    printf("  check_notification\n");
    printf("  check_conve <conv_name>\n");
    printf("  list_users\n");
    printf("  list_conv\n");
    printf("  logout\n");
    printf("  exit\n");
    printf("----------------------------------------\n");
}

