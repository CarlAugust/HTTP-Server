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
        perror("Could not find in public folder:");
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
    uint32_t matched = sscanf(request, "%15s %431s %63s", httpRequest->method, httpRequest->request_target, httpRequest->http_version);
    if (matched != 3)
    {
        return -1;
    }

    return 0;
}

int reasonFromCode(uint16_t code, char reason[64])
{
    switch(code) {
        case 200:
            strncpy(reason, "OK", 64);
            return 0;
        case 400:
            strncpy(reason, "Bad Request", 64);
            return 0;
        case 403:
            strncpy(reason, "Forbidden", 64);
            return 0;
        case 404:
            strncpy(reason, "Not Found", 64);
            return 0;
        case 500:
            strncpy(reason, "Internal Server Error", 64);
            return 0;
        default:
            strncpy(reason, "Unknown", 64);
            return -1;
    }
}