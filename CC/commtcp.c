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
void *traitement(int s)
{
    char *hello = "Hello from CC";
    write(s, (void *)hello, strlen(hello));
    return 0;
}

int affiche(unsigned char *message, int nboctets){
     
     printf("nboctets : %d", nboctets);
     return 0;
}

int main(){
//PARTIE SERVEUR UDP
    char port_s[6] = "2020";
    int s_udp;

    s_udp = initialisationServeurUDP(port_s);
    // Initialisation du serveur
    if (s_udp < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }
    printf("ok");
    // Lancement de la boucle d'ecoute udp 
    if (boucleServeurUDP(s_udp, affiche) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }

  
    //Msg à envoyer à tout le monde en TCP
    char *message = "Hello from CC";
    int s_tcp;
    // Structure addresse du serveur (comme en UDP)
    struct sockaddr_in servaddr;

    s_tcp = socket(AF_INET, SOCK_STREAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if (s_tcp < 0)
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
   
        if (connect(s_tcp, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            printf("connection with the server failed...\n");
            exit(0);
        }
        else
        {
            printf("connected to the server..\n");
            sendTCP(s_tcp, message, strlen(message));
            // close the socket
            
        }
    close(s_tcp);
    return 0;
}