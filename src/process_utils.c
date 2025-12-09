#include "include/process_utils.h"

// Get the UID of a process by PID (returns -1 on error)
uid_t proc_get_uid(int32_t pid) {
    char filename[FILENAME_LENGTH] = {0};
    snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
    
    FILE* fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        return (uid_t)-1;
    }
    
    char line[BUFFER_LENGTH];
    uid_t uid = (uid_t)-1;
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "Uid:", 4) == 0) {
            // Format: "Uid:    1000    1000    1000    1000"
            // First value is real UID
            if (sscanf(line, "Uid: %u", (unsigned int*)&uid) == 1) {
                break;
            }
        }
    }
    
    fclose(fp);
    return uid;
}

int proc_alive(int32_t pid) {
    char filename[FILENAME_LENGTH] = {0};
    sprintf(filename, "/proc/%d/stat", pid);

    FILE* fp;
    if ((fp = fopen(filename, "r")) == NULL) {   
        return PROC_STATE_NONALIVE;
    }

    int __pid;
    char state;
    char name_buf[BUFFER_LENGTH] = {0};
    int num = fscanf(fp, "%d %s %c", &__pid, name_buf, &state);
    int res;
    if (num != 3 || num == EOF) {
        res = PROC_STATE_UNKNOWN;
    } else if (state == 'Z' || state == 'X' || state == 'x') {
        res = PROC_STATE_NONALIVE;
    } else {
        res = PROC_STATE_ALIVE;
    }
    fclose(fp);
    return res;
}

