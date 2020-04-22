#include "liblistes.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

/** void init_listCU(liste_cu_t* list)
 * Fonction permetant d'inistialiser une liste de charges utiles
*/

void init_listCU(liste_cu_t *list)
{ //Init new list
    *list = NULL;
}
/** void init_listbot(info_bot_t *bot)
 * Fonction permetant d'inistialiser une liste de bots
*/

void init_listbot(liste_bot_t *bot)
{ //Init new list
    *bot = NULL;
}
/** void ajout_tete_cu(liste_cu_t *list, charge_utile_t *charge)
 * Fonction permetant d'ajouter en tete des charges utiles
 * param pointeur d'une liste de charges utiles
 * param pointeur d'une structure de charge utile
*/

void ajout_tete_cu(liste_cu_t *list, charge_utile_t *charge)
{
    node_cu_t *newNode = (node_cu_t *)malloc(sizeof(node_cu_t));
    newNode->charge = charge;
    newNode->next = (struct node_cu_t *)*list;
    *list = newNode;
}

/** void ajout_tete_bot(liste_bot_t *list, info_bot_t *bot)
 * Fonction permetant d'ajouter en tete des bots
 * param pointeur d'une liste de bot
 * param pointeur d'une structure de bot
*/

void ajout_tete_bot(liste_bot_t *list, info_bot_t *bot)
{
    node_bot_t *newNode = (node_bot_t *)malloc(sizeof(node_bot_t));
    newNode->bot = bot;
    newNode->next = (struct node_bot_t *)*list;
    *list = newNode;
}

/** void print_CU_structure(charge_utile_t *charge)
 *  Fonction permettant d'imprimer une charge utile sous format json
 *  param pointeur vers une charge utile
 */
void print_CU_structure(charge_utile_t *charge)
{
    printf("\t{\n");
    printf("\t\t\"Nom\": %s,\n", charge->nom);
    //printf("\t\t\"IP adresse\": %d,\n", charge->addresse_cc);
    printf("\t\t\"Resultat\": %d,\n", charge->resultat);
    printf("\t\t\"Executed\": %d\n", charge->executed);
    printf("\t}\n");
}

/** void print_CU_structure(charge_utile_t *charge)
 *  Fonction permettant d'imprimer une charge utile sous format json
 *  param pointeur vers une charge utile
 */
void print_BOT_structure(info_bot_t *bot)
{
    printf("\t{\n");
    printf("\t\t\"ID\": %s,\n", bot->ID);
    printf("\t\t\"Temps de vie\": %s,\n", bot->life_time);
    printf("\t\t\"Etat\": %c\n", bot->etat);
    printf("\t}\n");
}
/**void print_listeCU(liste_cu_t liste)
 * Fonction wrapper permettant d'imprimer le contenu de la liste des charges utiles
 * param une liste de charges utiles
 */
void print_listeCU(liste_cu_t liste)
{
    if (liste == NULL)
    {
        printf("List is empty.");
    }
    printf("[\n");
    while (liste != NULL)
    {
        print_CU_structure(liste->charge);
        liste = (liste_cu_t)liste->next;
    }
    printf("]\n");
}

/**void print_listeBot(liste_bot_t liste)
 * Fonction wrapper permettant d'imprimer le contenu de la liste des bots
 * param une liste de bot
 */
void print_listeBot(liste_bot_t liste)
{
    if (liste == NULL)
    {
        printf("List is empty.");
    }
    printf("[\n");
    while (liste != NULL)
    {
        print_BOT_structure(liste->bot);
        liste = (liste_bot_t)liste->next;
    }
    printf("]\n");
}

/** charge_utile_t *rechercheCU(char *filename, liste_cu_t *liste)
 *  Fonction permettant de rechercher une charge utile grâce à son nom
 *  param le nom de la charge utile
 *  param la liste des charges utiles
 *  return la structure de la charge utile
 */
int rechercheCU(char *filename, liste_cu_t *liste, charge_utile_t **charge)
{
    node_cu_t *current = *liste;
    while (current != NULL)
    {
        if (strcmp(current->charge->nom, filename) == 0)
        {
            printf("The file with the name %s is here \n", filename);
            print_CU_structure(current->charge);
            *charge = current->charge;
            return 0;
        }
        else
        {
            current = (liste_cu_t)current->next;
        }
    }
    perror("La charche utile n'est pas présente dans la liste\n");
    *charge = NULL;
    return 1;
}

/** void rechercheBOT(char *id, liste_bot_t *bot)
 *  Fonction permettant de rechercher une charge utile grâce à son ID
 *  param l'ID du bot
 *  param la liste des bots
 */
void rechercheBOT(char *id, liste_bot_t *list, info_bot_t **bot) {
    node_bot_t *current = *list;
    while (current != NULL) {
        if (strcmp(current->bot->ID, id) == 0) {
            printf("The file with the ID %s is here \n", id);
            print_BOT_structure(current->bot);
            *bot = current->bot;
            return;
        } else {
            current = (liste_bot_t)current->next;
        }
    }
    *bot = NULL;
    printf("Error : this Bot doesn't exist\n");
}
/** void supp_tete(liste_cu_t *liste)
 * Fonction per mettant de supprimer un elmt de la liste
 * param pointeur d'une liste de charges utiles 
 * param nom de la charge utile
 */

/* void supp_tete(liste_cu_t *liste)
{
    node_cu_t *tmp = *liste;
    free(tmp->charge);
    *liste = (liste_cu_t)tmp->next;
    free(tmp);
} */
/** void supp_elm_liste_CU(liste_cu_t *liste, char *filename)
 * Fonction permettant de supprimer un élement de la liste des CU grâce à son nom 
 * param pointeur vers la liste des charges utiles
 * param nom de la charge utile
 */
