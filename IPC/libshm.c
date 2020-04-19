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

/**
 * void *create_shared_memory(size_t size)
 * Fonction de création d'une shared_memory.
 *
 * Taille de la mémoire partagée ( taille de notre ordre par expl).
 * return un pointeur de la zone mémoire partagée.
 */
/* void *create_shared_memory(size_t size)
{
    // notre buffer sera disponible en lecture et écriture
    int protection = PROT_READ | PROT_WRITE;

    // Le buffer sera partagé (Les autres process pourront y accéder)
    // Donc seulement ce process et son fils peuvent l'utiliser

    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    void *s = mmap(NULL, size, protection, visibility, -1, 0);
    if (s == MAP_FAILED)
    {
        perror("create_shared_memory.mmap");
        exit(1);
    }

    return (s);
} */

/**
 * void *lecture_ecriture_shm(void *shmem, void *msg)
 * Fonction d'écriture / lecture dans une zone de mémoire partagée 
 * 
 * Pointeur vers une zone de mémoire partagée précise
 * Le msg à écrire / lire  ( ici on envoie/lit un char mais à voir plus tard).
 * return pointeur de l'adresse de destination 
 * (ici msg écrit ou lu sur la zone de mémoire partagée).
 */
//Fonction permettant d'écrire et lire dans une shm
/* void *lecture_ecriture_shm(void *shmem, void *msg)
{
    return memcpy(shmem, msg, strlen(msg));
} */

void *create_shared_memory()
{

    int shmid;
    char *shm; //*s;
    /*
     * Create the segment.
     */
    if ((shmid = shmget(SHM_KEY_ID, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) < 0)
    {
        perror("shmget");
        exit(2);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    printf(" Le contenu de la shared mem est :%s\n ", shm);
    return (shm);

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    /* 
    while (*shm != '*')
        sleep(1);

    exit(0); */
}
void *ecritureShm(void *key, void *msg)
{
    //TO DO : Envoyer un signal comme quoi on prend la ressource
    printf("[ecritureShm]Start\n");
    return (memcpy(key, msg, strlen(msg)));
}
void *lectureShm(key_t key)
{
    int shmid;
    char *shm; // *s;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHM_SIZE, 0666)) < 0)
    {
        perror("shmget");
        exit(2);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, SHM_RDONLY)) == (char *)-1)
    {
        perror("shmat");
        exit(2);
    }
    return shm;

    /*
     * Now read what the server put in the memory.
     */
    /* 
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n'); */

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    /* 
    *shm = '*';

    exit(0); */
}