#ifndef BOT_H
#define BOT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

#include "utils.h"
#include "envoie_udp.h"
#include "ecoute_tcp.h"
#include "protocole.h"

#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>
/*       Constantes       */
#define PORT_UDP_CLIENT 4242
#define PORT_UDP_SERVEUR 4242
#define PORT_TCP_CLIENT 4242
#define PORT_TCP_SERVEUR "4242"
#define TAILLE_STRUCTURE 32
#define TAILLE_FILENAME 20
#define BOT_ACTIF "1"
#define BOT_INACTIF "0"

/* Variables  publiques */
extern liste_cu_t list_CU;
extern clock_t debut;
extern charge_utile_t charge1;
extern charge_utile_t charge2;
extern info_bot_t *bot;


typedef void (*init_f)(void *);



#endif