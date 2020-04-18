# Botnet

## Mode de fonctionnement

## Protocole 

* cat ProtocoleUDP Port utilisé : 4242 Contenu du paquet UDP : identifiant du bot (unique) : 6 octets temps de vie (nombre de secondes depuis le démarrage) : 4 octets

* cat ProtocoleTCP Port TCP bot : 4242 Commandes : STAT : retourne le status du bot (la réponse comprend son temps de vie, le nombre de charges utiles téléversées, le nombre d'exécution de charges utiles) QUIT : deconnexion du C&C par rapport au bot


## Axes d'amélioration du projet
Faute de temps, certaines fonctionalités aurait pu être améliorées : 

* Remplacer les listes chainées par des arbres binaires afin de rendre la complexité de recherche nlog(n) au lieu de n, et ce en faisant une recherche dicotomique.
* Enregistrer le resulat de l'execution d'une charge utile dans une structure : pour pourvoir renvoyer au CC tous les resultats de toutes les exécutions , et pas seulement le denier resultat(car pour le moment, le dernier écrase la valeur du précedent).
