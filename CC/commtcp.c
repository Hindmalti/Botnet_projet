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
// function for chat
/* void *traitement(int s)
{
    char *hello = "Hello from CC";
    write(s, (void *)hello, strlen(hello));
    return 0;
} */

int traitement_udp(unsigned char *message, char *hote)
{
    printf("Adresse du Bot source : %s\n", hote);
    printf("Message reçu : %s\n", (char *)message);

    return 0;
}

int main()
{

    // PARTIE SERVEUR UDP (écoute)
    char *port_udp = UDP_PORT_ECOUTE;

    int socket_udp = initialisationServeurUDP(port_udp);

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

    //PARTIE Client TCP (envoie)
    char *hello = "Hello from CC";
    int socket_tcp;
    // Structure addresse du serveur (comme en UDP)
    struct sockaddr_in servaddr;

    socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
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
    servaddr.sin_port = htons(2020);

    if (connect(socket_tcp, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
        //boucleServeurTCP(s, traitement(s));
        sendTCP(socket_tcp, hello, strlen(hello));
        // close the socket
    }
    close(socket_tcp);
    return 0;
}