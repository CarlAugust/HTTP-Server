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