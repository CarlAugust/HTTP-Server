#ifndef MAIN_H
#define MAIN_H

#define MAX_REQUEST_SIZE 2048

#include<utils.h>
#include<response.h>
#include<types.h>

// Gets the client on the current thread
int get_client_fd();

#endif // !