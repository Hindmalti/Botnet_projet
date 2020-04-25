#ifndef LIBNET_H
#define LIBNET_H

#include <sys/socket.h>


void sendUDPBroadcast(void* payload, int taille_payload, int port);

int initialisationServeurTCP(char *service);
int boucleServeurTCP(int socket, void (*traitement)(int));

int initialisationServeurUDP(char *service);
int boucleServeurUDP(int s, int (*traitement_udp)(struct sockaddr_storage, void *, int), int taille_payload);
int openTCPClient(char *hote, int port);
void socketVersClient(int s,char **hote,char **service);


#endif