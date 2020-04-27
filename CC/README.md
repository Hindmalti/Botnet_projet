# Tâches du CC  

1 - Il envoie un msg en TCP au bot (client tcp).

2 - Boucle d'écoute UDP qui récupère la structure du bot (envoyée) (ecoute_udp.c).

3 - Stocke dans une liste chainée la liste des bots, puis transforme la liste en un tableau.

4 - Ecrit le nombre de bots sur une shared memory.

5 - Ecrit le tableau des bots dans une autre shared memory.

6 - Si le CC reçoit le signal `SIGUSR1` alors il va lire dans la shared memory du serveur_web.

7 - Découpe le paquet envoyé par le serveur Web et envoie les ordres au bot via (protocole.c).

