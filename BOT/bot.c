#include "bot.h"

info_bot_t *bot;
liste_cu_t list_CU;

int main()
{
    //clock_t debut = clock(); //prise de temps au moment du démarrage stricte du process
    //srand(time(NULL));       //Initialisation nécessaire à faire une seule fois pour la fonction rand

    // PARTIE envoie UDP en THREAD
    //partie_udp(debut);

    // PARTIE SERVEUR TCP en THREAD
    // init_listCU(&list_CU);
    partie_tcp();

    while (1)
    {
    }
    return 0;
}
