#ifndef PROTOCOLE_H
#define PROTOCOLE_H
#include "bot.h"

void send_status(int socket_tcp);
void send_result(int socket_tcp, int *resultat);
void quit_connexion();
void recvFile(void *s,char* filename);
void start_charge(char *filename);
void print_CU_structure(charge_utile_t *structure);
int getChargeFromMessage(int socket, charge_utile_t **returned_charge);
void receive_cmd_TCP(void *arg);
void install_charge(char *file_name);
void rm_charge(char *filename);
void quit_connexion();
#endif