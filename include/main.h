#ifndef MAIN_H
#define MAIN_H

#define MAX_REQUEST_SIZE 2048

#include<utils.h>
#include<response.h>
#include<types.h>
#include<router.h>

typedef struct {
    int server_fd;
    struct sockaddr* addr;
    socklen_t addr_len;
} ServerSocket;


// Creates a ServerSocket struct and binds it to a port
ServerSocket socket_serverSetup(uint16_t port);

int socket_runHttpServer(ServerSocket serverSocket, Router* router);

// Gets the client on the current thread
int get_client_fd();


#endif // !