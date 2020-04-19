#include "envoie_tcp.h"


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
        send_commad_tcp((void *)&socket_tcp);
    }
    return;
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
