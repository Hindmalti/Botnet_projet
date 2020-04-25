# PROCÉDURE d'utilisation du server Web

Coté serveur :

- make clean
- make
- lancer le serveur : ./serveur_web.out

Ensuite,

Coté navigateur web :
- ouvrir votre navigateur
- mettre l'url : localhost:8000/acceuil.html et valider
- Normalement le formulaire doit s'afficher. 

ATTENTION ! Veillez à toujours bien choisir votre bot dans la liste
avant de lancer une commande.

Enfin :
Pour observer ce qui se passe, retournez dans votre terminal où le serveur à été lancé.
Normalement la requête POST aura été effectuée et vous devriez voir les différents headers.
Vous pourrez voir le contenu de votre requete post.
Ce contenu à été stocké dans une variable pour être réutiliser plus tard.

Prochaines étapes :

- retirer les lignes contenant "content-disposition" dans le contenu du fichier.
- envoyer les données obtenues dans une shm au CC.