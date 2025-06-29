#include <router.h>


int router_create(Router* router)
{
    router = malloc(sizeof(router));
    if (router == NULL)
    {
        return -1;
    }

    router->listLength = 0;
    return 0;
}

static uint8_t router_includes(Router* router, const char* path)
{
    for (uint32_t i = 0; i < router->listLength; i++)
    {
        if (!strcmp(router->list[i].path, path))
        {
            return 1;
        }
    }
    return 0;
}

static int router_add(Router* router, HttpMethod httpMethod, const char* path, RouteHandler routeHandler)
{
    if (router->listLength == MAX_ROUTES)
    {
        printf("Max route capacity reached when adding %s\n", path);
        return -1;
    }

    Route* currentRoute = &router->list[router->listLength];
    if (!router_includes(router, path))
    {
        strncpy(currentRoute->path, path, sizeof(currentRoute->path));
    }

    switch (httpMethod)
    {
        case HTTP_GET:
            if (currentRoute->get != NULL) {
                printf("The handler for GET at %s already exists\n", path);
                return -1;
            }
            currentRoute->get = routeHandler;
            break;
        case HTTP_POST:
            if (currentRoute->post != NULL) {
                printf("The handler for POST at %s already exists\n", path);
                return -1;
            }
            currentRoute->post = routeHandler;
            break;
        case HTTP_DELETE:
            if (currentRoute->delete != NULL) {
                printf("The handler for DELETE at %s already exists\n", path);
                return -1;
            }
            currentRoute->delete = routeHandler;
            break;
        default:
            printf("Router add did not use implemented method");
            return -1;
            break;
    }
    router->listLength++;
    return 0;
}

int router_get(Router* router, const char* path, RouteHandler routeHandler)
{
    return router_add(router, HTTP_GET, path, routeHandler);
}

int router_post(Router* router, const char* path, RouteHandler routeHandler)
{
    return router_add(router, HTTP_POST, path, routeHandler);
}

int router_delete(Router* router, const char* path, RouteHandler routeHandler)
{
    return router_add(router, HTTP_DELETE, path, routeHandler);
}


/*
    I need to be able to compare routes
    Case 1: /about /about, simple strcmp
    Case 2: /user/:id /user/142, need to do a pattern match to check for the pattern
    Case 3: /user /user?id=23, need to check for query paramters
    Case 4: /user /user/, should these be two seperate routes???
    Case 5: /user/* /user/index.html ect, possible implementation to help easier middleware later on

    For case 2 must /user/about still be possible? or does /user/:id override the path that folows user so it must be /user/:id/about?
    For case 3 query parameters always follows the last route

*/

static int router_pathStripQuery(HTTPRequest* httpRequest, const char* path)
{
    return 0;
}

static int router_pathNormalize(const char* path, const char* routerPath)
{
    return 0;
}

static int router_pathMatchTrue(const char* path, const char* routerPath)
{
    if (strcmp(path, routerPath) == 0) return 1;
    return 0;
}

static int router_pathMatchParam(const char* path, const char* routerPath)
{
    return 0;
}

static int router_pathMatchWildcard(const char* path, const char* routerPath)
{
    return 0;
}

static Route* router_pathMatcher(Router* router, HTTPRequest* httpRequest)
{
    for (uint32_t i = 0; i < router->listLength; i++)
    {
        if (router_pathMatchTrue(httpRequest->request_target, router->list[i].path)) return &router->list[i];
    }

    return NULL;
}

int router_runHandler(Router* router, HTTPRequest* httpRequest)
{
    Route* routePtr = router_pathMatcher(router, httpRequest);
    if (routePtr == NULL)
    {
        return -1;
    }

    HTTPResponse* httpResponse = malloc(sizeof(httpResponse));

    switch (httpRequest->method)
    {
        case HTTP_GET:
            routePtr->get(httpRequest, httpResponse);
            break;
        case HTTP_POST:
            routePtr-post(httpRequest, httpResponse);
            break;
        case HTTP_DELETE:
            routePtr->delete(httpRequest, httpResponse);
            break;
        default:
            return -1;
    }

    free(httpResponse);
}