#ifndef PROTOCOLE_H
#define PROTOCOLE

#include "cc.h"
void lancementBoucleServeurUDP(void *s);
void send_file_tcp(void *s, char *filename);
void send_commad_tcp(void *s);
#endif