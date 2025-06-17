#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/socket.h>

#define MAX_PATH_SIZE 100
#define MAX_FILE_SIZE 10240
#define MAX_REQUEST_LINE_SIZE 512


// Struct for parsed http requests
typedef struct {
    char method[16];
    char request_target[432];
    char http_version[64];
} HttpRequest;


int resolvePath(char* path, char resolved_path[]);
int sendFile(char* path, int client_fd);

// Returns NULL if invalid request
int parseRequest(char* request, HttpRequest* httpRequest);