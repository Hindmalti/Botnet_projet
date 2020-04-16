#ifndef LIBLISTES_H
#define LIBLISTES_H

#include "../BOT/utils.h"


typedef struct {
    charge_utile_t *charge;
    struct node_cu_t *next;
} node_cu_t, *liste_cu_t;


typedef struct {
    info_bot_t *bot;
    struct node_bot_t *next;
} node_bot_t, *liste_bot_t;

#endif