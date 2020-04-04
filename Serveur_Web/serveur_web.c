
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

#define WEB_DIR  "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK  200
#define CODE_NOTFOUND 404



// gestion des requêtes web
void gestionClientWeb(void *s){
   
    char buffer[MAX_BUFFER];
    char cmd[MAX_BUFFER];
    char page[MAX_BUFFER];
    char proto[MAX_BUFFER];
    char path[MAX_BUFFER];
    char type[MAX_BUFFER];

    int socket = *((int *)s);
    /* Obtient une structure de fichier */
    FILE *dialogue = fdopen(socket, "a+");
    if (dialogue == NULL)
    {
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }
    
    //Gestion des requêtes

    if(fgets(buffer,MAX_BUFFER,dialogue)==NULL) exit(-1);
    if(sscanf(buffer,"%s %s %s",cmd,page,proto)!=3) exit(-1);
    printf("%s\n", buffer);
    while(fgets(buffer,MAX_BUFFER,dialogue)!=NULL){
    printf("%s", buffer);
    if(strcmp(buffer,"\r\n")==0) break;
  }

  // 1) faire un malloc de la longueur de ma page (content-length) qui doit etre
  // en unsigned char
  // 2) recupérer la longueur avec un fread(buffer,0,bytes,dialogue)
  // si > 0 alors fficher

  
  //Methode POST
  if(strcasecmp(cmd,"GET")==0 || strcasecmp(cmd,"POST")==0){
  int code=CODE_OK;
  struct stat fstat;
  sprintf(path,"%s%s",WEB_DIR,page);
  if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode)){
    sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
    code=CODE_NOTFOUND;
    }
  strcpy(type,"text/html");
  char *end=page+strlen(page);
  if(strcmp(end-4,".png")==0) strcpy(type,"image/png");
  if(strcmp(end-4,".jpg")==0) strcpy(type,"image/jpg");
  if(strcmp(end-4,".gif")==0) strcpy(type,"image/gif");
  fprintf(dialogue,"HTTP/1.0 %d\r\n",code);
  fprintf(dialogue,"Server: CWeb\r\n");
  
  fprintf(dialogue,"Content-type: %s\r\n",type);
  fprintf(dialogue,"Content-length: %ld\r\n",fstat.st_size);
  fprintf(dialogue,"\r\n");
  fflush(dialogue);
  int fd=open(path,O_RDONLY);
  if(fd>=0){
    int bytes;
    while((bytes=read(fd,buffer,MAX_BUFFER))>0){
        fwrite(buffer,1,bytes,dialogue);
    }
    
    close(fd);
    }
  }

    fclose(dialogue);
    return;
}

// Fonction qui lance le thread
void nouveauClientWeb(int dialogue)
{

    printf("Client Web connecté !");
    gestionClientWeb((void *)&dialogue);
}


/** Main procedure **/

int main(void){

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








