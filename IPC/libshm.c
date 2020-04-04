
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

/**
 * void *create_shared_memory(size_t size)
 * Fonction de création d'une shared_memory.
 *
 * Taille de la mémoire partagée ( taille de notre ordre par expl).
 * return un pointeur de la zone mémoire partagée.
 */
void *create_shared_memory(size_t size)
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
}

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
void *lecture_ecriture_shm(void *shmem, void *msg)
{
    return memcpy(shmem, msg, strlen(msg));
}