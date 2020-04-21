#define _XOPEN_SOURCE
 
#include <stdlib.h>     /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>      /* Pour printf, perror */
#include <sys/msg.h>    /* Pour msgget, msgsnd, msgrcv */
#include <errno.h>      /* Pour errno */
#include <sys/stat.h>   /* Pour S_IRUSR, S_IWUSR */
 
#include "libipc.h"

//Fonction pour lire une requete
void lecture_requete(int msgid, req_t la_requete, char commande, char *charge_name){
     /* On crée la file si elle n'existe pas */
    if((msgid = msgget((key_t)CLE_FDM, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1) {
        if(errno == EEXIST)
        fprintf(stderr, "Erreur ! la file (cle=%d) existe deja\n", CLE_FDM);
        else
        perror("Erreur de creation\n");
        exit(EXIT_FAILURE);
    }
    
    /* Attente d'une requete */
    printf("attend une requete...\n");
    if(msgrcv(msgid, &la_requete, sizeof(req_t) - sizeof(long), TYPE_REQ, 0) == -1) {
        perror("Erreur de reception\n");
        exit(EXIT_FAILURE);
    }
    //récupération des valeurs
    commande = la_requete.commande;
    charge_name = la_requete.charge_name;
    printf("la requete recue est la commande %c et de nom de charge %s\n", commande, charge_name);
    }

//fonction pour envoyer une requete
void envoyer_requete(int msgid, req_t la_requete, char commande, char *charge_name){
    /* On récupère la file */
    if((msgid = msgget((key_t)CLE_FDM, 0)) == -1) {
        perror("Erreur survenue pour récupération de la file\n");
        exit(EXIT_FAILURE);
    }
    
    /* remplissage la requête */  
    la_requete.type = TYPE_REQ;
    la_requete.commande = commande;
    la_requete.charge_name = charge_name;

    /* Envoi de la requete */
    if(msgsnd(msgid, &la_requete, sizeof(req_t) - sizeof(long), 0) == -1){
        perror("Erreur d'envoi de requete");
        exit(EXIT_FAILURE);
    }
    printf("la requete a bien été envoyée");
    
}
//fonction pour lire une reponse
void lecture_reponse(int msgid, rep_t la_reponse, int resultat){
     /* On récupère la file */
    if((msgid = msgget((key_t)CLE_FDM, 0)) == -1) {
        perror("Erreur survenue pour récupération de la file\n");
        exit(EXIT_FAILURE);
    }
    
    /* Attente d'une reponse */
    printf("attend une reponse...\n");
    if(msgrcv(msgid, &la_reponse, sizeof(rep_t) - sizeof(long), TYPE_REP, 0) == -1) {
        perror("Erreur de reception\n");
        exit(EXIT_FAILURE);
    }
    //récupération des valeurs
    resultat = la_reponse.resultat;
    printf("reponse bien recue : %d\n", resultat);
    }

//fonction pour envoyer une reponse

//fonction pour créer la file ?

//fonction pour détruire la file de message