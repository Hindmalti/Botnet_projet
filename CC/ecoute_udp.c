#include "ecoute_udp.h"
extern liste_bot_t list_bot;
/**
 * int traitementUDP(info_bot_t structure, int taille)
 * Fonction de traitement de la réception UDP, stocke les bots reçus dans une liste chainée
 *
 * param structure du bot qu'elle a reçu
 * param taille de la reception (argument utile pour la librairie Network)
 */
int traitementUDP(void *payload, int taille)
{
    info_bot_t *bot = (info_bot_t *)payload;
    printf("[traitementUDP]Affichage bot recu");
    print_BOT_structure(bot);
    (void)taille;
    //TO DO : Verifier qu'on n'a pas deja le bot avant de l'ajouter
    ajout_tete_bot(&list_bot, bot);
    print_listeBot(list_bot);
    // On compte le nbre de bots disponibles 
    // On écrit le nbre de bots dispo sur la shm pour que le serveur les récupère
    //écrit les ID des bots disponibles sur la shmemory
    //ecritureIDshmem(structure);
    return 0;
}

/**
 * void lancementBoucleServeurUDP(void *s)
 * Fonction wrapper permettant de donner les arguments à  
 * la fct boucleServeurUDP vu que elle est lancée dans un thread
 *
 * param pointeur vers la socket UDP
 * 
 */
void lancementBoucleServeurUDP(void *s)
{
    int socket_udp = *((int *)s);
    //On indique à boucleServeur qu'on s'attend à recevoir des info_bot_t
    boucleServeurUDP(socket_udp, traitementUDP, sizeof(info_bot_t));
}
/**
 * void partie_udp()
 * Fonction wrapper qui initialise le serveur UDP dans un thread
 * 
 */
void partie_udp()
{
    char *port_udp = UDP_PORT_ECOUTE;
    printf("############ Partie UDP ############\n");
    int socket_udp = initialisationServeurUDP(port_udp);
    if (socket_udp < 0)
    {
        fprintf(stderr, "Initialisation du serveur UDP impossible\n");
        exit(-1);
    }

    //Lancement de la boucle d'ecoute UDP dans un thread
    if (lanceThread(lancementBoucleServeurUDP, (void *)&socket_udp, sizeof(socket_udp)) < 0)
    {
        perror("boucleServeurUDP.lanceThread");
        exit(-1);
    }
    //close(socket_udp);
}