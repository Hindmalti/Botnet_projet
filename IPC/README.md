## Fonctionnement 
Deux types de signaux : 
* Signal comme quoi j'écris sur la shm => occupée
* Signal comme quoi j'ai fini d'écrire => libre
rajouter d'autres signaux ? comme quoi faut aller lire la shm ? la liberer ? 
## Deroulement 

Faut-il différentes shm ? une spécifique pour les ID, une pour la data , une pour les PID ? 
* Le CC crée une shm 
* Ecrit dessus son PID_CC
* Le Serveur récupère le PID_CC 
* Le serveur écrit le PID_SRV
* Le CC envoie un signal au serveur comme quoi il va écrire 
* Le serveur écoute le signal et saura qu'il faut pas écrire jusqu'au signal suivant 
* Le CC écrit le nmbre de bots dispos 
* Le CC écrit les ID des bots 
* Le CC envoie un signal comme quoi il a finit d'écrire 
* Le serveur recupère le signal qu'il peut accéder à la shm
* Le serveur recupère la data écrite sur la shm
* Le serveur clean la shm 