#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>
#include "bot.h"

liste_cu_t list_CU;

int main()
{
    clock_t debut = clock(); //prise de temps au moment du démarrage stricte du process
    srand(time(NULL));       //Initialisation nécessaire à faire une seule fois pour la fonction rand

    // PARTIE envoie UDP en THREAD
    partie_udp_BOT(debut);

    // PARTIE SERVEUR TCP en THREAD
    init_listCU(&list_CU);
    partie_tcp_BOT();

    while (1)
    {
    }
    return 0;
}
