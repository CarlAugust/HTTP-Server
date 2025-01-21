#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    FILE* htmlPtr;

    htmlPtr = fopen("index.html", "r");

    char charBuffer[256];

    fread(charBuffer, 256, 1, htmlPtr);

    printf("%s", charBuffer);
    fclose(htmlPtr);


    // Some testing

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


        char httpBuffer[256];
        read(client_fd, &httpBuffer, 256);
        printf("Read data\n");
        printf("%s\n\n", httpBuffer);

        // 5 since thats when the route begins in a http get request (GET /some route)
        // for (int i = 5; i < 256, httpBuffer[i] != ' '; i++)
        // {

        // }

        // Return a valid http response


        close(client_fd);
    }

    return 0;
}