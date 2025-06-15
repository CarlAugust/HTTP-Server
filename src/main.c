#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <handler.h>

#define MAX_CLIENTS 100

void* client_handle(void* arg);

int main(int argc, char* argv[]) {
    
    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    uint16_t port = atoi(argv[1]);
    pthread_t threads[MAX_CLIENTS];
    uint8_t thread_count = 0;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Error when creating socket\n");
        close(fd);
        exit(0);
    }

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(port);

    struct sockaddr* addr = (struct sockaddr *)&addr_in;
    socklen_t addr_len = sizeof(addr_in);

    if(bind(fd, addr, addr_len) == -1)
    {
        perror("Error when binding socket\n");
        close(fd);
        exit(0);
    }

    if(listen(fd, 10) == -1)
    {
        perror("Error when listening");
        close(fd);
        exit(0);
    }

    while(1) {
        int client_fd = accept(fd, addr, &addr_len);
        if(client_fd == -1) {
            perror("Failed while accepting\n");
            continue;
        }

        if (thread_count < MAX_CLIENTS) {
            if(pthread_create(&threads[thread_count++], NULL, client_handle, (void*)&client_fd) != 0)
            {
                perror("Thread creation failed\n");
                close(client_fd);
            }
        } else {
            printf("Thread count max reached, refused connection\n");
            close(client_fd);
        }
    }
}


void* client_handle(void* arg) {
    int client_fd = *(int *)arg;
    char buffer[1024];
    int readval;
    char* message = "Boo\n";

    while((readval = read(client_fd, &buffer, 1024)) > 0) {
        printf("Client: %s", buffer);
        send(client_fd, message, strlen(message), 0);
        memset(buffer, 0, sizeof(buffer));
    }    

    if (readval == 0) 
    {
        printf("Client disconnected\n");
    } else if (readval < 0) {
        perror("Read error\n");
    }

    close(client_fd);
    pthread_exit(NULL);
    return NULL;
}