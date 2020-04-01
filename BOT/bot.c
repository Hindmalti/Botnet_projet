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

void gestionClient(void *s)
{
    printf("[gestionClient]Start\n");
    char msg_recu[80];
    int socket = *((int *)s);
    /* Obtient une structure de fichier */
    FILE *dialogue = fdopen(socket, "a+");
    //ProcessDialog(socket);
    receiveTCP(socket, msg_recu, sizeof(msg_recu));
    printf("[gestionClient]Le msg que j'ai lu de mon client TCP est : %s\n", msg_recu);
    if (dialogue == NULL)
    {
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }
    fclose(dialogue);
    return;
}

/*
Fonction de traitement qui traite la socket

*/
void nouveauClient(int dialogue)
{
    printf("[nouveauClient]Start\n");
    if (lanceThread(gestionClient, (void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
}

// wrapper EnvoieUDPBroadcast

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
    //Msg à envoyer à tout le monde en UDP
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

/*
Problème : "J'ai lancé la boucle serveur TCP" ne s'affiche pas => Car boucleServeurTCP s'execute dans le thread principal du main. Il faudra prévoir
par la suite de mettre en place cette boucle serveur dans un tread à lui tout seul

Programme BOT : 

1/ Lancer une boucleServeur TCP afin de gérer les nouvelles connexion des clients => 1 thread qui lancera ensuite 1 thread par client (prévoir probablement un wrapper)
2/ Lancer une boucle UDP afin d'envoyer les status en broadcast => 1 thread à part (prévoir probablement un wrapper)
3/ Traiter les commandes recues => 1 thread (prévoir un wrapper)

*/