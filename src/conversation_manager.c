#include "conversation_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

int save_conversation_metadata(const char* username, const char* cnv_id, const Conversation* conv) {
    char conv_dir[512];
    snprintf(conv_dir, sizeof(conv_dir), "user/%s/conversations/%s", username, cnv_id);

    // Create the specific conversation directory
    if (MKDIR(conv_dir) != 0) {
        // On Windows, _mkdir returns -1 if path exists. On POSIX, it's EEXIST.
        // We can ignore the error if the directory already exists, but for simplicity,
        // we'll just proceed. A more robust solution would check errno.
    }

    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "%s/meta.dat", conv_dir);

    FILE* fp = fopen(meta_path, "w");
    if (!fp) {
        perror("Failed to open meta.dat for writing");
        return -1;
    }

    fprintf(fp, "cnv_name=%s\n", conv->name);
    fprintf(fp, "last_timestamp=%ld\n", conv->last_timestamp);

    fprintf(fp, "participants=");
    for (int i = 0; i < conv->num_participants; ++i) {
        fprintf(fp, "%s%s", conv->participants[i], (i == conv->num_participants - 1) ? "" : ",");
    }
    fprintf(fp, "\n");

    fclose(fp);
    printf("Successfully saved metadata to %s\n", meta_path);
    return 0;
}

int load_conversation_metadata(const char* username, const char* cnv_id, Conversation* conv) {
    char meta_path[1024];
    snprintf(meta_path, sizeof(meta_path), "user/%s/conversations/%s/meta.dat", username, cnv_id);

    FILE* fp = fopen(meta_path, "r");
    if (!fp) {
        perror("Failed to open meta.dat for reading");
        return -1;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        char* key = strtok(line, "=");
        char* value = strtok(NULL, ""); // Get the rest of the line

        if (!key || !value) continue;

        if (strcmp(key, "cnv_name") == 0) {
            strncpy(conv->name, value, sizeof(conv->name) - 1);
        } else if (strcmp(key, "last_timestamp") == 0) {
            conv->last_timestamp = atol(value);
        } else if (strcmp(key, "participants") == 0) {
            int i = 0;
            char* token = strtok(value, ",");
            while (token != NULL && i < MAX_PARTICIPANTS) {
                strncpy(conv->participants[i], token, sizeof(conv->participants[i]) - 1);
                i++;
                token = strtok(NULL, ",");
            }
            conv->num_participants = i;
        }
    }

    strncpy(conv->cnv_id, cnv_id, sizeof(conv->cnv_id) - 1);

    fclose(fp);
    printf("Successfully loaded metadata from %s\n", meta_path);
    return 0;
}
