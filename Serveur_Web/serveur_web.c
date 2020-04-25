/** Include files **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <libthrd.h>
#include <libnetwork.h>
#include <errno.h>

#include "serveur_web.h"
/** quelques constantes **/

#define WEB_DIR "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK 200
#define CODE_NOTFOUND 404

#define NB_BOTS 3

//pour des besoins de tests on prepare une liste 
char *ListeBot[3] = {"Bot1", "Bot2", "Bot3"};


/**
* Void page_acceuil(int nb_bots, char* ListeBot[])
* Fonction qui utilise la liste des id de bots reçu du CC et le nombre de bots
* La fonction va se charger de créer la page d'acceuil html
* cette page d'acceuil nous permettra de faire nos commandes selon le bot souhaité
*/
void page_acceuil(int nb_bots, char* ListeBot[])
{
    FILE *acceuil = fopen("www/acceuil.html", "w");
    if (acceuil != NULL)
    {
        //fprintf(dialogue,"HTTP/1.1 200 OK\n");
        //fprintf(dialogue,"Content-Type: text/html;\n\n");
        fprintf(acceuil, "<!DOCTYPE html><html lang=\"fr\"><head><meta charset=\"utf-8\"/><title>upload</title></head>");
        fprintf(acceuil,"<body><h1 color:rgb(250,128,114)> BOTS </h1><h3>Groupe: Hind MALTI & Loris AHOUASSOU</h3>");
        fprintf(acceuil,"<div><form action=\"acceuil.html\" method=\"post\" enctype=\"application/x-www-form-urlencoded\">Choisissez un Bot : <select name=\"Bot\">");
        for (int i = 0; i < nb_bots; i++)
        {
            fprintf(acceuil, "<option value=\"%s\">%s</option>", ListeBot[i], ListeBot[i]);
        }
        fprintf(acceuil, "</select><input type=\"hidden\" name=\"Charge\" value=\"NONE\" /><input type=\"submit\" name=\"Commande\" value=\"Statut\" /><input type=\"submit\" name=\"Commande\" value=\"Quitter\" /></form></div>");
        fprintf(acceuil, "<br/><br/>");
        fprintf(acceuil, "<div><form action=\"acceuil.html\" method=\"post\" enctype=\"application/x-www-form-urlencoded\">Choisissez un Bot :<select name=\"Bot\">");
        for (int i = 0; i < nb_bots; i++)
        {
            fprintf(acceuil, "<option value=\"%s\">%s</option>", ListeBot[i], ListeBot[i]);
        }
        fprintf(acceuil, "</select><br/>Entrez le nom de votre charge:<input type=\"text\" name=\"Charge\" value=\"\" /><input type=\"submit\" name=\"Commande\" value=\"Executer\" /><input type=\"submit\" name=\"Commande\" value=\"Effacer\" /><input type=\"submit\" name=\"Commande\" value=\"Resultat\" /></form></div>");
        fprintf(acceuil, "<br/><br/>");
        fprintf(acceuil, "<div><form action=\"acceuil.html\" method=\"post\" enctype=\"multipart/form-data\">Choisissez un Bot :<select name=\"Bot\">");
        for (int i = 0; i < nb_bots; i++)
        {
            fprintf(acceuil, "<option value=\"%s\">%s</option>", ListeBot[i], ListeBot[i]);
        }
        fprintf(acceuil, "</select><br />Quelle Charge souhaitez-vous envoyer ? <input type=\"file\" name=\"MyFile\" /><input type=\"submit\" value=\"Envoyer\" /></form></div>");
        fprintf(acceuil, "</body></html>");
    }
    fclose(acceuil);
}

/**
 * char* extraction(char* chaine, int stop, const char* delim)
 * la fonction utilise une chaine dans laquelle on va extraire un sous chaine
 * le parametre delim est le delimiteur qui nous permet de couper la chaine
 * le parametre stop constitue le nombre de découpes avant de s'arrêter
 * 
 * la fonction retourne la sous-chaine extraite
 * 
 * cette fonction nous est particulièrement utile pour :
 * extraire de la requete post le nom du fichier
 * extraire de la requete post le nom du bot (son id)
 * extraire de la requete post le type de commande
*/

char* extraction(char* chaine, int stop, const char* delim){
        int i = 0;
        char* temp = strtok(chaine, delim);
        while(temp != NULL && i!=stop){
                temp=strtok(NULL, delim);
                i++;
            }
        if(i==0){
            return temp;
        }
        temp = strtok(NULL, delim);
    return temp;
}

/** void gestionClientWeb(void *s)
* prend en paramètre la socket de dialogue
* elle crée un file descriptor de la socket de dialogue
* ainsi elle va nous permet de recupérer les requetes GET ou POST
* Selon le type de requete, elle va lancer le traitement adéquat
* le detail des commentaires pour le traitement est à voir plus bas dans le code
*/

