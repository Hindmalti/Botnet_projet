
/**** Minimal web server ****/

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

/** Some constants **/

#define WEB_DIR "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK 200
#define CODE_NOTFOUND 404

#define NB_BOTS 3
char *ListeBot[3] = {"Bot1", "Bot2", "Bot3"};

/* gestion des requêtes web 
- prend en paramètre la socket de dialogue
- met en place la file descriptor de cette socket afin de read-write dessus
- 

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

char* extract_filename(char* chaine, int stop){
        int i = 0;
        char* temp = strtok(chaine, "\"");
        while(temp != NULL && i!=stop){
                temp=strtok(NULL, "\"");
                i++;
            }
        temp = strtok(NULL, "\"");
    return temp;
}

void gestionClientWeb(void *s)
{

    char buffer[MAX_BUFFER];
    char cmd[MAX_BUFFER];
    char page[MAX_BUFFER];
    char proto[MAX_BUFFER];
    char path[MAX_BUFFER];
    char type[MAX_BUFFER];

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

    //PageWeb d'acceuil chargée dynamiquement créant les bouton
    // Pour chaque bot présent dans la liste

    //char liste[MAX_BUFFER];
    //while(fgets(liste,MAX_BUFFER,dialogue)!=NULL){

    /* Termine la connexion */

    //}

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

    printf(" CONTENU DU FICHIER =%s\n",donnees);
    printf("fin de mon contenu\n");

    //Différenciation du type de requete
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
        printf("Le Bot est %s, la charge est %s, et la commande est %s\n", Bot, Charge, Commande);
        

    }
    else{
        char* d1=malloc(content_length);
        char* d2=malloc(content_length);
        strcpy(d1, donnees);
        strcpy(d2, donnees);
        printf("Il s'agit d'un fichier récupéré ici\n");
        printf("Le bot est = %s\n", extract_filename(d1, 1));
        printf("filename = %s\n", extract_filename(d2, 4));
        
    }

    //printf("%ld", fread(buffer,strlen(buffer)+1, 1, dialogue));
    // 1) faire un malloc de la longueur de ma page (content-length) qui doit etre
    // en unsigned char
    // 2) recupérer la longueur avec un fread(buffer,0,bytes,dialogue)
    // si > 0 alors fficher

    // Traitement des réquêtes POST et GET : adapté du serveur web C minimal
    // le traitement de la requêtes post a été rajouté.
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

// Juste pour rediriger vers la fonction gestClientWeb()
void nouveauClientWeb(int dialogue)
{

    printf("Client Web connecté !\n");
    gestionClientWeb((void *)&dialogue);
}

/** Main procedure **/

int main(void)
{

    /*FILE *acceuil = fopen("www/acceuil.html", "w");
    if (acceuil != NULL)
    {
        //fprintf(dialogue,"HTTP/1.1 200 OK\n");
        //fprintf(dialogue,"Content-Type: text/html;\n\n");
        fprintf(acceuil, "<!DOCTYPE html>\n<html>\n<head>");
        fprintf(acceuil, "<title> BOTS DISPONIBLES </title></head>");
        fprintf(acceuil, "<body><h1 color:rgb(250,128,114)> BOTS </h1><h3>Groupe: Hind MALTI & Loris AHOUASSOU</h3>");
        for (int i = 0; i < NB_BOTS; i++)
        {
            fprintf(acceuil, "<button onclick=\"document.location.href='formulaire.html';\"> %s </button>", ListeBot[i]);
        }
        fprintf(acceuil, "</body></html>");
        fclose(acceuil);
    }*/
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
