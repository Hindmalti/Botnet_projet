#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE_ID 7
#define SIZE_TIME 5
#define ETAT_ACTIF '1'
#define ETAT_INACTIF '0'

typedef struct
{
    char ID[SIZE_ID];          //ID sur 6 octets
    char life_time[SIZE_TIME]; // le temps de vie du bot depuis son existence 4 octets
    char etat;                 // état du bot actif/inactif
} info_bot_t;

char *create_ID(char *str)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < SIZE_ID; i++)
    {
        int key = rand() % (strlen(charset));
        str[i] = charset[key];
    }
    str[SIZE_ID - 1] = '\0';
    return str;
}

char *timeLife(clock_t debut, char *temps)
{
    double time_spent = ((double)(clock() - debut)) / (CLOCKS_PER_SEC / 1000);
    snprintf(temps, SIZE_TIME, "%.1f", time_spent);
    temps[SIZE_TIME - 1] = '\0';
    return temps;
}
//Fonction qui remplie la structure
// https://stackoverflow.com/questions/9653072/return-a-struct-from-a-function-in-c
info_bot_t remplissageStructure(info_bot_t structure, clock_t debut)
{
    //initialisation de la structure
    char id[SIZE_ID];
    char timeSpent[SIZE_TIME];
    char etat = ETAT_ACTIF;
    //remplissage
    strcpy(structure.ID, create_ID(id));
    sleep(5);
    strcpy(structure.life_time, timeLife(debut, timeSpent));
    structure.etat = etat;
    return structure;
}
int main()
{
    clock_t debut = clock(); //prise de temps au moment du démarrage du process
    srand(time(NULL)); //Initialisation nécessaire à faire une seule fois pour la fct rand

    //renvoiStatusAuCC(socket, debut);
    info_bot_t info_bot;
    info_bot = remplissageStructure(info_bot, debut);
    //impression de la structure
    printf("***************La structure ************ \n");
    printf("L'ID dans la structure est : %s\n", info_bot.ID);
    printf("Le temps de vie dans la structure est : %s milliseconds\n", info_bot.life_time);
    printf("L'état dans la structure est : %c\n", info_bot.etat);

    return 0;
}