void gestionClientWeb(void *s)
{

    char buffer[MAX_BUFFER];
    char cmd[MAX_BUFFER];
    char page[MAX_BUFFER];
    char proto[MAX_BUFFER];
    char path[MAX_BUFFER];
    char type[MAX_BUFFER];

    //on prepare les receptacle des sous-chaines à extraire
    char* Bot;
    char* Charge;
    char* Commande;
    char* filename;

    int socket = *((int *)s);
    /* Obtient une structure de fichier */
    FILE *dialogue = fdopen(socket, "a+");
    if (dialogue == NULL)
    {
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }


    //Gestion des requêtes POST et GET

    if (fgets(buffer, MAX_BUFFER, dialogue) == NULL)
        exit(-1);
    if (sscanf(buffer, "%s %s %s", cmd, page, proto) != 3)
        exit(-1);
    printf("%s\n", buffer);

    int content_length = 0;
    char *temp = NULL;

    while (fgets(buffer, MAX_BUFFER, dialogue) != NULL)
    {
        printf("%s", buffer); // Pour afficher les header
        if (strcmp(buffer, "\r\n") == 0)
            break;

        if ((temp = strstr(buffer, "Content-Length:")) != NULL)
        {
            content_length = atoi(temp + 16); //On récupère la taille du fichier
        }
    }
    printf("MON CONTENT LENGTH = %d\n", content_length);
    char donnees[content_length];
    if (content_length > 0)
    {
        for (int i = 0; i < content_length; i++)
        {
            donnees[i] = fgetc(dialogue); //On récupère le contenu du fichier
        }
        donnees[content_length] = '\0';
    }

    printf(" CONTENU DU FICHIER =%s\n",donnees); //pour voir
    printf("fin de mon contenu\n");

    /*Extraction des parametres concernant les commandes suivantes :
    * Statut
    * Quitter
    * Executer
    * Effacer
    * Resultat
    */
    if(donnees[0]=='B'){
        char *temp;
        int i = 0;
        
        temp = strtok(donnees, "&");
        while(temp != NULL && i<3){
            if(i==0){
                Bot=temp;
                temp=strtok(NULL, "&");
                i++;
            }
            if(i==1){
                Charge=temp;
                temp=strtok(NULL, "&");
                i++;
            }
            if(i==2){
                Commande=temp;
                temp=strtok(NULL, "&");
                i++;
            }
            
        }
        
        
        if(sscanf(Bot, "Bot=%s", Bot)!=1)
        {
            printf("Erreur recupération commande\n");
        }
        if(sscanf(Charge, "Charge=%s", Charge)!=1)
        {
            printf("Erreur recupération commande\n");
        }
        if(sscanf(Commande, "Commande=%s", Commande)!=1)
        {
            printf("Erreur recupération commande\n");
        }
        //fin de l'extraction, on afficher les paramètres
        printf("Le Bot est %s, la charge est %s, et la commande est %s\n", Bot, Charge, Commande);
        

    }

    /*Sinon dans le cas de la commande d'installation d'une charge
    * On extrait le nom du fichier et du bot concerné
    */
    else{
        char* d1=malloc(content_length);
        char* d2=malloc(content_length);
        strcpy(d1, donnees);
        strcpy(d2, donnees);
        printf("Il s'agit d'un fichier récupéré ici\n");
        printf("Le bot est = %s\n", extraction(extraction(extraction(d1, 1, "\""), 0, "-"), 1, "\n"));
        printf("filename = %s\n", extraction(d2, 4, "\""));
    }

    

    // Traitement des réquêtes POST et GET : adapté du serveur web C minimal
    // le traitement de la requête post a été rajouté.
    if (strcasecmp(cmd, "GET") == 0 || strcasecmp(cmd, "POST") == 0)
    {
        int code = CODE_OK;
        struct stat fstat;
        sprintf(path, "%s%s", WEB_DIR, page);
        if (stat(path, &fstat) != 0 || !S_ISREG(fstat.st_mode))
        {
            sprintf(path, "%s/%s", WEB_DIR, PAGE_NOTFOUND);
            code = CODE_NOTFOUND;
        }
        strcpy(type, "text/html");
        char *end = page + strlen(page);
        if (strcmp(end - 4, ".png") == 0)
            strcpy(type, "image/png");
        if (strcmp(end - 4, ".jpg") == 0)
            strcpy(type, "image/jpg");
        if (strcmp(end - 4, ".gif") == 0)
            strcpy(type, "image/gif");
        fprintf(dialogue, "HTTP/1.0 %d\r\n", code);
        fprintf(dialogue, "Server: CWeb\r\n");

        fprintf(dialogue, "Content-type: %s\r\n", type);
        fprintf(dialogue, "Content-length: %ld\r\n", fstat.st_size);
        fprintf(dialogue, "\r\n");
        fflush(dialogue);
        int fd = open(path, O_RDONLY);
        if (fd >= 0)
        {
            int bytes;
            while ((bytes = read(fd, buffer, MAX_BUFFER)) > 0)
            {
                fwrite(buffer, 1, bytes, dialogue);
            }

            close(fd);
        }
    }

    fclose(dialogue);
}


/**void nouveauClientWeb(int dialogue)
 * elle prend en paramètre la socket de dialogue
 * elle lance la fonction gestionClientWeb()
 */
void nouveauClientWeb(int dialogue)
{

    printf("Client Web connecté !\n");
    gestionClientWeb((void *)&dialogue);
}

/** Procédure principale
 * la page d'acceuil est constitué en fonction de la liste des bots
 * puis boucleServeurTCP attend qu'on charge acceuil.html dans le navigateur
 * les différents traitement peuvent commencer selon nos requetes
 **/

int main(void)
{

    page_acceuil(3, ListeBot);

    // Serveur TCP + boucle serveur
    char *PORT_WEB = "8000";
    int s_tcp;

    s_tcp = initialisationServeurTCP(PORT_WEB);

    // Initialisation du serveur
    if (s_tcp < 0)
    {
        fprintf(stderr, "Initialisation du serveur impossible\n");
        exit(-1);
    }

    // Lancement de la boucle d'ecoute
    if (boucleServeurTCP(s_tcp, nouveauClientWeb) < 0)
    {
        fprintf(stderr, "Connexion avec le client impossible\n");
        exit(-1);
    }

    close(s_tcp);
    return 0;
}
