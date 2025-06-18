#include "internal/memmngr.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _memnode {
    void* dstruct;
    void (*destructor)(void* dstruct);
    struct _memnode* next;
} *_MemNode;

typedef struct memmngr {
    _MemNode head;
    size_t size;
} *MemMngr;

static MemMngr _memmngr = NULL;

static _MemNode _memmngr_create_node(void* dstruct, void (*destructor)(void* dstruct)) {
    _MemNode new_node = (_MemNode) malloc(sizeof (struct _memnode));
    if (!new_node) return NULL;

    *new_node = (struct _memnode) {.dstruct = dstruct, .destructor = destructor, .next = NULL};
    return new_node;
}

static void _memmngr_destructor() {
    if (!_memmngr) return;

    _MemNode curr = _memmngr->head;
    while (curr) {
        #ifdef DEBUG
        printf("\nDestroying strucuture %p\n", curr->dstruct);
        #endif
        _MemNode next = curr->next;

        curr->destructor(curr->dstruct);

        curr = next;
    }

    free(_memmngr);
    _memmngr = NULL;
}

__attribute__((constructor)) void _memmngr_new(void) {
    MemMngr new_memmngr = (MemMngr) malloc(sizeof (struct memmngr));
    if (!new_memmngr) {
        fprintf(stderr, "Error: dstruct internal memory manager cannot be initialized");
        exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    printf("Memory manager succesfully created.\n");
    #endif

    *new_memmngr = (struct memmngr){.head = NULL, .size = 0};
    _memmngr = new_memmngr;

    atexit(_memmngr_destructor);
} 

#ifdef DEBUG
void memmngr_print() {
    if (!memmngr) {
        printf("NULL");
        return;
    }

    printf("\nStructures registered: ");
    
    _MemNode curr = memmngr->head;;
    while (curr) {
        printf("%p", curr->dstruct);
        
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
    printf("\nStructures allocated: %lld\n", memmngr->size);
}
#endif

bool _memmngr_register(void* dstruct, void (*destructor)(void* dstruct)) {
    if (!_memmngr) return false;

    _MemNode new_node = _memmngr_create_node(dstruct, destructor);
    if (!new_node) return false;

    new_node->next =  _memmngr->head;
    _memmngr->head = new_node;
    _memmngr->size++;

    #ifdef DEBUG
    memmngr_print();
    #endif

    return true;
}

void _memmngr_rollback(void) {
    if (!_memmngr|| !_memmngr->head) return;

    _memmngr->head->destructor(_memmngr->head->dstruct);
    _memmngr->head = _memmngr->head->next;
}