#ifndef UTILS_H
#define UTILS_H
#include <time.h>
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

char *create_ID(char *);

char *timeLife(clock_t, char *);
int remplissageStructure(info_bot_t*, clock_t);
void impressionStructure(info_bot_t);
#endif