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
#include <semaphore.h>

#include "libshm.h"

/** int getShm(key_t key, size_t size_shm, void **mem_adr)
 * Fonction permettant de créer une shared memory
 * param la clé 
 * param taille de la shm
 * param la shm
*/

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
/** int ecritureShm(void *mem_adr, void *msg, size_t  size)
 * Fonction permettant d'écrire sur une shm
 * param la shm sur laquelle écrire
 * param le msg à écrire
 * param la taille du msg à écrire 
*/
int ecritureShm(void *mem_adr, void *msg, size_t  size)
{

    printf("[ecritureShm]Start\n");
    //printf("[ecritureShm]j'ai recup au début est : %s\n", mem_arr);
    memcpy(mem_adr, msg, size);
    //printf("[ecritureShm]La shared memory contient : %s\n", mem_arr);
    //shmdt(mem_adr);
    return 0;
}
/** void lectureShm(key_t key, void **msg_recu, size_t size)
 * Fonction permettant de lire depuis une shm
 * param la clé pour accéder à la bonne shm
 * param un receptacle pour y mettre le msg reçu
 * param la taille du msg à recevoir
 * 
*/
int lectureShm(key_t key, void **msg_recu, size_t size)
{

    int shmid;
    void *shared_mem;
    if ((shmid = shmget(key, size, 0666)) == -1)
    {
        return 1;
    }
    if ((shared_mem = (void *)shmat(shmid, NULL, SHM_RDONLY)) == (void *)(-1))
    {
        perror("shmat");
    }
    *msg_recu = (void *)shared_mem;
    return 0;
    
    //printf("J'ai lu sur la shm : %s \n", (char *)shared_mem);
    // TO DO : penser quand supprimer 
    //shmdt(shared_mem);
}