#ifndef SERVEUR_WEB_H
#define SERVEUR_WEB_H
//Prototypes
char* extraction(char* chaine, int stop, const char* delim);
void gestionClientWeb(void *s);
void nouveauClientWeb(int dialogue);
#endif