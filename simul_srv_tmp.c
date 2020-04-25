#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//#include <libshm.h>
#include "IPC/libshm.h"
//#include <libnetwork.h>
#include "Listes/liblistes.h"

#define KEY_DATA_CC (key_t)400
#define KEY_NBRE_BOT (key_t)987
#define KEY_DATA (key_t)100

#define KEY (key_t)1996

#define SIZE 1024
#define NBRE_MAX_BOT 100 

// l'idée est d'envoyé uen chaine contenant cmd,filename,bot
void send_data(const char *cmd, const char *filename, const char *id_bot)
{
    void *mem_adr;
    getShm(KEY_DATA, SIZE, &mem_adr);
    char to_write[30];
    //to_write = "1,example.so,bot123"
    sprintf(to_write, "%s,%s,%s", cmd, filename, id_bot);
    sprintf((char *)mem_adr, "%s,%s", to_write, (char *)mem_adr);
    //Suppression de la dernière virgule de la chaine de caractère
    size_t taille_chaine = strlen((char*)mem_adr);
    *(char *)(mem_adr + taille_chaine - 1) = 0;
    printf("[Simul]La shm contient: %s \n", (char *)mem_adr);
}

int main()
{

    info_bot_t *array_bots;
    int *nbre_bot;
    lectureShm(KEY_NBRE_BOT, (void *)&nbre_bot, sizeof(int));
    printf("le nbre bots est : %d\n", *nbre_bot);
    lectureShm(KEY, (void *)&array_bots, NBRE_MAX_BOT * sizeof(info_bot_t));

    for (int i = 0; i < *nbre_bot; i++)
    {
        printf("-----------------BOT_INACTIF-------------------------\n");
        printf("\nID : %s \nlife_time: %s \netat: %c\n\n", array_bots[i].ID, array_bots[i].life_time, array_bots[i].etat);
        printf("------------------------------------------\n");
    }

    //Créer une shm de taille FIXE définie à l'avance et très grande

    char *filename = "example.so";
    char *id_bot = "OetiDa";
    char *cmd = "1";
    
    send_data(cmd,filename,id_bot);

    return 0;
}