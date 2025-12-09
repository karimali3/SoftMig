#ifndef __UTILS_PROCESS_UTILS_H__
#define __UTILS_PROCESS_UTILS_H__ 

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_LENGTH 8192  // ensure larger than linux max filename length
#define FILENAME_LENGTH 8192

#define PROC_STATE_ALIVE 0
#define PROC_STATE_NONALIVE 1
#define PROC_STATE_UNKNOWN 2

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


#endif  // __UTILS_PROCESS_UTILS_H__
