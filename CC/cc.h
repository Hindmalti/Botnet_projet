#ifndef LIBCC_H
#define LIBCC_H

#include <liblistes.h>
#include <libnetwork.h>
#include <libthrd.h>
#include "../BOT/utils.h"

#define UDP_PORT_ECOUTE "4242"
#define IP_BOT "127.0.0.1"
#define PORT_TCP_BOT 4242
#define TAILLE 20

int traitementUDP(info_bot_t structure, int taille);
void lancementBoucleServeurUDP(void *s);
void send_file_tcp(void *s);
void init_socket(void *arg);
void partie_udp();
void partie_tcp();


#endif