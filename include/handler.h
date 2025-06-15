#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/socket.h>

#define MAX_PATH_SIZE 100
#define MAX_FILE_SIZE 10240


int resolvePath(char* path, char resolved_path[]);
int sendFile(char* path, int client_fd);