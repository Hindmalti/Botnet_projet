#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include "libthrd.h"

pthread_mutex_t liste_mutex[MAX_MUTEX]; 

/*** Prototype local ***/
void *_lanceThread(void *arg);


/**
 * int lanceThread(void (*fonction)(void*), void* arg, int taille)
 * Fonction de lancement de thread.
 *
 * Pointeur de fonction de la fonction à executer (dans notre cas la gestion du client par expl).
 * Pointeur générique représentant la donnée à passer au thread (socket du dialogue).
 * Taille de la valeur pointée par arg ( taille de la socket).
 * return 0 si aucune erreur, -1 sinon.
 */
int lanceThread(void (*fonction)(void *), void *arg, int taille) {

    thread_param_t *arguments = (thread_param_t *)malloc(sizeof(thread_param_t));

    if(arguments == NULL){
        perror("lanceThread.malloc");
        return 1;
    }

    arguments->arg = malloc(taille);
    memcpy(arguments->arg, arg, taille);
    arguments->fonction = fonction;

    pthread_t tid;
    pthread_attr_t attr;

    if(pthread_attr_init(&attr) != 0) {
        perror("lanceThread.pthread_attr_init");
        return 1;
    }

    if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("lanceThread.pthread_attr_setdetachstate");
        return 1;
    }

    if(pthread_create(&tid, &attr, _lanceThread, arguments) != 0) {
       perror("lanceThread.pthread_create");
       return 1;
    }

    return 0;

}

// Fonction invisible pour l'utilisateur 
void *_lanceThread(void *arg) {

    thread_param_t *arguments = (thread_param_t *)arg;
    DEBUG_PRINT(("[+] New thread\n"));
    arguments->fonction(arguments->arg);
    free(arguments->arg);
    free(arguments);
    DEBUG_PRINT(("[-] End of thread\n"));
    DEBUG_PRINT(("------------------------------------------------------------\n"));

    return NULL;

}

/**
 * void P(int sem)
 * Fonction qui prend une ressource.
 *
 * sem Indice du mutex dans la liste globale liste_mutex
 *
 * return void
 */
void P(int sem) {

    DEBUG_PRINT(("[+] Mutex %d\n",sem));
    pthread_mutex_lock(&liste_mutex[sem]);

}

/**
 * void V(int sem)
 * Fonction qui libère une ressource.
 *
 *  Indice du mutex dans la liste globale liste_mutex
 *
 * return void
 */
void V(int sem) {

    DEBUG_PRINT(("[-] Mutex %d\n",sem));
    pthread_mutex_unlock(&liste_mutex[sem]);

}