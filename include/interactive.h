#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "common.h" // For MAX_USERNAME_LEN, etc.

// Function to handle interactive registration flow
void interactive_register();

// Function to handle interactive login flow
void interactive_login();

// Function to handle interactive send message flow
void interactive_send();

// Function to handle interactive check conversation flow
void interactive_check_conve();

// Function to handle interactive logout flow
void interactive_logout();

// Helper function to list users and get a numeric selection
// Returns the index of the selected user, or -1 on error/cancellation.
int select_user_from_list(const char* prompt_message, char selected_username[]);

#endif // INTERACTIVE_H
