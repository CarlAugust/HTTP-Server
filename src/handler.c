#include <handler.h>

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

int sendFile(char* path, int client_fd)
{
    FILE* fptr;
    fptr = fopen(path, "rb");
    if (fptr == NULL) {
        perror("Something went wrong when opening a file:");
        return -1;
    }

    fseek(fptr, 0, SEEK_END);
    u_int64_t file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(fptr);
        return -1;
    }

    if (fread(buffer, 1, file_size, fptr) != file_size) {
        perror("Something went wrong when reading the file:");
        free(buffer);
        fclose(fptr);
        return -1;
    }

    buffer[file_size] = '\0';
    fclose(fptr);

    send(client_fd, buffer, file_size, 0);
    free(buffer);

    return 0;
}