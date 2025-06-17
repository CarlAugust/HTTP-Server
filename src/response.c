#include <response.h>

int http_response_sendFile(char* path, int client_fd)
{
    FILE* fptr;
    fptr = fopen(path, "rb");
    if (fptr == NULL) {
        perror("Error opening file:");
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
        perror("Error reading file:");
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