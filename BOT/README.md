## Fonctions du BOT

Ce bot envoie sa structure en broadcast (pour le moment il envoie un msg "Hello from BOT") => ceci tourne sur un Thread.

Il a également un seveur TCP qui écoute sur le port 2020  son client (le CC), établi une connexion et lit le message envoyé 
Il devra plus tard récupérer des charges utiles et des ordres du CC