#ifndef ROUTER_H
#define ROUTER_H

#include <types.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROUTES 256

/*
    Arguments:
        - httpRequest
        - httpReponse, does not take ownership
*/
typedef void (*RouteHandler)(HTTPRequest* httpRequest, HTTPResponse* httpResponse);

/*
    - path, used as a identifier for each route
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

Router* router_create();

int router_runHandler(Router* router, HTTPRequest* httpRequest);
int router_get(Router* router, const char* path, RouteHandler routeHandler);
int router_post(Router* router, const char* path, RouteHandler routeHandler);
int router_delete(Router* router, const char* path, RouteHandler routeHandler);


#endif