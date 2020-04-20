#ifndef LIBNET_H
#define LIBNET_H


void sendUDPBroadcast(void* payload, int taille_payload, int port);

int initialisationServeurTCP(char *service);
int boucleServeurTCP(int socket, void (*traitement)(int));

int initialisationServeurUDP(char *service);
int boucleServeurUDP(int s, int (*traitement_udp)(void *payload, int taille), int taille_payload);

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);


#endif