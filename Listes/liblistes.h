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

void init_listCU(liste_cu_t *list);
void init_listbot(liste_bot_t *bot);

void ajout_tete_cu(liste_cu_t *list, charge_utile_t *charge);
void ajout_tete_bot(liste_bot_t *list, info_bot_t *bot);

void print_CU_structure(charge_utile_t *charge);
void print_BOT_structure(info_bot_t *bot);

void print_listeCU(liste_cu_t liste);
void print_listeBot(liste_bot_t liste);

void rechercheCU(char *filename, liste_cu_t *liste);
void rechercheBOT(char *id, liste_bot_t *bot);

void supp_elm_liste_CU(liste_cu_t *liste, char *filename);
void supp_elm_liste_BOT(liste_bot_t *bot, char *id);

void detruire_liste_CU(liste_cu_t *list);
void detruire_liste_BOT(liste_bot_t *bot);

#endif