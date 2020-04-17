# PROCÉDURE d'utilisation du server Web

Coté serveur :

1 - make clean
2 - make
3 - lancer le serveur : ./serveur_web.out

Ensuite,

Coté navigateur web :

1 - ouvrir votre navigateur
2 - mettre l'url : localhost:8000/formulaire.html et valider
3 - Normalement le formulaire doit s'afficher. Là vous choisissez un fichier et vous envoyez

Enfin :

Pour observer ce qui se passe, retournez dans votre terminal où le serveur à été lancé.
Normalement la requête POST aura été effectuée et vous devrieez vous les différents headers.
Aussi Vous pourrez voir le contenu de votre fichier s'afficher en clair (dû à un printf).
Ce contenu à été stocké dans une variable pour être réutiliser plus tard.

Prochaines étapes :

- Récupérer le nom du fichier qui servira à nommer la charge
- retirer les lignes contenant "content-disposition" dans le contenu du fichier.
- envoyer les données obtenues dans une shm au CC.