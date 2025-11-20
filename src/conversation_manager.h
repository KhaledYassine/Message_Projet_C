#ifndef CONVERSATION_MANAGER_H
#define CONVERSATION_MANAGER_H

#include "conversation.h"

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

#endif // CONVERSATION_MANAGER_H
