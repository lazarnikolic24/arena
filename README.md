# C linear memory allocator / arena
Arenas are a simple way to manage memory by allocating large blocks of memory from the OS then using an offset pointer to divide the block into small pieces.

With Arenas, you never free individual allocations. Instead, you destroy the entire arena at once when the memory is no longer needed.

A good way of using arenas is allocating one at the start of a function, using the arena to dynamically allocate memory within the function, and then destroying the arena before returning from the function.

## Pros:
- Avoid memory leaks
- Significantly reduce the malloc/realloc overhead in functions that often resize memory
- Simplify freeing memory in error handling code

## Cons:
- Less flexible than standard malloc/realloc
- You can only resize the most recent allocation within any arena

### A note on memory alignment:
malloc always returns memory suitable for any alignment. Usually this is 8 bytes on 32 bit systems and 16 bytes on 64 bit systems. The Arena_alloc function has the same behaviour by default, but this behaviour is usually undesirable as it will cause lots unnecesary padding within the arena. Instead, you should preffer the Arena_alloc_aligned function which also takes an alignment argument. This argument should be equal to the largest alignment of any struct member, if allocating memory for a struct. The alignment of all primitives is the same as their sizeof.
