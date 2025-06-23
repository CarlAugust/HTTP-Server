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

    send(client_fd, response, MAX_RESPONSE_SIZE, 0);

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
    fclose(fptr);

    body[file_size] = '\0';
    
    HTTPResponse* httpResponse = malloc(sizeof(HTTPResponse));
    strncpy(httpResponse->http_version, "HTTP/1.1", sizeof(httpResponse->http_version));
    httpResponse->body = body;
    httpResponse->content_length = file_size;
    strncpy(httpResponse->content_type, "text/html", sizeof(httpResponse->content_type));
    httpResponse->response_code = 200;
    reasonFromCode(httpResponse->response_code, httpResponse->response_reason);
    
    response_sendRaw(httpResponse);
    free(httpResponse->body);
    free(httpResponse);

    return 0;
}