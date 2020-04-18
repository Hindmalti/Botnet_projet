#ifndef UTILS_H
#define UTILS_H
#include <time.h>
#define SIZE_ID 7
#define SIZE_TIME 5
#define ETAT_ACTIF '1'
#define ETAT_INACTIF '0'
#define SIZE_FILE 1000
typedef struct
{
    char ID[SIZE_ID];          //ID sur 6 octets
    char life_time[SIZE_TIME]; // le temps de vie du bot depuis son existence 4 octets
    char etat;                 // état du bot actif/inactif
} info_bot_t;


typedef struct {
    void *plugin;
    char *nom;
    //struct sockaddr_in addresse_cc;
    int resultat;//0 si ça s'est bien passé, autre 
    char executed; //0 si non executée, 1 sinon. A la fin de l'execution, on met le résultat dans resultat et on passe l'exec à 1
    
} charge_utile_t;

char *create_ID(char *);
char *timeLife(clock_t, char *);
int remplissageStructure(info_bot_t *, clock_t);
void impressionStructure(info_bot_t);
#endif