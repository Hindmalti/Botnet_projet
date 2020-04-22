
#include "cc.h"

liste_bot_t list_bot;

/**
 * Fonction permettant de parcourir la liste des bots disponibles et 
 * nous renvoie le nombre de bots => elle servira au serveur Web pour qu'il crée des boutons ID
 * param Pointeur vers la liste
 * return nombre 
 */
int comptageNbreBot(liste_bot_t list)
{
    printf("[ComptageNmbreBot]Start\n");
    int nbre = 0;
    node_bot_t *current = list;
    while (current != NULL)
    {
        nbre++;
        current = (liste_bot_t)current->next;
    }
    printf("Le nombre de Bots présents sur la liste est : %d\n", nbre);
    return nbre;
}

int llist_bot_to_array(liste_bot_t list, info_bot_t *returned_array) 
{
    printf("creation[creation_table_bot]Start\n");
    int i = 0;
    // TO DO : réfléchir à où free ce malloc chaque 10 sc ?
    //info_bot_t *tmp = (info_bot_t *)malloc(nbre * sizeof(info_bot_t));
    node_bot_t *current = list;
    while(current != NULL) {
        strcpy(returned_array[i].ID, current->bot->ID);
        returned_array[i].etat = current->bot->etat;
        strcpy(returned_array[i].life_time, current->bot->life_time);
        i++;
        current = (liste_bot_t)current->next;
    }
    
    return 0;
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