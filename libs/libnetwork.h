#ifndef LIBNET_H
#define LIBNET_H

#include <stdint.h>

void sendUDPBroadcast(unsigned char *message, int taille_message, int port);

int initialisationServeurTCP(char *service);
int boucleServeurTCP(int socket, void (*traitement)(int));

int initialisationServeurUDP(char *service);
int boucleServeurUDP(int s, int (*traitement_udp)(unsigned char *, int));

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);


#endif