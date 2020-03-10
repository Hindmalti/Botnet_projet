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

int nombre_thread_tcp;
int nombre_thread_udp;
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
    P(MUTEX_THREAD);
    nombre_thread_tcp--;
    V(MUTEX_THREAD); 

    return;
}
// Fonction qui lance le thread
void nouveauClient(int dialogue)
{

    char *recu = "Salut CC, je suis le serveur du BOT et je te recois";
    write(dialogue, recu, strlen(recu));
    if (lanceThread(gestionClient,(void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
    P(MUTEX_THREAD);
    nombre_thread_tcp++;
    V(MUTEX_THREAD);
}

int main()
{ // PARTIE UDP
    /*  //Msg à envoyer à tout le monde en UDP
    char *hello = "Hello from Bot";
    while (1)
    {
        sendUDPBroadcast((unsigned char *)hello, strlen(hello), 5000);
    } */
    // PARTIE SERVEUR TCP
    char port_s[6] = "2020";
    int s;
    nombre_thread_tcp = 0;
    nombre_thread_udp = 0;

    s = initialisationServeurTCP(port_s);
    /* Initialisation du serveur */
    if (s < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }

    /* Lancement du serveur de messages UDP */
    /*     if (lanceThread(_serveurMessages, NULL, 0))
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
    P(MUTEX_THREAD);
    nombre_thread_udp++;
    V(MUTEX_THREAD); */

    /* Lancement de la boucle d'ecoute */
    if (boucleServeurTCP(s, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }

    return 0;
}