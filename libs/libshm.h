#ifndef LIBSHM_H
#define LIBSHM_H

#include <stdint.h>

/**** Prototypes ****/
void *create_shared_memory(size_t size);
void *lecture_ecriture_shm(void *shmem, void *msg);

#endif
