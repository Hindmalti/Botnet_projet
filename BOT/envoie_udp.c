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
 
 /** void EnvoieBroadcast(void *structure)
 * Fonction wrapper pour sendUDPBroadcast
 * param pointeur vers la structure à envoyer en UDP
*/
info_bot_t *bot;

void EnvoieBroadcast()
{
    while (1)
    {
        sendUDPBroadcast((void *)bot, sizeof(info_bot_t), PORT_UDP_CLIENT);
        sleep(5);
    }
}

/**
 * void partie_udp(clock_t debut)
 * Fonction wrapper qui initialise le client UDP dans un thread
 * param debut 
 * 
 */
void partie_udp_BOT(clock_t debut)
{

    printf("############ Partie UDP ############\n");
    bot = (info_bot_t *)malloc(sizeof(info_bot_t));
     // création d'une structure
    remplissageStructure(bot, debut); //remplissage avec l'ID et le temps de vie et état
    impressionStructure(bot);
    // PARTIE UDP
    //Envoie de la structure crée en broadcast UDP
    if (lanceThread(EnvoieBroadcast, (void *)&bot, sizeof(info_bot_t)) < 0)
    {
        perror("EnvoieBroadcast.lanceThread");
        exit(-1);
    }
}