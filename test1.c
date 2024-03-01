#include "arena.h"
#include <stdio.h>

#define ARENA_DEF 1024

int main(){
    Arena* arena = Arena_create(ARENA_DEF);

    char* c = Arena_alloc(arena, 1);
    int* a = Arena_alloc_aligned(arena, sizeof(*a), sizeof(*a));
    long long* ll = Arena_alloc(arena, sizeof(*ll));
    
    *c = 17;
    *a = 65535;
    *ll = 345345345;
    printf("a=%d\n", *a);
    printf("ll=%d\n", *ll);

    Arena_print(arena);

    Arena_destroy(arena);

    return 0;
}

