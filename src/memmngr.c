#include "internal/memmngr.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct memnode {
    void* dstruct;
    void (*destructor)(void* dstruct);
    struct memnode* next;
} *MemNode;

typedef struct memmngr {
    MemNode head;
    size_t size;
} *MemMngr;

static MemMngr memmngr = NULL;

static MemNode memmngr_create_node(void* dstruct, void (*destructor)(void* dstruct)) {
    MemNode new_node = (MemNode) malloc(sizeof (struct memnode));
    if (new_node == NULL) return NULL;

    *new_node = (struct memnode) {.dstruct = dstruct, .destructor = destructor, .next = NULL};
    return new_node;
}

static void memmngr_destructor() {
    if (memmngr == NULL) return;

    MemNode curr = memmngr->head;
    while (curr != NULL) {
        #ifdef DEBUG
        printf("\nDestroying strucuture %p\n", curr->dstruct);
        #endif
        MemNode next = curr->next;

        curr->destructor(curr->dstruct);

        curr = next;
    }

    free(memmngr);
    memmngr = NULL;
}

__attribute__((constructor)) void memmngr_new(void) {
    MemMngr new_memmngr = (MemMngr) malloc(sizeof (struct memmngr));
    if (new_memmngr == NULL) {
        fprintf(stderr, "Error: dstruct internal memory manager cannot be initialized");
        exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    printf("Memory manager succesfully created.\n");
    #endif

    *new_memmngr = (struct memmngr){.head = NULL, .size = 0};
    memmngr = new_memmngr;

    atexit(memmngr_destructor);
} 

#ifdef DEBUG
void memmngr_print() {
    if (memmngr == NULL) {
        printf("NULL");
        return;
    }

    printf("\nStructures registered: ");
    
    MemNode curr = memmngr->head;;
    while (curr != NULL) {
        printf("%p", curr->dstruct);
        printf(" -> ");

        curr = curr->next;
    }
    printf("NULL\n");
    printf("Structures allocated: %lld\n", memmngr->size);
}
#endif

bool memmngr_register(void* dstruct, void (*destructor)(void* dstruct)) {
    if (memmngr == NULL) return false;

    MemNode new_node = memmngr_create_node(dstruct, destructor);
    if (new_node == NULL) return false;

    new_node->next =  memmngr->head;
    memmngr->head = new_node;
    memmngr->size++;

    #ifdef DEBUG
    memmngr_print();
    #endif

    return true;
}

void memmngr_rollback(void) {
    if (memmngr == NULL || memmngr->head == NULL) return;

    memmngr->head->destructor(memmngr->head->dstruct);
    memmngr->head = memmngr->head->next;
}