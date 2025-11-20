#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "conversation_manager.h"

void show_usage() {
    printf("Usage:\n");
    printf("  messenger createuser <username> <password>\n");
    printf("  messenger createconv <username> <conv_name> <participants>\n");
    printf("  messenger login <username> <password>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_usage();
        return 1;
    }

    char* command = argv[1];

    if (strcmp(command, "createuser") == 0) {
        if (argc != 4) {
            printf("Error: createuser requires <username> and <password>.\n");
            show_usage();
            return 1;
        }
        char* username = argv[2];
        char* password = argv[3];
        if (create_user(username, password) != 0) {
            printf("Failed to create user.\n");
            return 1;
        }
    } else if (strcmp(command, "createconv") == 0) {
        if (argc != 5) {
            printf("Error: createconv requires <username> <conv_name> and <participants> (comma-separated).\n");
            show_usage();
            return 1;
        }
        char* username = argv[2];
        char* conv_name = argv[3];
        char* participants_str = argv[4];

        Conversation new_conv = {0};

        // Generate a simple unique ID based on timestamp
        snprintf(new_conv.cnv_id, sizeof(new_conv.cnv_id), "%ld", time(NULL));
        
        strncpy(new_conv.name, conv_name, sizeof(new_conv.name) - 1);
        new_conv.last_timestamp = time(NULL);

        // Parse participants
        int i = 0;
        char* token = strtok(participants_str, ",");
        while (token != NULL && i < MAX_PARTICIPANTS) {
            strncpy(new_conv.participants[i], token, sizeof(new_conv.participants[i]) - 1);
            i++;
            token = strtok(NULL, ",");
        }
        new_conv.num_participants = i;

        if (save_conversation_metadata(username, new_conv.cnv_id, &new_conv) != 0) {
            printf("Failed to create conversation.\n");
            return 1;
        }

    } else if (strcmp(command, "login") == 0) {
         if (argc != 4) {
            printf("Error: login requires <username> and <password>.\n");
            show_usage();
            return 1;
        }
        // TODO: Implement login logic using verify_user
        printf("Login functionality not implemented yet.\n");
    } else {
        printf("Error: Unknown command '%s'\n", command);
        show_usage();
        return 1;
    }

    return 0;
}
