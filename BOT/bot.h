#ifndef BOT_H
#define BOT_H

#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
/*       Constantes       */
#define PORT_UDP_CLIENT 4242
#define PORT_UDP_SERVEUR 4242
#define PORT_TCP_CLIENT 4242
#define PORT_TCP_SERVEUR 4242
#define TAILLE_STRUCTURE 32
#define TAILLE_FILENAME 20
#define TAILLE_MSG_ERREUR 50
#define TAILLE_MSG_PROTOCOLE 50
#define BOT_ACTIF '1'
#define BOT_INACTIF '0'
#define CHARGE_EXECUTED 1
#define CHARGE_NON_EXECUTED 0
#define SIZE_ID 7
#define SIZE_TIME 5
#define SIZE_FILE 1000



typedef struct
{
    char ID[SIZE_ID];          //ID sur 6 octets
    char life_time[SIZE_TIME]; // le temps de vie du bot depuis son existence 4 octets
    char etat;                 // état du bot actif/inactif
} info_bot_t;

typedef struct {
    info_bot_t *info;
    struct sockaddr_in  addr;
} bot_t;
typedef struct
{
    void *plugin;
    char *nom;
    //struct sockaddr_in addresse_cc;
    int resultat;  //0 si ça s'est bien passé, autre
    char executed; //0 si non executée, 1 sinon. A la fin de l'execution, on met le résultat dans resultat et on passe l'exec à 1

} charge_utile_t;

/* Variables  publiques */
//extern liste_cu_t list_CU;
extern clock_t debut;

typedef int (*init_f)(void *);

void gestionClientTCP(void *s);

void nouveauClient(int dialogue);
void TCP(void *arg);
void partie_tcp_BOT();


void EnvoieBroadcast();
void partie_udp_BOT(clock_t debut);

void recvFile(void *s, char* filename);
void start_charge(char *filename);
void print_CU_structure(charge_utile_t *structure);
void receive_cmd_TCP(void *arg);
void install_charge(char *file_name);
void rm_charge(char *filename);
void send_status(int socket_tcp);
void send_result(int socket_tcp, int *resultat);
void quit_connexion();

char *create_ID(char *);
char *timeLife(clock_t, char *);
int remplissageStructure(info_bot_t *, clock_t);
void impressionStructure(info_bot_t *info);

#endif