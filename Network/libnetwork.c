#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

#include "libnetwork.h"
#include "../Threads/libthrd.h"
#include "../BOT/utils.h"

#define MAX_TCP_CONNEXION 10
#define MAX_UDP_MESSAGE 1024
#define TAILLE_STRUCTURE 32



//Fonction permettant d'envoyer en broadcast un message
/**
 * fct void sendUDPBroadcast(unsigned char *message, int taille_message, int port)
 * Fonction d'envoi de msg en UDP en Broadcast.
 *
 * param message à envoyer en broadcast
 * param taille_message taille du message à envoyer
 * param port : port depuis lequel il envoie (port client)
 * return 
 */

void sendUDPBroadcast(info_bot_t info_bot, int taille_structure, int port)
{
    int broadcast_enable = 1;
    //Option broadcast ON
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if (s < 0)
    {
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1);
    }
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0)
    {
        //Mise option broadcast à la socket
        perror("sendUDPBroadcast.setsockopt");
        exit(-1);
    }

    struct sockaddr_in broadcast_address;
    memset(&broadcast_address, 0, sizeof(broadcast_address));
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_port = htons(port);
    broadcast_address.sin_addr.s_addr = INADDR_BROADCAST; //255.255.255.255
    //remplissageStructure(info_bot,debut);
    taille_structure = TAILLE_STRUCTURE;

    //Envoie de la structure depuis les bots

    if (sendto(s, &info_bot, taille_structure, 0, (struct sockaddr *)&broadcast_address,
               sizeof(broadcast_address)) < 0)
    {
        perror("sendUDPBroadcast.sendto");
        exit(-1);
    }
    close(s);
}

/**
 * fct int initialisationServeur(char *service)
 * Fonction d'initialisation d'un serveur TCP.
 *
 * param service Port sur lequel doit écouter le serveur TCP.
 *
 * return Descripteur de fichier de la socket si aucune erreur, -1 sinon.
 */

int initialisationServeurTCP(char *service)
{
    struct addrinfo precisions, *resultat, *origine;
    int statut;
    int s;

    /*Construction de la structure adresse*/
    memset(&precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_STREAM;
    precisions.ai_flags = AI_PASSIVE;
    statut = getaddrinfo(NULL, service, &precisions, &origine);
    if (statut < 0)
    {
        perror("initialisationServeur.getaddrinfo");
        exit(EXIT_FAILURE);
    }
    struct addrinfo *p;
    for (p = origine, resultat = origine; p != NULL; p = p->ai_next)
        if (p->ai_family == AF_INET6)
        {
            resultat = p;
            break;
        }

    /*Creation d'une socket*/
    s = socket(resultat->ai_family, resultat->ai_socktype, resultat->ai_protocol);
    if (s < 0)
    {
        perror("initialisationServeur.socket");
        exit(EXIT_FAILURE);
    }

    /*Options utiles*/
    int vrai = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &vrai, sizeof(vrai)) < 0)
    {
        perror("initialisationServeur.setsockopt (REUSEADDR)");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &vrai, sizeof(vrai)) < 0)
    {
        perror("initialisationServeur.setsockopt (NODELAY)");
        exit(EXIT_FAILURE);
    }

    /*Specification de l'adresse de la socket*/
    statut = bind(s, resultat->ai_addr, resultat->ai_addrlen);
    if (statut < 0)
        return -1;

    /*Liberation de la structure d'informations*/
    freeaddrinfo(origine);

    /*Taille de la queue d'attentei*/
    statut = listen(s, MAX_TCP_CONNEXION);
    if (statut < 0)
        return -1;
        
    return s;
}

/**
 *  boucleServeur(int ecoute, void (*traitement)(int))
 *  Fonction boucle du serveur gérant les connexions entrantes des clients.
 *
 *  ecoute Socket d'écoute correspondant à la socket TCP bind.
 *  traitement Fonction de traitement de la socket de connexion du client.
 *
 * return 0 si aucune erreur, -1 sinon.
 */

int boucleServeurTCP(int ecoute, void (*traitement)(int))
{

    int dialogue;

    while (1)
    {
        /*Attente d'une connexion*/
        if ((dialogue = accept(ecoute, NULL, NULL)) < 0)
            return -1;
        /*Passage de la socket de dialogue a la fonction de traitement*/
        traitement(dialogue);
    }
    return 0;
}

// Creation d'un serveur UDP
int initialisationServeurUDP(char *service)
{
    struct addrinfo precisions, *resultat, *origine;
    int statut;
    int s;

    /* Construction de la structure adresse */
    memset(&precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_DGRAM;
    precisions.ai_flags = AI_PASSIVE;
    statut = getaddrinfo(NULL, service, &precisions, &origine);
    if (statut < 0)
    {
        perror("initialisationSocketUDP.getaddrinfo");
        exit(EXIT_FAILURE);
    }

    struct addrinfo *p;
    for (p = origine, resultat = origine; p != NULL; p = p->ai_next)
        if (p->ai_family == AF_INET6)
        {
            resultat = p;
            break;
        }

    /* Creation d'une socket */
    s = socket(resultat->ai_family, resultat->ai_socktype, resultat->ai_protocol);
    if (s < 0)
    {
        perror("initialisationSocketUDP.socket");
        exit(EXIT_FAILURE);
    }

    /* Options utiles */
    int vrai = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &vrai, sizeof(vrai)) < 0)
    {
        perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
        exit(-1);
    }

    /* Specification de l'adresse de la socket */
    statut = bind(s, resultat->ai_addr, resultat->ai_addrlen);
    if (statut < 0)
    {
        perror("initialisationServeurUDP.bind");
        exit(-1);
    }

    /* Liberation de la structure d'informations */
    freeaddrinfo(origine);

    return s;
}

/**
 * fct int boucleServeurUDP(int s, int (*traitement)(info_bot_t, int))
 * Fonction d'initialisation d'un serveur UDP ( qui tournera sur le CC).
 * param socket d'écoute udp 
 * param traitement_udp Fonction qui va traiter les requêtes UDP entrantes.
 */
int boucleServeurUDP(int s, int (*traitement_udp)(info_bot_t, int))
{
    while (1)
    {
        struct sockaddr_storage adresse;
        info_bot_t structure;
        socklen_t taille = sizeof(adresse);
        char addresse_string[20];

        int nboctets = recvfrom(s,&structure, sizeof(info_bot_t), 0, (struct sockaddr *)&adresse, &taille);
        if (nboctets < 0)
            return -1;
        //
        inet_ntop(AF_INET, &(adresse.ss_family), addresse_string, 20);
        if (traitement_udp(structure, nboctets) < 0)
        {
            perror("serveurMessages.traitement_udp");
            exit(-1);
        }
    }
    return 0;
}

// Initialise un client TCP, et renvoie la socket de connexion
int openTCPClient(char *hote, int port)
{
    int s;
    struct sockaddr_in adresse;
    socklen_t taille = sizeof adresse;

    // creation socket
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        perror("connexionServeur.socket");
        exit(-1);
    }

    // connexion
    memset(&adresse, 0, sizeof(adresse));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(port);
    adresse.sin_addr.s_addr = inet_addr(hote);
    if (connect(s, (struct sockaddr *)&adresse, taille) < 0)
        return -1;
    return s;
}

// Envoie un message TCP sur une connexion active
void sendTCP(int socket, char *message, int length_message)
{
    if (length_message <= 0)
        return;
    write(socket, message, length_message);
}

// Recois un message TCP sur une connexion active
int receiveTCP(int socket, char *message, int max_length)
{
    return read(socket, message, max_length);
}