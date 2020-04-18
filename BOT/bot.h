#ifndef BOT_H
#define BOT_H

#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>

#define PORT_UDP_CLIENT 4242
#define PORT_UDP_SERVEUR 4242
#define PORT_TCP_CLIENT 4242
#define PORT_TCP_SERVEUR "4242"
#define TAILLE_STRUCTURE 32

// Type de la fonction présente dans le .so il faut qu'il soit le plus générique possible
typedef void (*init_f)(void *);
void start_charge(char *filename);
void print_CU_structure(charge_utile_t *structure);
void install_charge(char *file_name);
void rm_charge(charge_utile_t *structure);
void gestionClientTCP(void *s);
void recvFile(void *s);
void nouveauClient(int dialogue);
void EnvoieBroadcast(void *structure);
void TCP(void *arg);
void partie_tcp();
void partie_udp(clock_t debut);

#endif