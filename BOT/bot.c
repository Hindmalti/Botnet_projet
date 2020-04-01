#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libthrd.h>
#include <libnetwork.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#define PORT_UDP_CLIENT 4242
#define PORT_UDP_SERVEUR 4242
#define PORT_TCP_CLIENT 2020
#define PORT_TCP_SERVEUR "2020"
#define TAILLE_STRUCTURE 32

extern clock_t debut;

/**
 * void gestionClientTCP(void *s)
 * Fonction qui reçoi le msg envoyée par le CC (qui deviendra un ordre plus tard)
 * elle le reçoit et le printf
 * param pointeur vers la socket TCP
 */

void gestionClientTCP(void *s)
{
    printf("[gestionClientTCP]Start\n");
    char msg_recu[80];
    int socket = *((int *)s);
    /* Obtient une structure de fichier */
    FILE *dialogue = fdopen(socket, "a+");
    //ProcessDialog(socket);
    receiveTCP(socket, msg_recu, sizeof(msg_recu));
    printf("[gestionClientTCP]Le msg que j'ai lu de mon client TCP est : %s\n", msg_recu);
    if (dialogue == NULL)
    {
        perror("gestionClientTCP.fdopen");
        exit(EXIT_FAILURE);
    }
    fclose(dialogue);
    return;
}

/**
 * void nouveauClient(int dialogue)
 * Fonction wrapper qui lance un thread avec la fct de traitement TCP
 * param socket de dialogue 
*/
void nouveauClient(int dialogue)
{
    printf("[nouveauClient]Start\n");
    if (lanceThread(gestionClientTCP, (void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
}
/**
 * void EnvoieBroadcast(void *structure)
 * Fonction wrapper pour sendUDPBroadcast
 * param pointeur vers la structure à envoyer en UDP
*/

void EnvoieBroadcast(void *structure)
{
    while (1)
    {  
        sendUDPBroadcast(*((info_bot_t *)structure), sizeof(info_bot_t), PORT_UDP_CLIENT);
        sleep(5);
    }
}

int main()
{
    clock_t debut = clock(); //prise de temps au moment du démarrage stricte du process
    srand(time(NULL));       //Initialisation nécessaire à faire une seule fois pour la fonction rand

    info_bot_t info_bot;                              // création d'une structure
    remplissageStructure(&info_bot, debut); //remplissage avec l'ID et le temps de vie et état
    impressionStructure(info_bot);
    // PARTIE UDP
    //Envoie de la structure crée en broadcast UDP
    if (lanceThread(EnvoieBroadcast, (void *)&info_bot, sizeof(info_bot_t)) < 0)
    {
        perror("EnvoieBroadcast.lanceThread");
        exit(-1);
    }
    while(1){}

     /* // PARTIE SERVEUR TCP
    char port_s[6] = PORT_TCP_SERVEUR;
    int socket_tcp;
    // Initialisation du serveur 
    socket_tcp = initialisationServeurTCP(port_s);
    if (socket_tcp < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }
    
    printf("J'ai bien initialisé la socket du serveur TCP\n");

    //TO DO : à mettre dans un thread
    //Lancement de la boucle d'ecoute 
    if (boucleServeurTCP(socket_tcp, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    } 
 */
    return 0;
}