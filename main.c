#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <libgen.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    char* link_name = malloc(strlen(argv[1]) + 1);
    strcpy(link_name, argv[1]);

    struct stat sb;

    char* link_target = NULL;
    ssize_t bufsize = 0;
    short retval = 0;
    do {
        if(lstat(link_name, &sb) == -1) {
            perror("lstat");
            retval = 1;
            goto exit;
        }

        if(link_target == NULL) {
            printf("%s", link_name);
        }

        if(!S_ISLNK(sb.st_mode)) {
            break;
        }

        bufsize = sb.st_size != 0 ? sb.st_size + 1 : PATH_MAX;

        link_target = realloc(link_target, bufsize);
        if(link_target == NULL) {
            perror("malloc");
            retval = 1;
            goto exit;
        }

        ssize_t nbytes = readlink(link_name, link_target, bufsize);
        if(nbytes == -1) {
            perror("readlink");
            retval = 1;
            goto exit;
        }

        link_target[nbytes] = '\0';

        printf(" -> %s", link_target);

        if(link_target[0] != '/') {
            char* link_dirname = malloc(strlen(link_name) + 1);
            strcpy(link_dirname, link_name);

            char* dir = dirname(link_dirname);

            ssize_t full_path_len = strlen(dir) + 1 + strlen(link_target) + 1;
            char* full_path = malloc(full_path_len);
            snprintf(full_path, full_path_len, "%s/%s", dir, link_target);

            link_target = realloc(link_target, full_path_len);
            strcpy(link_target, full_path);

            free(full_path);
            free(dir);
        }

        // if(nbytes == bufsize)
        //     printf("(Returned buffer may have been truncated)\n");

        link_name = realloc(link_name, strlen(link_target) + 1);
        strcpy(link_name, link_target);
        link_name[strlen(link_target)] = '\0';
    } while(1);

exit:
    free(link_name);
    free(link_target);

    if(errno == 0) {
        printf("\n");
    }
    return retval;
}
