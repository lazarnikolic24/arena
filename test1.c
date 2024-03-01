#include "arena.h"
#include <stdio.h>

#define ARENA_DEF 1024

int main(){
    Arena* arena = Arena_create(ARENA_DEF);

    char* c = Arena_alloc(arena, 1);
    int* a = Arena_alloc(arena, sizeof(*a));
    
    *a = 13;
    printf("%d\n", *a);

    Arena_print(arena);

    Arena_destroy(arena);

    return 0;
}
