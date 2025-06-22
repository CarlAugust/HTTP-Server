#include <response.h>


int response_sendRaw(HTTPResponse* httpResponse)
{
    int client_fd = get_client_fd();
    // Worlds largest snprintf???
    char* response = (char*)malloc(MAX_RESPONSE_SIZE);
    snprintf(response, MAX_RESPONSE_SIZE, 
        "%s %i %s\r\n"
        "Content-Type: %s\r\n"
        "Content-length: %i\r\n\r\n"
        "%s",
        httpResponse->http_version,
        httpResponse->response_code,
        httpResponse->response_reason,
        httpResponse->content_type,
        httpResponse->content_length,
        httpResponse->body);
    free(response);
    return 0;
}

int response_sendFile(char* path)
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

    char* body = (char*)malloc(file_size + 1);
    if (body == NULL) {
        fclose(fptr);
        return -1;
    }

    if (fread(body, 1, file_size, fptr) != file_size) {
        perror("Error reading file:");
        free(body);
        fclose(fptr);
        return -1;
    }

    body[file_size] = '\0';
    fclose(fptr);
    free(body);

    return 0;
}