#include <sys/types.h>
#include <sys/ipc.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <liblistes.h>
#include <stdio.h>
#include <libshm.h>
#include <libthrd.h>
#include <libnetwork.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "cc.h"

extern liste_bot_t list_bot;

/** char **str_split(char *a_str, const char a_delim)
 * Fonction permettant de découper une chaine de caractère 
 * param la chaine de caractère à délimiter
 * param le symbole permettant de délimiter (,) pour notre cas
 * return le resulat du découpage
 * 
*/
//https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char **str_split(char *a_str, const char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

/** void getOrdreFromShm()
 *  Fonction qui récupère les ordres écrit sur la shm
 * 
*/

void getOrdreFromShm()
{
    char *data;
    if (lectureShm(KEY_DATA, (void *)&data, 1024) != 0)
    {
        DEBUG_PRINT(("[getOrdreFromShm]: Impossible de lire la shm\n"));
        return;
    }
    if (strlen(data) == 0)
    {
        DEBUG_PRINT(("[getOrdreFromShm]:La shared memory est vide\n"));
        return;
    }
    DEBUG_PRINT(("[getOrdreFromShm]Reception de l'odre %s depuis le serveur WEB\n", data));
    char data_2[SIZE_DATA];
    strcpy(data_2, data);
    //Clean de la shm
    char **tokens;
    tokens = str_split(data_2, ',');
    char id[8];
    ordre_t ordre;
    if (tokens)
    {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            // data[0] => La CMD à envoyer au bot
            if (i == 0)
            {
                //*(tokens + i) ==> CMD
                ordre.cmd = (char)**(tokens + i);
            }
            // data[2] => l'ID du bot
            if (i == 2)
            {
                //rechercher bot
                strcpy(id, (char *)*(tokens + i));
                id[strlen((id))-1]='\0';
                rechercheBOT(id, &list_bot, &(ordre.bot));
                if (ordre.bot != NULL)
                {
                    print_BOT_structure(ordre.bot);
                    send_command_tcp(&ordre);
                }
            }
            // data[1] => Le filename à envoyer au bot
            if (i == 1)
            {
                strcpy(ordre.filename, (char*)*(tokens + i));
                printf("filename est : %s\n", ordre.filename);
                //ordre.filename = (char) **(tokens + i);
            }

            free(*(tokens + i));
        }
        free(tokens);
    }
}
/**
 * int traitementUDP(struct sockaddr_in *addr, void *payload, int taille)
 * Fonction de traitement de la réception UDP, stocke les bots reçus dans une liste chainée
 * elle convertit la liste en tableau pour l'écrire ensuite sur la shm
 * param adresse ip 
 * param payload l'info reçu du bot
 * param taille de la reception (argument utile pour la librairie Network)
 */
int traitementUDP(struct sockaddr_storage addr, void *payload, int taille)
{
    //Le payload a ete alloc, il faut le free
    info_bot_t *info_bot = (info_bot_t *)payload; //Ok car sera free par les fonctions de destruction de listes
    struct sockaddr_in *vrai_addr = (struct sockaddr_in *)&addr;
    bot_t *bot = (bot_t *)malloc(sizeof(bot_t)); // Ok car sera free par les fonctions de destruction de listes
    bot->info = info_bot;
    bot->addr = *vrai_addr;
    DEBUG_PRINT(("[traitementUDP]Affichage bot recu\n"));
    //print_BOT_structure(bot);
    (void)taille;
    // Verifier qu'on n'a pas deja le bot avant de l'ajouter à la liste
    bot_t *tmp;
    rechercheBOT(bot->info->ID, &list_bot, &tmp);
    if (tmp == NULL)
    {
        DEBUG_PRINT(("[traitementUDP] La recherche bot n'a pas abouti"));
        print_listeBot(list_bot);
        // On renseigne le bot sur la liste chainée
        ajout_tete_bot(&list_bot, bot);
        int nbre_bot = comptageNbreBot(list_bot);
        void *mem_adr, *mem_adr2;
        getShm(KEY_NBRE_BOT, sizeof(int), &mem_adr2);
        //On écrit le nbre de bots dessus
        ecritureShm(mem_adr2, (void *)&nbre_bot, sizeof(int));
        // on crée une liste
        info_bot_t *array_bots = malloc(nbre_bot * sizeof(info_bot_t));
        llist_bot_to_array(list_bot, array_bots);
        getShm(KEY, NBRE_MAX_BOT * sizeof(info_bot_t), &mem_adr);
        ecritureShm(mem_adr, (void *)array_bots, nbre_bot * sizeof(info_bot_t));
    }
    return 0;
}

/**
 * void lancementBoucleServeurUDP(void *s)
 * Fonction wrapper permettant de donner les arguments à  
 * la fct boucleServeurUDP vu que elle est lancée dans un thread
 *
 * param pointeur vers la socket UDP
 * 
 */
void lancementBoucleServeurUDP(void *s)
{
    int socket_udp = *((int *)s);
    //On indique à boucleServeur qu'on s'attend à recevoir des info_bot_t
    boucleServeurUDP(socket_udp, traitementUDP, sizeof(info_bot_t));
}

/**
 * void partie_udp()
 * Fonction wrapper qui initialise le serveur UDP dans un thread
 * 
 */
void partie_udp()
{
    char *port_udp = UDP_PORT_ECOUTE;
    printf("############ Partie UDP ############\n");
    int socket_udp = initialisationServeurUDP(port_udp);
    if (socket_udp < 0)
    {
        fprintf(stderr, "Initialisation du serveur UDP impossible\n");
        exit(-1);
    }

    //Lancement de la boucle d'ecoute UDP dans un thread
    if (lanceThread(lancementBoucleServeurUDP, (void *)&socket_udp, sizeof(socket_udp)) < 0)
    {
        perror("boucleServeurUDP.lanceThread");
        exit(-1);
    }
}