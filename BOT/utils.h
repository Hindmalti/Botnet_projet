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
    char *ip; // IP du bot
    char *port; //Port depuis lequel le bot émet 
} info_bot_t;

char *create_ID(char *str);

char *timeLife(clock_t debut, char *temps);
//char* get_ip_bot();
info_bot_t remplissageStructure(info_bot_t structure, clock_t debut);
void impressionStructure(info_bot_t info_bot);