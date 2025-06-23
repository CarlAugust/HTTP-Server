#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>
#include <main.h>
#include <utils.h>
#include <response.h>

#define MAX_CLIENTS 100

int server_fd;
volatile sig_atomic_t shutdown_requested = 0;

pthread_key_t client_fd_key;
pthread_once_t key_once = PTHREAD_ONCE_INIT;



void* client_handle(void* arg);
static void make_key();
static void handle_sigint(int sig);

int main(int argc, char* argv[]) {

    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    // Signal setup
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Socket setup

    uint16_t port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error when creating socket\n");
        close(server_fd);
        exit(0);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(port);

    struct sockaddr* addr = (struct sockaddr *)&addr_in;
    socklen_t addr_len = sizeof(addr_in);

    if(bind(server_fd, addr, addr_len) == -1)
    {
        perror("Error when binding socket\n");
        close(server_fd);
        exit(0);
    }

    if(listen(server_fd, 10) == -1)
    {
        perror("Error when listening");
        close(server_fd);
        exit(0);
    }

    pthread_t threads[MAX_CLIENTS];
    uint8_t thread_count = 0;

    while(!shutdown_requested) {
        int client_fd = accept(server_fd, addr, &addr_len);
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

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    exit(0);
}


void* client_handle(void* arg) {
    int client_fd = *(int *)arg;
    pthread_once(&key_once, make_key);
    pthread_setspecific(client_fd_key, (void*)(intptr_t)client_fd);
    char request[MAX_REQUEST_SIZE];
    int readval;

    while((readval = read(client_fd, &request, MAX_REQUEST_SIZE - 1)) > 0 && !shutdown_requested) {
        // Null terminate to be safe
        request[readval] = '\0';

        printf("%s\n", request);

        HTTPRequest httpRequest;
        if(parseRequest(request, &httpRequest) == -1)
        {
            continue;
        }
        char path[MAX_PATH_SIZE];
        resolvePath("/index.html", path);

        response_sendFile(path);
        memset(request, 0, sizeof(request));
    }    

    if (readval == 0) {
        printf("Client disconnected\n");
    } else if (shutdown_requested) {
        printf("SIGNINT told the thread to stop\n");
    } else if (readval < 0) {
        perror("Read error\n");
    }

    close(client_fd);
    pthread_exit(NULL);
    return NULL;
}

static void make_key() {
    pthread_key_create(&client_fd_key, NULL);
}

static void handle_sigint(int sig)
{
    shutdown_requested = 1;
    shutdown(server_fd, SHUT_RDWR);
    close(server_fd);
}

int get_client_fd() {
    return (int)(intptr_t)pthread_getspecific(client_fd_key);
}

