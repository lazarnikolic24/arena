#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "arena.h"

#define CHECK_ERROR(cnd, msg)\
    do{\
        if(!(cnd)){\
            if(errno!=0)perror(msg);\
            else fprintf(stderr, "Error: %s\n", (msg));\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

#define ARENA_DEF 4

int main(){
    Arena_ptr arena = Arena_create(ARENA_DEF);
    CHECK_ERROR(arena != NULL, "Arena_create");

    char* str = Arena_alloc_aligned(arena, 1, sizeof(*str));
    CHECK_ERROR(str != NULL, "Arena_alloc_aligned");

    *str = '\0';
    size_t len = 0;

    int c;
    while (1){
        c = getchar();
        if (c == EOF) break;

        void* new = Arena_grow_move(arena, str, 1);
        CHECK_ERROR(new != NULL, "Arena_grow_move");
        str = new;

        str[len++] = c;
        str[len] = '\0';
    }

    printf("\n");
    puts(str);

    Arena_print(arena);
    Arena_destroy(arena);
    return 0;
}
