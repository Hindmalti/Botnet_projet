#include "ecoute_udp.h"
extern liste_bot_t list_bot;
/**
 * int traitementUDP(info_bot_t structure, int taille)
 * Fonction de traitement de la réception UDP, stocke les bots reçus dans une liste chainée
 *
 * param structure du bot qu'elle a reçu
 * param taille de la reception (argument utile pour la librairie Network)
 */
int traitementUDP(struct sockaddr_in *addr, void *payload, int taille)
{
    info_bot_t *bot = (info_bot_t *)payload;
    printf("[traitementUDP]Affichage bot recu\n");
    //print_BOT_structure(bot);
    (void)taille;
    // Verifier qu'on n'a pas deja le bot avant de l'ajouter
    info_bot_t *tmp;
    rechercheBOT(bot->ID, &list_bot, &tmp);
    if (tmp == NULL)
    {
        // On renseigne le bot
        ajout_tete_bot(&list_bot, bot);
    }
    print_listeBot(list_bot);

    // On compte le nbre de bots disponibles
    int nbre_bot = comptageNbreBot(list_bot);
    // on crée une liste
    info_bot_t *array_bots = malloc(nbre_bot * sizeof(info_bot_t));
    llist_bot_to_array(list_bot, array_bots);
    for (int i = 0; i < nbre_bot; i++) {
        printf("------------------------------------------\n");
        printf("\nID : %s \nlife_time: %s \netat: %c\n\n", array_bots[i].ID, array_bots[i].life_time, array_bots[i].etat);
        printf("------------------------------------------\n");
    }
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