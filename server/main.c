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

struct Request mapToRequest(char* req);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }
    
    uint16_t port = 8080;

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

    struct sockaddr * addr = (struct sockaddr *)&addr_in;
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