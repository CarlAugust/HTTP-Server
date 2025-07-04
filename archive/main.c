#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

struct Request {
    char* method;
    char* path;
};

struct Route {
    char* path;
    void(* fptr)(int client_fd);
};

struct RouteList {
    struct Route* list;
    int size;
};

int newHttpResponse(char * path, void(*func)(int client_fd));
int readHttpRequest(struct Request* request, int client_fd);
int redirect(char* path); 
int sendFile(char* path, int client_fd);
int errorResponse(int client_fd);
struct Request mapToRequest(char* req);

int routeListPush(struct RouteList* routeList, struct Route route)
{
    routeList->size += 1;
    routeList->list = (struct Route* )realloc(routeList->list, sizeof(struct Route) * routeList->size);
    if (routeList->list == NULL)
    {
        perror("RouteList allocation failed");
        return -1;
    }
    routeList->list[routeList->size - 1] = route;
    return 0;
}

// Global variables

struct RouteList route = {NULL, 0};
struct RouteList* routeListPtr = &route;

void test(int client_fd)
{
    sendFile("/", client_fd);
}

int main(int argc, char* argv[])
{
    newHttpResponse("/", &test);

    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }
    
    uint16_t port = atoi(argv[1]);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("Socket failed!!!\n");
        close(fd);
        return 0;
    }

    printf("Socket success\n");
    
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(port);

    struct sockaddr* addr = (struct sockaddr *)&addr_in;
    socklen_t addr_len = sizeof(addr_in);

    if(bind(fd, addr, addr_len) == -1)
    {
        perror("bind");
        close(fd);
        return 0;
    }

    if(listen(fd, 10) == -1)
    {
        perror("listen");
        close(fd);
        return 0;
    }

    while (1)
    {   
        int client_fd = accept(fd, addr, &addr_len);
        if (client_fd == -1)
        {
            perror("Connection failed");
            continue;
            return 0;
        }

        char requestBuffer[1024];
        read(client_fd, requestBuffer, 1024);
        printf("Response from client ---- \n%s\n\n", requestBuffer);

        struct Request request = mapToRequest(requestBuffer);
        int answer = readHttpRequest(&request, client_fd);

        if (answer == -1)
        {
            printf("Boo\n");
            errorResponse(client_fd);
            close(client_fd);
        }

        close(client_fd);
    }

    close(fd);
    return 0;
}


struct Request mapToRequest(char* req)
{
    struct Request request = {NULL, NULL};

    if (req == NULL)
    {
        return request;
    }

    request.method = strtok(req, " ");
    request.path = strtok(NULL, " ");

    return request;
}

int newHttpResponse(char* path, void(* fptr)(int client_fd))
{
    struct Route route = {"", NULL};

    printf("%s\n", path);

    if (fptr == NULL)
    {
        printf("Function pointer i empty\n");
        return 1;
    }

    if (strcmp(path, "") == 0)
    {
        printf("Path is empty");
        return 1;
    }

    if (path[0] != '/')
    {
        printf("This is not a valid path\n");
        return 1;
    }

    route.fptr = fptr;
    route.path = path;

    int success = routeListPush(routeListPtr, route);
    if (success == -1)
    {
        return -1;
    }

    return 0;
}

int readHttpRequest(struct Request* request, int client_fd)
{
    int routeListSize = routeListPtr->size;
    struct Route* paths = routeListPtr->list;
    for (int i = 0; i < routeListSize; i++)
    {
        if (strcmp(request->path, paths[i].path) == 0)
        {
            paths[i].fptr(client_fd);
            return 0;
        }
    }
    printf("The path %s was requested but was not found\n", request->path);
    return -1;
}

int sendFile(char* path, int client_fd)
{
        FILE* htmlPtr;

        htmlPtr = fopen("./public/index.html", "r");

        char fileBuffer[256];

        int numRead = fread(fileBuffer, 1, 256, htmlPtr); 
        fclose(htmlPtr);

        char length_str[32];
        snprintf(length_str, 31, "Content-Length: %d\r\n", numRead);

        char response[1024] = "HTTP/1.1 200 OK\r\n";
        strcat(response, "Content-Type: text/html; charset=UTF-8\r\n");
        strcat(response, length_str);
        strcat(response, "\r\n");

        strcat(response, fileBuffer);
        send(client_fd, response, strlen(response),  0);
}

int errorResponse(int client_fd)
{
    char* response = "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 0\r\n"
    "Connection: close\r\n"
    "\r\n";
    send(client_fd, response, strlen(response), 0);    
}