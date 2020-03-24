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

#define UDP_PORT_ECOUTE "4242"
#define IP_BOT "127.0.0.1"
#define PORT_TCP_BOT 2020
// function for chat
/* void *traitement(int s)
{
    char *hello = "Hello from CC";
    write(s, (void *)hello, strlen(hello));
    return 0;
} */

int traitement_udp(unsigned char *message, int taille)
{
    (void)taille;
    printf("le msg que j'ai reçu du bot: %s\n", (char *)message);

    return 0;
}
int client_tcp(int socket_tcp)
{
    printf("0000000000000\n");
    char *msg = "Hello from CC\n";
    
    // Structure addresse du serveur (comme en UDP)
    struct sockaddr_in servaddr;
    socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    printf("11111111111111111\n");
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if (socket_tcp < 0)
    {
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1);
    }
    printf("222222222222222222\n");
    servaddr.sin_family = AF_INET;
    //On met l'addresse du serveur avec qui on veut communiquer
    servaddr.sin_addr.s_addr = inet_addr(IP_BOT);
    //Pareil pour le port
    servaddr.sin_port = htons(PORT_TCP_BOT);
    printf("333333333333333333333\n");
    if (connect(socket_tcp, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }
    printf("444444\n");
    // function for chat
    write(socket_tcp, (void *)msg, strlen(msg));
    printf("55555\n");
    sendTCP(socket_tcp, msg, strlen(msg));
    printf("666666\n");
    // close the socket
    close(socket_tcp);
    printf("7777\n");
    return socket_tcp;
}

int main()
{

     //PARTIE Client TCP (envoie)
    printf("partie TCP\n");
    int socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (lanceThread(client_tcp(socket_tcp)) < 0)
    {
        perror("clientTCP.lanceThread");
        exit(-1);
    }
    printf("sortie du thread\n");


    // PARTIE SERVEUR UDP (écoute)
    char *port_udp = UDP_PORT_ECOUTE;
    printf("partie udp\n");
    int socket_udp = initialisationServeurUDP(port_udp);
    printf("chui là\n");
    if (socket_udp < 0)
    {
        fprintf(stderr, "Initialisation du serveur UDP impossible\n");
        exit(-1);
    }

    /* Lancement de la boucle d'ecoute */
    if (lanceThread(boucleServeurUDP(socket_udp, traitement_udp)) < 0)
    {
        perror("boucleServeurUDP.lanceThread");
        exit(-1);
    }
    close(socket_udp);

   
    return 0;
}
