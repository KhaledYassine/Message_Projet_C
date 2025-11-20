#ifndef CONVERSATION_MANAGER_H
#define CONVERSATION_MANAGER_H

#include "conversation.h"
#include "message.h"

/**
 * @brief Saves conversation metadata to its meta.dat file.
 *
 * @param username The owner of the conversation.
 * @param cnv_id The unique ID of the conversation.
 * @param conv A pointer to the Conversation struct containing the data to save.
 * @return 0 on success, -1 on failure.
 */
int save_conversation_metadata(const char* username, const char* cnv_id, const Conversation* conv);

/**
 * @brief Loads conversation metadata from its meta.dat file.
 *
 * @param username The owner of the conversation.
 * @param cnv_id The unique ID of the conversation.
 * @param conv A pointer to a Conversation struct to be populated with the loaded data.
 * @return 0 on success, -1 on failure.
 */
int load_conversation_metadata(const char* username, const char* cnv_id, Conversation* conv);

/**
 * @brief Appends a single message to the local conversation log.
 *
 * @param username The owner of the conversation.
 * @param cnv_id The unique ID of the conversation.
 * @param msg A pointer to the Message struct to append.
 * @return 0 on success, -1 on failure.
 */
int append_message_to_local_log(const char* username, const char* cnv_id, const Message* msg);

/**
 * @brief Lists all conversations for a given user.
 *
 * @param username The user whose conversations will be listed.
 * @return 0 on success, -1 on failure.
 */
int list_conversations(const char* username);

/**
 * @brief Reads and displays messages from a specific conversation.
 *        Displays the last 10 messages with a prestigious design.
 *
 * @param username The user whose conversation messages are to be read.
 * @param cnv_id The unique ID of the conversation.
 * @return 0 on success, -1 on failure.
 */
int read_conversation_messages(const char* username, const char* cnv_id);

#endif // CONVERSATION_MANAGER_H
