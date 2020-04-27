
# Librairie des shared memory 

## Objectifs

Cette librairie nous est utile pour la communication entre serveurWeb et CC.
En effet, le CC a besoin de transmettre le nbre de bots et leurs ID
Le Serveur Web a son tour a besoin d'écrire l'ordre à exécuter , l'ID du bot et le nom de fichier ( à executer, supprimer ou pour recupérer le résultat de son execution)

## Contenants : 

Les signatures des différentes fonctions se trouvent dans le .h, chaque fonction dans le .c est documentée en commentaire. Les fonctions de cette lib servent à : 

* int getShm : initialiser et créer une shared memory 
* int ecritureShm : écrire sur une shared memory
* int lectureShm : Lire depuis une shared memory

# Librairie des Files de messages 

## Objectif 

Libipc regroupe toutes les fonctions nécessaires à la communication par file de messages.
Elles auraient servi, si l'on avait implémenté une application afin de donner des ordres au bot sans passer par le serveur Web, mais en IPC de manière interne avec le CC.
Ce sont des fonction prêtes à l'emploi, bien qu'on ne les ais pas intégrées dans l'admin.

## Contenants 

1 - Les fonctions de lecture de requête (pour que le CC lise la commande censé provenir de l'admin)

2 - Lecture de réponse (pour que l'admin lise le retour du CC)
De même

3 -Les fonctions d'écriture de requête (de l'admin vers le CC)

4 - Fonction D'écriture de réponse (du CC vers l'admin)