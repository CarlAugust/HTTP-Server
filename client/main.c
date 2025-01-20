#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    char* ipv4 = argv[1];
    int port = atoi(argv[2]);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("Socket failed!!!\n");
        close(fd);
        return 0;
    }
    

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, ipv4, &addr_in.sin_addr) <= 0)
    {
        perror("Inet failed");
        close(fd);
        return 0;
    }

    addr_in.sin_port = htons(port);

    
    if (connect(fd, (struct sockaddr * )&addr_in, sizeof(addr_in)) == -1)
    {
        perror("Connection");
        close(fd);
        return 0;
    }

    char buffer[7];
    read(fd, buffer, 7);

    printf("%s", buffer);

    close(fd);
}