#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

// Struct for parsed http requests
typedef struct {
    char method[16];
    char request_target[432];
    char http_version[64];
} HTTPRequest;


// HTTP response struct
typedef struct {
    char http_version[64];

    uint16_t response_code;
    // Reason corresponds to code
    char response_reason[64];

    char content_type[64];
    uint32_t content_length;
    char* body;
} HTTPResponse;