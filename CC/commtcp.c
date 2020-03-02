#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
//#include <libnetwork.h>

int main()
{
    //Msg à envoyer à tout le monde en TCP
    char *hello = "Hello from CC";
    int s;
    // Structure addresse du serveur (comme en UDP)
    struct sockaddr_in servaddr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    //Création de la socket : s = file descriptor de la socket, AF_INET (socket internet), SOCK_DGRAM (datagramme, UDP, sans connexion)
    if (s < 0)
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

    if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }

    // function for chat
    write(s,(void*)hello,strlen(hello));

    // close the socket
    close(s);
}