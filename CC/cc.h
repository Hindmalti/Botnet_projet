#ifndef LIBCC_H
#define LIBCC_H

#include <liblistes.h>
#include "../BOT/bot.h"

#define UDP_PORT_ECOUTE "4242"
#define PORT_TCP_BOT 4242
#define TAILLE 20
#define TAILLE_FILENAME 20
#define TAILLE_MSG_ERREUR 50
#define TAILLE_MSG_PROTOCOLE 50

#define KEY (key_t)1996
#define KEY_DATA_CC (key_t)400
#define KEY_NBRE_BOT (key_t)987
#define KEY_DATA (key_t)100
#define SIZE_DATA 19
#define NBRE_MAX_BOT 100 // on fixe la taille max du nbre de bots sinon on ne pourra pas redimensionner la shm


#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

extern liste_bot_t list;
typedef struct
{
    char cmd;
    char filename[TAILLE_FILENAME];
    bot_t *bot;
} ordre_t;
//void ecritureIDshmem(info_bot_t *bot);
int llist_bot_to_array(liste_bot_t list, info_bot_t *returned_array);
int comptageNbreBot(liste_bot_t list);
void send_command_tcp(ordre_t *ordre);
void lancementBoucleServeurUDP(void *s);
void send_file_tcp(void *s, char *filename);
int init_socket_bot(bot_t *bot);
void partie_tcp();
void partie_udp();
char **str_split(char *a_str, const char a_delim);
void getOrdreFromShm();
int traitementUDP(struct sockaddr_storage addr, void *payload, int taille);
void lancementBoucleServeurUDP(void *s);

#endif