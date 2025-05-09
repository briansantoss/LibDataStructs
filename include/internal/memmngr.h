#ifndef MEM_MNGR_H
#define MEM_MNGR_H

#include <stdbool.h>

bool memmngr_register(void* dstruct, void (*destroy_func)(void* dstruct));
void memmngr_rollback(void);

#endif // MEM_MNGR_H