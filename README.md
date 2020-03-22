## Protocole 

7 6 5 4 3 2 1 0
X X X X X X _ _     
            S E

E = 0 (Inactif) ou 1 (Actif)
S = 0 (Début) ou 1 (Termine)


cat ProtocoleUDP Port utilisé : 4242 Contenu du paquet UDP : identifiant du bot (unique) : 6 octets temps de vie (nombre de secondes depuis le démarrage) : 4 octets

 cat ProtocoleTCP Port TCP bot : 4242 Commandes : STAT : retourne le status du bot (la réponse comprend son temps de vie, le nombre de charges utiles téléversées, le nombre d'exécution de charges utiles) QUIT : deconnexion du C&C par rapport au bot
