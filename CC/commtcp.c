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

// function for chat
/* void *traitement(int s)
{
    char *hello = "Hello from CC";
    write(s, (void *)hello, strlen(hello));
    return 0;
} */

int main()
{

    // PARTIE SERVEUR UDP (écoute) 

    int s1 = initialisationServeurUDP("2020");

    if (s1 < 0)
    {
        fprintf(stderr, "Initialisation du serveur UDP impossible\n");
        exit(-1);
    }

    /* Lancement de la boucle d'ecoute */
    if (boucleServeurUDP(s1, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client UDP impossible\n");
        exit(-1);
    }


    //PARTIE Client TCP (envoie)
    char *hello = "Hello from CC";
    int s2;
    // Structure addresse du serveur (comme en UDP)
    struct sockaddr_in servaddr;

    s2 = socket(AF_INET, SOCK_STREAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if (s2 < 0)
    {
        //Test de la valeur de retour de la socket
        perror("sendUDPBroadcast.socket");
        exit(-1);
    }

    servaddr.sin_family = AF_INET;
    //On met l'addresse du serveur avec qui on veut communiquer
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //Pareil pour le port
    servaddr.sin_port = htons(2020);

    if (connect(s2, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
        //boucleServeurTCP(s, traitement(s));
        sendTCP(s2,hello, strlen(hello));
        // close the socket
    }
    close(s2);
    return 0;
}
