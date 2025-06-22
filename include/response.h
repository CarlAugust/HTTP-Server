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
#include <main.h>

#define MAX_RESPONSE_SIZE 10240

// Abstracted send functions
/*
All abstracted functions in this file creates a HTTPResponse object
which is then sent to the client via the response_formatAndSend function
*/

int response_sendFile(char* path);
int response_sendError(uint16_t code);

//--------//

/*
Arguments:
- Pointer to a fully constructed HTTPResponse struct

Parses a HTTPResonse struct object and sends it to the client
This function can be reused to create custom response_send* functions,
and is used in all prebuilt response_send* functions

Note: This functions does not take ownership over the httpResponse
*/ 
int response_sendRaw(HTTPResponse* httpResponse);

#endif