#ifndef LIBSHM_H
#define LIBSHM_H

#include <stdint.h>
#define SHM_KEY_ID (key_t)2000
#define SHM_KEY_DATA (key_t)2010
#define SHM_KEY_PID (key_t)2020
#define SHM_FLG
#define SHM_SIZE 30
/**** Prototypes ****/
void *create_shared_memory();
void *lectureShm(key_t key);
void *ecritureShm(void *key, void *msg);

#endif
