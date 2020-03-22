#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libthrd.h>
#include <libnetwork.h>
#include <errno.h>
#include <time.h>


// Fonction du thread et qui continue le chat en TCP avec le CC
void gestionClient(void *s)
{

    int socket = *((int *)s);
    /* Obtient une structure de fichier */
    FILE *dialogue = fdopen(socket, "a+");
    if (dialogue == NULL)
    {
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }
    fclose(dialogue);

    return;
}
// Fonction qui lance le thread
void nouveauClient(int dialogue)
{

    char *recu = "Salut CC, je suis le serveur du BOT et je te recois";
    write(dialogue, recu, strlen(recu));
    if (lanceThread(gestionClient, (void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
}
void EnvoieUDPBroadcast(void *arg)
{
    //Ici le arg est la chaine de caractère
    //structure_example *arg2;
    //arg2 = (*structure_example)arg;
    
    while (1)
    {
        sendUDPBroadcast((unsigned char *)arg, strlen((char *) arg), 4242);
        sleep(5);
    }
}

int main()
{ // PARTIE UDP
    int debut = clock();
    char string[] = "Coucou from bot, depuis";
    //Msg à envoyer à tout le monde en UDP
    if (lanceThread(EnvoieUDPBroadcast, (void *)string, strlen(string)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }

    
    // PARTIE SERVEUR TCP
    char port_s[6] = "2020";
    int s;
    /* Initialisation du serveur */
    s = initialisationServeurTCP(port_s);

    if (s < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }

    /* Lancement de la boucle d'ecoute */
    if (boucleServeurTCP(s, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }

    return 0;
}