# Tâches du BOT

1 - Ce bot envoie sa structure en broadcast dans un while 1=> ceci tourne sur un Thread.

2 - Lancer une boucleServeur TCP afin de gérer les nouvelles connexion des clients , établi une connexion et lit le message envoyé => 1 thread qui lancera ensuite 1 thread par client (prévoir probablement un wrapper)

3/ Il devra plus tard récupérer des charges utiles et des ordres du CC Traiter les commandes recues => 1 thread (prévoir un wrapper)

