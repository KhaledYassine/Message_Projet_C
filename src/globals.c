#include "common.h"

// --- Global Data (definitions) ---
// These are defined here and declared extern in common.h
User users[MAX_USERS];
int user_count = 0;
Message messages[MAX_MESSAGES];
int message_count = 0;

char current_username[MAX_USERNAME_LEN + 1];
int logged_in = 0;
