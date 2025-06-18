#ifndef MEM_MNGR_H
#define MEM_MNGR_H

#include <stdbool.h>

bool _memmngr_register(void* dstruct, void (*destroy_func)(void* dstruct));
void _memmngr_rollback(void);

#endif // MEM_MNGR_H