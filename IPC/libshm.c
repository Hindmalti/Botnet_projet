#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdint.h>
#include <sys/mman.h>

#include "libshm.h"

void *create_shared_memory(size_t size)
{
    // notre buffer sera disponible en lecture et écriture
    int protection = PROT_READ | PROT_WRITE;

    // Le buffer sera partagé (Les autres process pourront y accéder), mais
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    void *s = mmap(NULL, size, protection, visibility, -1, 0);
    return (s);
}
