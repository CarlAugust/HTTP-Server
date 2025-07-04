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
#include <router.h>
#include <poll.h>

#define MAX_CLIENTS 100

int server_fd;
volatile sig_atomic_t shutdown_requested = 0;

pthread_key_t client_fd_key;
pthread_once_t key_once = PTHREAD_ONCE_INIT;

void* client_httpHandle(void* arg);
static void make_key();
static void handle_sigint(int sig);

void test(HTTPRequest* httpRequest, HTTPResponse* HTTPResponse)
{
    response_sendFile("/index.html");
}

int main(int argc, char* argv[]) {

    if (argc != 2)
    {
        printf("Not enough arguments\n");
        return 0;
    }

    // Router setup
    Router* router = router_create();
    if(router == NULL)
    {
        return 0;
    };

    router_get(router, "/", test);

    // Signal setup
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Socket setup

    uint16_t port = atoi(argv[1]);

    ServerSocket serverSocket = socket_serverSetup(port);
    socket_runHttpServer(serverSocket, router);

    free(router);
    exit(0);
}

// Return a server file descriptor that is binded to a port 
ServerSocket socket_serverSetup(uint16_t port)
{
    ServerSocket serverSocket;
    serverSocket.server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket.server_fd == -1) {
        perror("Error when creating socket\n");
        close(server_fd);
        exit(0);
    }

    int opt = 1;
    setsockopt(serverSocket.server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = INADDR_ANY;
    addr_in.sin_port = htons(port);

    serverSocket.addr = (struct sockaddr *)&addr_in;
    serverSocket.addr_len = sizeof(addr_in);

    if(bind(serverSocket.server_fd, serverSocket.addr, serverSocket.addr_len) == -1)
    {
        perror("Error when binding socket\n");
        close(serverSocket.server_fd);
        exit(0);
    }
    return serverSocket;
}

// Used specifically to get multible arguments for client_httpHandle
struct client_and_router {
    Router* router;
    int client_fd;
};

int socket_runHttpServer(ServerSocket serverSocket, Router* router)
{
    if(listen(serverSocket.server_fd, 10) == -1)
    {
        perror("Error when listening");
        close(serverSocket.server_fd);
        exit(0);
    }

    pthread_t threads[MAX_CLIENTS];
    uint8_t thread_count = 0;

    while(!shutdown_requested) {
        int client_fd = accept(serverSocket.server_fd, serverSocket.addr, &serverSocket.addr_len);
        if(client_fd == -1) {
            perror("Failed while accepting\n");
            continue;
        }
        if (thread_count < MAX_CLIENTS) {

            struct client_and_router candr;
            candr.client_fd = client_fd;
            candr.router = router;

            if(pthread_create(&threads[thread_count++], NULL, client_httpHandle, (void*)&candr) != 0)
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
    return 0;
}

void* client_httpHandle(void* arg) {
    struct client_and_router* candr = (struct client_and_router*)arg;
    int client_fd = candr->client_fd;

    pthread_once(&key_once, make_key);
    pthread_setspecific(client_fd_key, (void*)(intptr_t)client_fd);
    char request[MAX_REQUEST_SIZE];

    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;

    while(!shutdown_requested) {

        int poll_result = poll(&pfd, 1, 1000);
        if (poll_result == -1)
        {
            perror("Error when poll\n");
            break;
        } else if (poll_result == 0)
        {
            // Timeout
            continue;
        }

        if (pfd.revents && POLLIN)
        {
            // Null terminate to be safe
            int readval = read(client_fd, request, MAX_REQUEST_SIZE);
            if (readval > 0)
            {
                request[readval] = '\0';

                printf("%s\n", request);

                HTTPRequest httpRequest;
                if(parseRequest(request, &httpRequest) == -1)
                {
                    response_sendError(400);
                    continue;
                }
                char path[MAX_PATH_SIZE];

                router_runHandler(candr->router, &httpRequest);
                memset(request, 0, sizeof(request));
            } else if (readval == 0) {
                break;
            } else {
                perror("Error");
                break;
            }

        }
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

