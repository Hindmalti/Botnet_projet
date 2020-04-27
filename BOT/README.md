# Tâches du BOT

1 - Ce bot envoie sa structure en broadcast dans un while 1 qui tourne dans un Thread. (envoie_udp.c)

2 - Lancer une boucleServeur TCP afin de gérer les nouvelles connexion des clients , établi une connexion et lit le message envoyé => 1 thread qui lancera ensuite 1 thread par client (ecoute_tcp.c)

3 - Récupère les ordres du CC (le fichier protocole.c) contient les différentes commande et apelle la fonction adéquate.

4 - Le bot crée une liste de charges utiles et y enregistre les bots qu'il recoit. Il peut également, chercher dedans afin vérifier avant d'exécuter une charge s'il l'a déjà installée ou pas (de même pour la suppression). 

5 - Quand il reçoit l'ordre de quitter la connexion il free toutes les listes et les mémoires.