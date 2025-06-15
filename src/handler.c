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
        return -1;
    }

    if (strncmp(resolved_public, resolved_path, strlen(resolved_public)) == 0)
    {
        return 0;
    } else {
        return -1;
    }
}