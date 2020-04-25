#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libthrd.h>
#include "cc.h"

/**
 * void init_socket_bot(bot_t *bot)
 * Fonction qui crée une socket, lui met des options TCP, la connecte à un serveur TCP, 
 * une fois la connexion faite elle lance la fct wrapper qui lance un thread avec l'écriture sur la socket
 * param un structure bot_t
 */
int init_socket_bot(bot_t *bot)
{
    struct sockaddr_in *bot_addr = (struct sockaddr_in *)&(bot->addr);
    bot_addr->sin_port = htons(PORT_TCP_BOT);
    int socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_tcp < 0)
    {
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1);
    }
    if (connect(socket_tcp, (struct sockaddr *)&bot->addr, sizeof(bot->addr)) < 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    return socket_tcp;
}

/**
 * Fonction wrapper à mettre dans un lanceThread, permettant de receptionner les ordres données par le 
 * serveur Web
 * 
 */
void reception_serveur_web(void *arg)
{
    sleep(3);
    (void)arg;
    while (1)
    {
        //On récupère du serveur web les ordres à envoyer aux bots
        getOrdreFromShm();
        sleep(5);
    }
}

/**
 * void partie_tcp()
 * Fonction wrapper qui initialise le client TCP dans un thread
 * 
 */
void partie_tcp()
{
    sleep(10);
    printf("############ Partie TCP ############\n");
    //Reception des ordres du serveur web
    lanceThread(reception_serveur_web, (void *)NULL, 0);
    //Reception des ordres de la CLI admin
    //lanceThread(, (void *)NULL, 0);
}
