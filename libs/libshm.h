#ifndef LIBSHM_H
#define LIBSHM_H

#include <stdint.h>

#define SHM_KEY_CC (key_t)2000
#define SHM_KEY_SERV (key_t)2010


#define SHM_SIZE 30
/**** Prototypes ****/
int getShm(key_t key, size_t size_shm, void **mem_adr);
int ecritureShm(void *mem_adr, void *msg, size_t  size);
int lectureShm(key_t key, void **msg_recu, size_t size);

#endif
