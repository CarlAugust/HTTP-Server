#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/socket.h>

int http_response_sendFile(char* path, int client_fd);

#endif