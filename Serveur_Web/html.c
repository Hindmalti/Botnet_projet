/*
1) il faut implanter la méthode POST, c'est pas très compliqué, 
il faut lire la taille de la donnée envoyé par le navigateur dans 
l'entête content-length puis faire une boucle avec un fread pour 
lire autant d'octets qu'attendu (après avoir lu la ligne vide 
qui termine les entêtes du navigateur)


2) il faut être capable de retrouver le fichier 
envoyé dans le mime-multipart, pour cela repérez 
la chaine de séparation qui est donnée dans les entêtes


Elle se trouve dans le content-type, sous-information boundary
chercher dans ce que le navigateur a envoyé la chaine name="nomdevotreinputfile"

La méthode propre est de chercher les chaînes "boundary" puis de lire les entêtes du document courant,
puis de lire les données jusqu'au bondary suivant


Attention devant chaque chaîne de délimitation il y 
a deux - (--) supplémentaires, il y a aussi deux - (--) à 
la fin de la dernière chaîne de séparation

voir la section mime dans le chapitre protocoles applicatifs du cours de réseau S7
/*