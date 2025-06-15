#include <handler.h>

#define MAX_PATH_SIZE 100

char* resolvePath(char* path, char resolved_path[])
{
    char base_dir[] = "./public";

    char resolved_public[MAX_PATH_SIZE];
    if (realpath(base_dir, resolved_public) == NULL) {
        perror("Could not resolve public");
        return NULL;
    }

    char path_public[MAX_PATH_SIZE];
    snprintf(path_public, sizeof(path_public), "%s%s", base_dir, path);

    if (realpath(path_public, resolved_path) == NULL) {
        return NULL;
    }

    if (strncmp(resolved_public, resolved_path, strlen(resolved_public)) == 0)
    {
        return resolved_path;
    } else {
        return NULL;
    }
}