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
uid_t proc_get_uid(int32_t pid);

// Check if a process is alive
int proc_alive(int32_t pid);


#endif  // __UTILS_PROCESS_UTILS_H__
