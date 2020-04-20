
#include "cc.h"


liste_bot_t list_bot;

/** void ecritureIDshmem(info_bot_t *bot)
 *  Fonction permettant d'écrire dans la shared memory l'ID des bots 
 *  afin que le serveur web puisse savoir quels sont les bots présents
 *  param pointeur vers le bot
 */
// void ecritureIDshmem(info_bot_t *bot)
// {

//     char *shmem = (char *)create_shared_memory(sizeof(char));
//     //ecritureShm(shmem, bot->ID);
//     printf("La shared memory contient : %s\n", shmem);
// }

/**
 * Fonction permettant de parcourir la liste des bots disponibles et 
 * nous renvoie le nombre de bots => elle servira au serveur Web pour qu'il crée des boutons ID
 * param Pointeur vers la liste
 * return nombre 
 */
int comptageNbreBot(liste_bot_t *list)
{
    printf("[ComptageNmbreBot]Start\n");
    int nbre = 0;

    while (list != NULL)
    {
        nbre++;
    }
    printf("Le nombre de Bots présents sur la liste est : %d\n", nbre);
    return nbre;
}

/** void ecriturePIDshm(void *shm)
 *  Fonction permettant de récupérer le pid du process et l'ecrire sur une shm
 *  param adresse d'une shm
 */
// void ecriturePIDshm(void *shm)
// {
//     printf("[ecriturePIDshm]Start\n");
//     const size_t max_pid_len = 12; // Could be system dependent.
//     int pid = getpid();
//     char *char_pid = malloc(max_pid_len + 1);
//     snprintf(char_pid, max_pid_len, "%d", pid);
//     lecture_ecriture_shm(&shm, char_pid);
// }


int main()
{
    //PARTIE Client TCP dans un THREAD
    //partie_tcp();
    // PARTIE SERVEUR UDP (écoute) dans un THREAD
    init_listbot(&list_bot);
    partie_udp();
    //while true pour ne pas sortir du main et laisser
    //le temps aux fct de faire des threads etc
    while (1)
    {
    } 
    return 0;
}