#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libnetwork.h>

void (*traiement)(int, char *){
    receiveTCP(int socket, char *message, int max_length);
}

int main()
{
    //Msg à envoyer à tout le monde en TCP
    char *hello = "Hello from CC";
    int s = initialisationServeurTCP(5000);

    boucleServeurTCP(s, void (*traitement)(int, char *));
    sendTCP(s, hello, strlen(hello));
    return 0;
}