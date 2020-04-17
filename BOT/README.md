# Tâches du BOT

1 - Ce bot envoie sa structure en broadcast dans un while 1=> ceci tourne sur un Thread.

2 - Lancer une boucleServeur TCP afin de gérer les nouvelles connexion des clients , établi une connexion et lit le message envoyé => 1 thread qui lancera ensuite 1 thread par client (prévoir probablement un wrapper)

3/ Il devra plus tard récupérer des charges utiles et des ordres du CC Traiter les commandes recues => 1 thread (prévoir un wrapper)

# Important

Pour que la librairie dynamique puisse être chargée, il faudra ajouter son emplacement dans le ./bashrc

Replace libraryname with the name of your library)

If you go the $LD_LIBRARY_PATH route, you'll want to put that into your ~/.bashrc file so it will run every time you log in:

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/library

//https://stackoverflow.com/questions/480764/linux-error-while-loading-shared-libraries-cannot-open-shared-object-file-no-s


https://stackoverflow.com/questions/384121/creating-a-module-system-dynamic-loading-in-c

