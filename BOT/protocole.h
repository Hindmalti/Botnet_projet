#ifndef PROTOCOLE_H
#define PROTOCOLE_H
#include "bot.h"

void recvFile(void *s);
void start_charge(char *filename);
void print_CU_structure(charge_utile_t *structure);
void receive_cmd_TCP(void *arg);
void install_charge(char *file_name);
void rm_charge(char *filename);
#endif