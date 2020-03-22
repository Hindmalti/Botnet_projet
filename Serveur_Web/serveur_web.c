#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libnetwork.h>
#include <libshm.h>
/*
Pour le moment on sait écrire dans une mémoire partagée et lire dessus, la preuve avec un fork 
( un père et un fils qui lisent et écrivent sur la meme zone)
l'idée ici est juste de tester les fonctions de la libshm
TO do : supprimer ce bloc et envoyer des ordres du serveur web (qu'il faudra créer )
et les envoyer sur le CC et les écrire dans une shm pour que le CC les récup et les envoie au BOT
*/
int main()
{
    char parent_message[] = "hello"; 
    char child_message[] = "goodbye";
    void *shmem = create_shared_memory(128);
    lecture_ecriture_shm(shmem, parent_message);

    int pid = fork();

    if (pid == 0)
    {
        printf("Child read: %s\n", shmem);
        lecture_ecriture_shm(shmem, child_message);
        printf("Child wrote: %s\n", shmem);
    }
    else
    {
        printf("Parent read: %s\n", shmem);
        sleep(1);
        printf("After 1s, parent read: %s\n", shmem);
    }
}