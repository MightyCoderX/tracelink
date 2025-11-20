#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    char* file = malloc(strlen(argv[1]));
    strcpy(file, argv[1]);
    struct stat sb;

    printf("%s", file);

    char* buf;

    do {
        if(lstat(file, &sb) == -1) {
            // assume we finished if file doesn't exist
            // so don't print error
            // TODO: actually check using path of previous link
            // if file is in the same directory
            if(errno != ENOENT)
                perror("lstat");
            return 1;
        }

        if(!S_ISLNK(sb.st_mode)) {
            break;
        }

        ssize_t bufsize = sb.st_size != 0 ? sb.st_size + 1 : PATH_MAX;

        buf = malloc(bufsize);
        if(buf == NULL) {
            perror("malloc");
            return 1;
        }

        ssize_t nbytes = readlink(file, buf, bufsize);
        if(nbytes == -1) {
            perror("readlink");
            return 1;
        }

        buf[nbytes] = '\0';

        printf(" -> %s", buf);

        if(nbytes == bufsize)
            printf("(Returned buffer may have been truncated)\n");

        file = malloc(bufsize);
        strcpy(file, buf);
        file[nbytes] = '\0';
    } while(1);

    free(file);
    free(buf);

    printf("\n");
    return 0;
}
