#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cc.h"

liste_bot_t list_bot;


/** int comptageNbreBot(liste_bot_t list)
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
/** int llist_bot_to_array(liste_bot_t list, info_bot_t *returned_array) 
 *  Fonction permettant de créer un tableau de structures info_bot afin de l'écrire sur la shm
 * (vu que l'on ne peut pas écrire une liste chainée sur la shm)
 * param la liste depuis laquelle elle convertit
 * param le tableau à remplir
 * 
 */

int llist_bot_to_array(liste_bot_t list, info_bot_t *returned_array) 
{
    printf("creation[creation_table_bot]Start\n");
    int i = 0;
    // TO DO : réfléchir à où free ce malloc chaque 10 sc ?
    //info_bot_t *tmp = (info_bot_t *)malloc(nbre * sizeof(info_bot_t));
    node_bot_t *current = list;
    while(current != NULL) {
        strcpy(returned_array[i].ID, current->bot->info->ID);
        returned_array[i].etat = current->bot->info->etat;
        strcpy(returned_array[i].life_time, current->bot->info->life_time);
        i++;
        current = (liste_bot_t)current->next;
    }
    
    return 0;
}


int main()
{
    
    // PARTIE SERVEUR UDP (écoute) dans un THREAD
    init_listbot(&list_bot);
    partie_udp();
    // PARTIE Client TCP dans un THREAD
    partie_tcp();
    //while true pour ne pas sortir du main et laisser
    //le temps aux fct de faire des threads etc
    while (1)
    {
    }
    return 0;
}