#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

typedef struct {
    char key[32];
    char value[32];
} HTTPQuery;


typedef struct {
    char key[32];
    char value[32];
} HTTPParam;

typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_DELETE
} HttpMethod;

/* 
Necessary fields:
    - method
    - request_target, e.g /about /home ect...
    - http_version, most likely http/1.1
*/
typedef struct {
    HttpMethod method;
    char request_target[432];
    char http_version[64];

    HTTPQuery queryList[16];
    uint32_t queryListLength;

    HTTPParam paramList[8];
    uint32_t queryListLength;
} HTTPRequest;


/*
Necessary fields:
    - http_version, most likely http/1.1
    - response_code
    - response_reason, can use reasonFromCode from util.h
    - content_type
    - content_length
    - body
*/
typedef struct {
    char http_version[64];

    uint16_t response_code;
    // Reason corresponds to code
    char response_reason[64];

    char content_type[64];
    uint32_t content_length;
    char* body;
} HTTPResponse;

#endif 