#include <utils.h>

int resolvePath(char* path, char resolved_path[])
{
    char base_dir[] = "./public";

    char resolved_public[MAX_PATH_SIZE];
    if (realpath(base_dir, resolved_public) == NULL) {
        perror("Could not resolve public");
        return -1;
    }

    char path_public[MAX_PATH_SIZE];
    snprintf(path_public, sizeof(path_public), "%s%s", base_dir, path);

    if (realpath(path_public, resolved_path) == 0) {
        perror("Could not resolve public folder:");
        return -1;
    }

    if (strncmp(resolved_public, resolved_path, strlen(resolved_public)) == 0)
    {
        return 0;
    } else {
        perror("Requested path is not in the public directory:");
        return -1;
    }
}

int parseRequest(char* request, HTTPRequest* httpRequest) {

    char requestLine[MAX_REQUEST_LINE_SIZE];
    snprintf(requestLine, MAX_REQUEST_LINE_SIZE, "%s", request);

    // Tokenize
    char* token = strtok(requestLine, " ");
    if (!token) return -1;
    snprintf(httpRequest->method, sizeof(httpRequest->method), "%s", token);

    token = strtok(NULL, " ");
    if (!token) return -1;
    snprintf(httpRequest->request_target, sizeof(httpRequest->request_target), "%s", token);

    token = strtok(NULL, "\r\n");
    if (!token) return -1;
    snprintf(httpRequest->http_version, sizeof(httpRequest->http_version), "%s", token);

    return 0;
}

int reasonFromCode(uint16_t code, char* reason)
{
    switch(code) {
        case 200:
            strcpy(reason, "OK");
            return 0;
        case 400:
            strcpy(reason, "Bad Request");
            return 0;
        case 403:
            strcpy(reason, "Forbidden");
            return 0;
        case 404:
            strcpy(reason, "Not Found");
            return 0;
        case 500:
            strcpy(reason, "Internal Server Error");
            return 0;
        default:
            strcpy(reason, "Unknown");
            return -1;
    }
}