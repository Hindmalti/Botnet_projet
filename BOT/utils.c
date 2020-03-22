// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <string.h>

// #define SIZE 6

// typdef struct
// {
//     char ID[6];     //ID sur 6 octets
//     long life_time; // le temps de vie du bot depuis son existence
//     char etat;      // état du bot actif/inactif
// } info_bot_t;

// char *create_ID(char *str)
// {
//     const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//     for (int i = 0; i < SIZE; i++)
//     {
//         int key = rand() % (strlen(charset));
//         str[i] = charset[key];
//     }
//     return str;
// }

// int time_life()
// {
//     clock_t begin = clock();

//     /* here, do your time-consuming job */

//     clock_t end = clock();
//     double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
// }

// /*
// int main()
// {
//     srand(time(NULL)); //Initialisation nécessaire à faire une seule fois pour la fct rand
//     char str[SIZE];
//     strcpy(str, create_ID(str));
//     printf("%s\n", str);
//     return 0;
// }
// */


// //JE SUIS LE MAIN DU BOT C EST MOI LE BINAIRE QUI LANCE TOUT

// /* int main() {
//     int debut = clock() //Apell à la fonction Clock la première fois: début strict du demarage binaire
// renvoiStatusAuCC(socket, debut)


// int renvoiStatusAuCC(socket, debut) {
    
//     time_up = int getTime(debut);
//     //ECRIRE DANS LA STRUCTURE LE TEMPS ET ENVOYER A LA SOCKET
    
// }

// */
// int getTime(debut) {
//     return (clock() - debut) / CLOCKS_PER_SEC;
// }