#ifndef STORAGE_H
#define STORAGE_H

#include "common.h"

// --- Helper Functions ---
void get_conversation_filepath(const char* user1, const char* user2, char* filepath);

// --- User Storage Functions ---
void load_users_from_files();
void save_user_to_file(const User* user);
void delete_user_file(const char* username); // To be implemented later if needed

// --- Message/Conversation Storage Functions ---
void load_all_messages_from_files(); // Loads all messages from all conversation files
void save_message_to_file(const Message* msg); // Appends a single message to a conversation file
void load_conversation_from_file(const char* user1, const char* user2, Message conversation_messages[], int* conversation_count);


// --- Generic Data Loading/Saving (wrappers) ---
void load_all_data(); // Wrapper for loading all users and messages
void save_all_data(); // Wrapper for saving all users and messages (for now, mainly user files)

#endif // STORAGE_H