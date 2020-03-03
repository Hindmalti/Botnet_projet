#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/udp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_TCP_CONNEXION 10
#define BUFFER_SIZE 1024

//Fonction permettant d'envoyer en broadcast un message
void sendUDPBroadcast(unsigned char *message, int taille_message, int port)
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

    //Envoie du message depuis les bots
    if (sendto(s, message, taille_message, 0, (struct sockaddr *)&broadcast_address,
               sizeof(broadcast_address)) < 0)
    {
        perror("sendUDPBroadcast.sendto");
        exit(-1);
    }
    close(s);
}

// Retourne l'adresse ip d'une connexion active
static int socketVersNomUDP(struct sockaddr *padresse, char *nom)
{
    void *ip;
    int taille_nom;
    if (padresse->sa_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)padresse;
        ip = (void *)&ipv4->sin_addr;
        taille_nom = INET_ADDRSTRLEN;
    }
    if (padresse->sa_family == AF_INET6)
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)padresse;
        ip = (void *)&ipv6->sin6_addr;
        taille_nom = INET6_ADDRSTRLEN;
    }
    char nom_deco[20];
    inet_ntop(padresse->sa_family, ip, nom_deco, taille_nom);
    sprintf(nom, "%s", nom_deco + 7);
    return 0;
}
static int socketVersNomTCP(int s, char *nom)
{
    struct sockaddr_storage adresse;
    struct sockaddr *padresse = (struct sockaddr *)&adresse;
    socklen_t taille = sizeof adresse;
    int statut;

    /* Recupere l'adresse de la socket distante */
    statut = getpeername(s, padresse, &taille);
    if (statut < 0)
    {
        perror("socketVersNom.getpeername");
        return -1;
    }

    /* Recupere le nom de la machine */
    return socketVersNomUDP(padresse, nom);
}

// Fonction permettant de créer le serveur TCP
int initialisationServeurTCP(char *service)
{
    struct addrinfo precisions, *resultat, *origine;
    int statut;
    int s;

    /* Construction de la structure adresse */
    memset(&precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_STREAM;
    precisions.ai_flags = AI_PASSIVE;
    statut = getaddrinfo(NULL, service, &precisions, &origine);
    if (statut < 0)
    {
        perror("initialisationSocketUDP.getaddrinfo");
        exit(EXIT_FAILURE);
    }
    struct addrinfo *p;
    for (p = origine, resultat = origine; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET6)
        {
            resultat = p;
            break;
        }
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

    /* Taille de la queue d'attente */
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

int boucleServeurTCP(int ecoute, void (*traitement)(int)) {

    int dialogue;

    while(1) {
        /*Attente d'une connexion*/
        if((dialogue = accept(ecoute, NULL, NULL)) < 0) return -1;
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

// Reception des messages UDP et execute la fonction passee en argument
int boucleServeurUDP(int s, void (*traitement)(unsigned char *, int, char *))
{
    while (1)
    {
        struct sockaddr_storage adresse;
        struct sockaddr *padresse = (struct sockaddr *)&adresse;
        socklen_t taille = sizeof(adresse);
        unsigned char message[BUFFER_SIZE];

        int nboctets = recvfrom(s, message, BUFFER_SIZE, 0, (struct sockaddr *)padresse, &taille);
        if (nboctets < 0)
            return -1;

        /* Recupere le nom de la machine */
        char char_ip[20];
        int status = socketVersNomUDP(padresse, char_ip);
        if (status < 0)
            perror("socketVersNom");

        traitement(message, nboctets, char_ip);
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