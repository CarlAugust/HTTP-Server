#ifndef ROUTER_H
#define ROUTER_H

#include <types.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROUTES 256


typedef void (*RouteHandler)(HTTPRequest* httpRequest, HTTPResponse* httpResponse);

/*
    - path, might be removed as it will be used as a key for a hashmap
    - get, function for GET method
    - post, function for POST method
    - delete, function for DELETE method
*/
typedef struct Route {
    char path[432];

    RouteHandler get;
    RouteHandler post;
    RouteHandler delete;

} Route;

typedef struct Router {

    // Hardcoded to 256 to avoid dynamic allocation for now
    Route list[MAX_ROUTES];
    uint32_t listLength;
} Router;

typedef enum {
    HTTP_GET,
    HTTP_POST,
    HTTP_DELETE
} HttpMethod;

#endif