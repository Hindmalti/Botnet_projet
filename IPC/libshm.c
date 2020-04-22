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
#include <sys/types.h>
#include <sys/shm.h>

#include "libshm.h"

int getShm(key_t key, size_t size_shm, void **mem_adr)
{
    printf("[getShm]Start\n");
    int shmid;
    //Création de la shmem
    if ((shmid = shmget(key, size_shm, IPC_CREAT | 0666)) == -1)
    {
        perror("shmget");
        return 1;
    }
    
    //On attache la shmem
    if ((*mem_adr = (void *)shmat(shmid, NULL, 0)) == (void *)(-1))
    {
        perror("shmat");
        *mem_adr = NULL;
        return 1;
    }
    
    return (0);
}

int ecritureShm(void *mem_adr, void *msg, size_t  size)
{

    printf("[ecritureShm]Start\n");
    //printf("[ecritureShm]j'ai recup au début est : %s\n", mem_arr);
    memcpy(mem_adr, msg, size);
    //printf("[ecritureShm]La shared memory contient : %s\n", mem_arr);
    shmdt(mem_adr);
    return 0;
}
void lectureShm(key_t key, void **msg_recu, size_t size)
{

    int shmid;
    void *shared_mem;
    if ((shmid = shmget(key, size, 0666)) == -1)
    {
        perror("shmget");
        exit(2);
    }
    if ((shared_mem = (void *)shmat(shmid, NULL, SHM_RDONLY)) == (void *)(-1))
    {
        perror("shmat");
        exit(2);
    }
    *msg_recu = (char *)shared_mem;
    
    //printf("J'ai lu sur la shm : %s \n", (char *)shared_mem);
    shmdt(shared_mem);
}