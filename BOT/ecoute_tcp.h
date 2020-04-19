#ifndef ECOUTE_TCP_H
#define ECOUTE_TCP_H

#include "bot.h"

void gestionClientTCP(void *s);

void nouveauClient(int dialogue);
void TCP(void *arg);
void partie_tcp();

#endif