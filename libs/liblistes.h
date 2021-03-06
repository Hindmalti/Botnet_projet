#ifndef LIBLISTES_H
#define LIBLISTES_H

#include "../BOT/bot.h"
#include <dlfcn.h>

typedef struct {
    charge_utile_t *charge;
    struct node_cu_t *next;
} node_cu_t, *liste_cu_t;


typedef struct {
    bot_t *bot;
    struct node_bot_t *next;
} node_bot_t, *liste_bot_t;


void init_listCU(liste_cu_t *list);
void init_listbot(liste_bot_t *bot);

void ajout_tete_cu(liste_cu_t *list, charge_utile_t *charge);
void ajout_tete_bot(liste_bot_t *list, bot_t *bot);

void print_CU_structure(charge_utile_t *charge);
void print_BOT_structure(bot_t *bot);

void print_listeCU(liste_cu_t liste);
void print_listeBot(liste_bot_t liste);

int rechercheCU(char *filename, liste_cu_t *liste, charge_utile_t **charge);
void rechercheBOT(char *id, liste_bot_t *list, bot_t **bot);

void supp_elm_liste_CU(liste_cu_t *liste, char *filename);
void supp_elm_liste_BOT(liste_bot_t *bot, char *id);

void supp_charge(charge_utile_t *charge);

void detruire_liste_CU(liste_cu_t *list);
void detruire_liste_BOT(liste_bot_t *bot);

#endif