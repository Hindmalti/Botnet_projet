#ifndef LIBIPC_H
#define LIBIPC_H


///// FILE DE MESSAGES /////

/**** Clé utilisé pour la File de Message ****/
#define CLE_FDM 6413

/**** Les Types de message délivrés ****/
#define TYPE_REQ 1
#define TYPE_REP 2

/**** Structures pour Files de Messages ****/
/* Pour la requete */
typedef struct {
    long type;
    char commande; //1, 2, 3, 4 ou 5
    char *charge_name;   
} req_t;

/* Pour la reponse */
typedef struct {
    long type;
    int résultat; // retour 0 ou 1  
} rep_t;

#endif



