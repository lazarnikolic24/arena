#include "arena.h"
#include <stdio.h>
#include <string.h>

#define ARENA_DEF 1024

#define SA_ALIGN sizeof(int)
struct sa{
    char c;
    int i;
    short s;
};

int main(){
    Arena* arena = Arena_create(ARENA_DEF);

    char* c = Arena_alloc(arena, 1);
    
//    Arena_print(arena);

    int* a = Arena_alloc_aligned(arena, sizeof(*a), sizeof(*a));
    long long* ll = Arena_alloc(arena, sizeof(*ll));
    
    *c = 17;
    *a = 65535;
    *ll = 345345345;
    printf("a=%d\n", *a);
    printf("ll=%lld\n", *ll);

    char* str = Arena_alloc_aligned(arena, 16, sizeof(*str));
    strcpy(str, "Test string\n");
    printf("%s", str);

    struct sa* sa = Arena_alloc_aligned(arena, sizeof(*sa), SA_ALIGN);
    sa->c = 17;
    sa->i = 252454;
    sa->s = 65534;

    Arena_print(arena);

    Arena_destroy(arena);

    return 0;
}

