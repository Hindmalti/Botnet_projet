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

    char *recu = "Salut CC, je suis le serveur du BOT et je reçois ton msg";
    write(dialogue, recu, strlen(recu));
    if (lanceThread(gestionClient,(void *)&dialogue, sizeof(dialogue)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
}
void EnvoieUDPBroadcast(void *message){
    //char *hello = "Hello from Bot UDP Broadcast";
    
    while (1)
    {
        sendUDPBroadcast((unsigned char *)message, strlen((char *)message), 4242);
        sleep(5);
    } 
}

int main()
{ // PARTIE UDP
    //int socketUDP;
    char message[] = "Salut je suis un BOT";
    //Msg à envoyer à tout le monde en UDP
    //if (lanceThread(EnvoieUDPBroadcast,(void *)&socketUDP, sizeof(socketUDP)) < 0)
    if (lanceThread(EnvoieUDPBroadcast,(void *)message, strlen(message)) < 0)
    {
        perror("nouveauClient.lanceThread");
        exit(-1);
    }
     //PARTIE SERVEUR TCP
    char port_s[6] = "2020";
    int s;

    s = initialisationServeurTCP(port_s);
    // Initialisation du serveur 
    if (s < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }

    // Lancement de la boucle d'ecoute 
    if (boucleServeurTCP(s, nouveauClient) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }

    return 0;
}