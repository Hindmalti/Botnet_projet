#ifndef LIBNET_H
#define LIBNET_H

#include <sys/socket.h>

#define MAX_TAMPON 50
#define CODE_ERREUR 500
#define CODE_SUCCES 200

void sendUDPBroadcast(void* payload, int taille_payload, int port);

int initialisationServeurTCP(char *service);
int boucleServeurTCP(int socket, void (*traitement)(int));

int initialisationServeurUDP(char *service);
int boucleServeurUDP(int s, int (*traitement_udp)(struct sockaddr_storage, void *, int), int taille_payload);

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);
void socketVersClient(int s,char **hote,char **service);
void renvoieErreur(int socket);
void renvoieSucces(int socket);


#endif