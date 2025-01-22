#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
    // Some testing
    // if (argc != 2)
    // {
    //     printf("Not enough arguments\n");
    //     return 0;
    // }
    // Change to commandline argument later
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


        char response[1024] = "HTTP/1.1 200 OK\r\n";
        strcat(response, "Content-Type: text/html; charset=UTF-8\r\n");
        strcat(response, "Content-Length: 256\r\n");
        strcat(response, "\r\n");

        FILE* htmlPtr;

        htmlPtr = fopen("index.html", "r");

        char fileBuffer[256];

        int numRead = fread(fileBuffer, 1, 256, htmlPtr); 
        fclose(htmlPtr);

        strcat(response, fileBuffer);
        send(client_fd, response, strlen(response),  0);

        // char response[512] = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n";

        // strcat(response, charBuffer);
        // printf("The servers response --- \n%s\n\n", response);

        // send(client_fd, response, numRead, 0);

        close(client_fd);
    }

    close(fd);
    return 0;
}