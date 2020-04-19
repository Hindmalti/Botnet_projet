#ifndef LIBCC_H
#define LIBCC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include <libshm.h>
#include <liblistes.h>
#include <libnetwork.h>
#include <libthrd.h>

#include "protocole.h"
#include "ecoute_udp.h"
#include "envoie_tcp.h"
#include "../BOT/utils.h"

#define UDP_PORT_ECOUTE "4242"
#define IP_BOT "127.0.0.1"
#define PORT_TCP_BOT 4242
#define TAILLE 20
#define TAILLE_FILENAME 20

extern liste_bot_t list;
void ecritureIDshmem(info_bot_t *bot);


#endif