#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/socket.h>
#include <types.h>

#define MAX_PATH_SIZE 100
#define MAX_FILE_SIZE 10240
#define MAX_REQUEST_LINE_SIZE 512

int resolvePath(char* path, char resolved_path[]);

// Returns NULL if invalid request
int parseRequest(char* request, HTTPRequest* httpRequest);

// Get the status code message/reason from the code for HTTPResponse struct
int reasonFromCode(uint16_t code, char* reason);

#endif // !