void supp_elm_liste_CU(liste_cu_t *liste, char *filename)
{
    node_cu_t *current = *liste;
    node_cu_t *prev;
    //Cas où le fichier de charge utile à supprimer est le premier de la liste des charges utiles
    if (current != NULL && (strcmp(current->charge->nom, filename) == 0))
    {
        *liste = (liste_cu_t)current->next;
        printf("The file '%s' will be deleted ! \n", filename);
        //free l'espace mémoire occupé par la charge
        supp_charge(current->charge);
        free(current->charge);
        free(current);
        return;
    }
    while (current != NULL && (strcmp(current->charge->nom, filename) != 0))
    {
        prev = current;
        current = (liste_cu_t)current->next;
    }
    // Cas où le fichier n'est pas dans la liste
    if (current == NULL)
    {
        printf("La charge utile n'est pas présente dans la liste\n");
        return;
    }

    prev->next = current->next;
    printf("The file '%s' will be deleted ! \n", filename);
    free(current->charge);
    free(current);
}
/** void _supp_charge(charge_utile_t *charge) 
 * Fonction permettant de free les pointeurs proprement
 * param pointeur vers la charge utile
 */ 
void supp_charge(charge_utile_t *charge) {
    dlclose(charge->plugin);
    free(charge->nom);
}

/** void supp_elm_liste_BOT(liste_bot_t *bot, char *id)
 * Fonction permettant de supprimer un élement de la liste des BOTS grâce à son ID
 * param pointeur vers la liste des BOTS
 * param ID du BOT
 */
void supp_elm_liste_BOT(liste_bot_t *bot, char *id)
{
    node_bot_t *current = *bot;
    node_bot_t *prev;
    //Cas où le bot à supprimer est le premier de la liste des bots
    if (current != NULL && (strcmp(current->bot->ID, id) == 0))
    {
        *bot = (liste_bot_t)current->next;
        printf("The bot  with ID : '%s' will be deleted ! \n", id);
        free(current->bot);
        free(current);
        return;
    }
    while (current != NULL && (strcmp(current->bot->ID, id) != 0))
    {
        prev = current;
        current = (liste_bot_t)current->next;
    }
    // Cas où le bot n'est pas dans la liste
    if (current == NULL)
    {
        printf("Error : this bot doesn't exist\n");
        return;
    }

    prev->next = current->next;
    printf("The bot  with ID : '%s' will be deleted ! \n", id);
    free(current->bot);
    free(current);
}

/** void detruire_liste_CU(liste_cu_t *list)
 * Fonction générique permettant de vider une liste de charges utiles
 * param pointeur de la liste des charges utiles
 */
void detruire_liste_CU(liste_cu_t *list)
{
    node_cu_t *next;
    node_cu_t *current = *list;
    while (current != NULL)
    {
        free(current->charge);
        next = (node_cu_t*)current->next;
        free(current);
        current = next;
    }
    //Premier élement vers NULL
    *list = NULL;
    printf("SUCCESSFULLY DELETED ALL NODES OF LINKED LIST\n");
}

/** void detruire_liste_BOT(liste_bot_t *bot)
 *  Fonction générique permettant de vider une liste de bots  
 *  param pointeur de la liste des bots
 */
void detruire_liste_BOT(liste_bot_t *bot)
{
    node_bot_t *next;
    node_bot_t *current = *bot;
    while (current != NULL)
    {
        free(current->bot);
        next = (node_bot_t*)current->next;
        free(current);
        current = next;
    }
    //Premier élement vers NULL
    *bot = NULL;
    printf("SUCCESSFULLY DELETED ALL NODES OF LINKED LIST\n");
}
/* int main()
{
    charge_utile_t *charge1 = (charge_utile_t *)malloc(sizeof(charge_utile_t));
    charge_utile_t *charge2 = (charge_utile_t *)malloc(sizeof(charge_utile_t));
    strcpy(charge1->nom, "Charge1");
    charge1->resultat = 0;
    charge1->executed = 1;

    strcpy(charge2->nom, "Charge2");
    charge2->resultat = 500;
    charge2->executed = 1;

    liste_cu_t list;
    init_listCU(&list);

    ajout_tete_cu(&list, charge1);
    ajout_tete_cu(&list, charge2);

    print_listeCU(list);
    rechercheCU("Charge2", &list);
    supp_elm_liste_CU(&list, "Charge1");
    print_listeCU(list);
    detruire_liste_CU(&list);
    print_listeCU(list);
    return 0;
}
 */
/* 
int main()
{
    info_bot_t *bot1 = (info_bot_t *)malloc(sizeof(info_bot_t));
    info_bot_t *bot2 = (info_bot_t *)malloc(sizeof(info_bot_t));
    strcpy(bot1->ID, "bot100");
    strcpy(bot1->life_time, "0,02");
    //strcpy(bot1->etat, "0");

    strcpy(bot2->ID, "bot200");
    strcpy(bot2->life_time, "0,01");
    //strcpy(bot2->etat, "1");

    liste_bot_t list;
    init_listbot(&list);

    ajout_tete_bot(&list, bot1);
    ajout_tete_bot(&list, bot2);

    print_listeBot(list);
    rechercheBOT("bot200", &list);
    supp_elm_liste_BOT(&list, "bot100");
    print_listeBot(list);
    detruire_liste_BOT(&list);
    print_listeBot(list); 
    return 0;
}
*/