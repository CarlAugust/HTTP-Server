#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <utils.h>
#include <types.h>

#define MAX_RESPONSE_SIZE 10240

int response_sendFile(char* path, int client_fd);
int response_error(int client_fd, uint16_t code);

int reponse_formatAndSend(int client_fd, HTTPResponse* httpResponse);

#endif