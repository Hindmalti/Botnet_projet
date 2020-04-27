# Botnet
Ce projet destiné aux IMA4-SC vise à nous exercer à créer l'architecture d'un serveur Command&Control qui permet de gérer à distance des Bots (qu'on admet installés au préalable sur une machine distante).

## Mode de fonctionnement

* Les bots, envoient chaque 5 s en Broadcast leurs ID(uniques), temps de vie ainsi que leur statut.
* Le serveur écoute l'UDP et stocke les bots recus dans une liste chainée.
* Il écrit sur une shared memory le nombre de bots existants, puis leurs informations sous forme d'un tableau à destination du serveur web.
* Un serveur web est également implémenté, il lit la shared memory que le cc a écrit.
* Le serveur web crée des boutons en fonction du nombre de bots existants et propose d'envoyer un ordre à l'un d'eux.
* Une fois l'odre choisi, le serveur web écrit sur une shared memory : `commande|filename|ID_bot` et envoie un signal `SIGUSR1` indiquant au CC qu'il peut lire la shared memory
* Lorsque le CC reçoit le signal `SIGUSR1`, il doit alors lire la shared memory et découper le contenu de la shared memory
* Il transmet ensuite l'ordre au bon bot et avec le bon nom de fichier.
* Le BOT écoute en boucle le TCP et dès qu'il reçoit un ordre il l'exécute.

Dans chaque dossier vous trouverez un README.md détaillant le fonctionnement de ses fonctions.

## Les ordres possibles 

* Demander le statut du bot
* Demander au bot d'installer une charge utile
* Demander au bot d'exécuter une charge utile 
* Demander au bot de donner le résultat d'une charge utile 
* Demander au bot de supprimer une charge utile
* Demander au bot de quitter une connexion

## IMPORTANT (limites)
                                /!\ IMPORTANT /!\
 La chaine entière FONCTIONNE pour l'odre 1 (demander le statut), les autres ordres **fonctionnent**. 

 Dans l'état actuel des choses, ça ne fonctionne pas pour la simple raison que le http rajoute le HEADER CONTENT-DISPOSITION (https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Disposition) au fichier.so et donc le bot n'arrive pas à l'installer et l'exéctuer.
 Nous avons tout de même réussi à retirer le début du header, mais notre fichier.so reste modifié et nous n'arrivons pas à résoudre le problème.

## Manuel d'exécution des tests

* Compiler tout le projet à la racine
```bash
make clean all
```
* Executer le bot en premier temps
 ```bash
./bot
```
* Dans un autre terminal executer le cc
```bash
./cc
```
* Dans un autre terminal encore, executer le serveur web
```bash
./serveur_web
```
* Mettre sur votre navigateur l'adresse suivante 
```bash
localhost:8000/acceuil.html
```
* Choisissez votre bot et envoyez lui la commande statut.

### Pour générer une charge utile 

* Aller dans le dossier Admin
* Compiler le fichier example_charge.c en une lib.so avec la commande 
```bash
gcc -shared -o filename.so -fPIC example_charge.c
```

## Protocole 

### UDP 
* Le Bot envoie en broadcast
* Le CC écoute l'udp sur le port 4242
### TCP 
* Le CC envoie des ordres en TCP sur le port 4242 du Bot

* Le Bot écoute les ordres TCP sur 4242 
## Points forts du projet
* Le code compile entièrement en -Wall -pedantic -Wextra
* Mise en place des listes chainées pour l'allocution dynamique des bots et charge utile dans la mémoire.
* L'indentation, les make-debug, les debug conditionnels sont bien en place
* Nous avons tenté de découper les fichiers de manière modulaire, de les rendre lisibles, et faire un maximum de wrappers et de macros.
* Les libraries Network, Thread, IPC sont génériques.

## Axes d'amélioration du projet (Points faibles)
Faute de temps, certaines fonctionalités aurait pu être améliorées :

* Remplacer les listes chainées par des arbres binaires afin de rendre la complexité de recherche en nlog(n) au lieu de n, et ce en faisant une recherche dichotomique.
* Afficher le statut et le résultat sur le serveur Web (pour le moment seul un printf sur le terminal du cc s'effectue).

* La librairie des Listes n'est pas assez générique (elle dépend des types info_bot_t, bot_t etc), mais nous n'avons pas trouvé une autre solution pour le moment.
* La librairie des files de messages a été écrite mais non implantée au fonctionnement (faute de temps).

## Auteurs 
Ahouassou Loris - Hind MALTI Avril 2020