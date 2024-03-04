#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "arena.h"

#define check_error(cnd, msg)\
    do{\
        if(!(cnd)){\
            if(errno!=0)perror(msg);\
            else fprintf(stderr, "Error: %s\n", (msg));\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define ARENA_DEF 1024

int main(){
    Arena* arena = Arena_create(ARENA_DEF);
    check_error(arena != NULL, "Arena_create");

    char* str = Arena_alloc_aligned(arena, 1, sizeof(*str));
    check_error(str != NULL, "Arena_alloc_aligned");

    *str = '\0';
    size_t len = 0;

    int c;
    while (1){
        c = getchar();
        if (c == EOF) break;

        check_error(Arena_grow(arena, 1) != NULL, "Arena_grow");

        str[len++] = c;
        str[len] = '\0';
    }

    puts(str);

    Arena_print(arena);
    Arena_destroy(arena);
    return 0;
}
