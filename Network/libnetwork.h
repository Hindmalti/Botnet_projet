#ifndef LIBNET_H
#define LIBNET_H

#include <stdint.h>
#include "../BOT/utils.h"

void sendUDPBroadcast(info_bot_t info_bot, int taille_structure, int port);

int initialisationServeurTCP(char *service);
int boucleServeurTCP(int socket, void (*traitement)(int));

int initialisationServeurUDP(char *service);
int boucleServeurUDP(int s, int (*traitement_udp)(info_bot_t info_bot, int taille));

int openTCPClient(char *hote, int port);
void sendTCP(int socket, char *message, int length_message);
int receiveTCP(int socket, char *message, int max_length);


#endif