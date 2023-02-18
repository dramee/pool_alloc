#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100 * 1024 * 1024
#define BLOCK_SIZE 1024

typedef char byte;

typedef struct {
    void* start;
    void* curr;
} poolAllocator;

void poolAllocInit(poolAllocator* poolAlloc) {
    poolAlloc->start = malloc(MAX_SIZE);
    if (poolAlloc->start == NULL){
        exit(1);
    }
    poolAlloc->curr = poolAlloc->start;
    size_t CHUNKS_AMOUNT = MAX_SIZE / BLOCK_SIZE;
    for (size_t i = 0; i < CHUNKS_AMOUNT - 1; i++) {
        *(void**)((byte*)poolAlloc->start + i * BLOCK_SIZE) = (byte*)poolAlloc->start + (i + 1) * BLOCK_SIZE;
    }
    *(void**)((byte*) poolAlloc->start + (CHUNKS_AMOUNT - 1) * BLOCK_SIZE) = NULL;
}


void* poolAlloc(poolAllocator* poolAlloc) {
    void* ret_ptr = poolAlloc->curr;
    poolAlloc->curr = *(void**)poolAlloc->curr;
    return ret_ptr;
}

void freePoolAlloc(poolAllocator* poolAlloc, void* ptrToFree){
    void* old_ptr = poolAlloc->start;
    poolAlloc->start = ptrToFree;
    *(void**)poolAlloc->start = old_ptr;
}

int main() {
    poolAllocator Alloc;
    poolAllocInit(&Alloc);
    int* testPtr = (int*) poolAlloc(&Alloc);
    *testPtr = 1;
    freePoolAlloc(&Alloc, testPtr);
    return 0;
}
