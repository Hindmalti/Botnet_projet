#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
// port et IP fixes momentanément 
#define PORT_UDP_CLIENT "4242"
#define IP_UDP_CLIENT "127.0.0.1"
/**
 * fct char *create_ID(char *chaine)
 * Fonction de création d'ID unique pour les bots
 *
 * param tableau de chaines de caractère 
 *
 * return tableau de chaines de caractère rempli avec l'ID généré
 */
char *create_ID(char *chaine)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < SIZE_ID; i++)
    {
        int key = rand() % (strlen(charset));
        chaine[i] = charset[key];
    }
    chaine[SIZE_ID - 1] = '\0';
    return chaine;
}

/**
 * fct char *timeLife(clock_t debut, char *temps)
 * Fonction qui calcule le temps de vue des bots
 *
 * param debut : l'instant de lancement du binaire (du bot)
 * param temps : chaine de caractère à remplir avec le temps calculé 
 * return tableau de chaines de caractère rempli avec le temps de vie calculé
 */
char *timeLife(clock_t debut, char *temps)
{
    double time_spent = ((double)(clock() - debut)) / (CLOCKS_PER_SEC / 1000); //on divise par 1000 pour avoir des millisecondes
    snprintf(temps, SIZE_TIME, "%.1f", time_spent);
    temps[SIZE_TIME - 1] = '\0';
    return temps;
}

/**
 *
 * info_bot_t remplissageStructure(info_bot_t structure, clock_t debut)
 * //Fonction qui remplie la structure représentant les infos des bots
 *
 * param strcture : la structure info_bot_t qu'il faudra remplir et retourner
 * param debut : l'instant de lancement du binaire (du bot)
 * 
 * return la strcture remplie
 */


int remplissageStructure(info_bot_t *structure, clock_t debut)
{
    //initialisation de la structure
    char id[SIZE_ID];
    char timeSpent[SIZE_TIME];
    //remplissage
    strcpy(structure->ID, create_ID(id));
    sleep(5);
    strcpy(structure->life_time, timeLife(debut, timeSpent));
    strcpy(structure->etat, BOT_INACTIF);
    return 0;
}

/**
 * void impressionStructure(info_bot_t info_bot)
 * //Fonction qui printf les structures 
 */
void impressionStructure(info_bot_t info_bot)
{
    printf("***************La structure ************ \n");
    printf("L'ID dans la structure est : %s\n", info_bot.ID);
    printf("Le temps de vie dans la structure est : %s milliseconds\n", info_bot.life_time);
    printf("L'état dans la structure est : %c\n", info_bot.etat);
}