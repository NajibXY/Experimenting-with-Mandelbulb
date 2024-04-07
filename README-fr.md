# Génération et rendu d'un Mandelbulb en C++ et Python

## Auteur: [Najib El khadir](https://github.com/NajibXY)

## 1. Motivations

<figure text-align="right">
  
</figure>

Qui n'est pas fan des fractales ? </br>
Afin de m'exercer et d'en apprendre davantage sur les mathématiques derrière la génération et le rendu des fractales, j'ai travaillé sur cet exemple de génération de Mandelbulb. Mandelbulb est un type de fractale 3D émergeant des expérimentations visant à trouver une représentation canonique 3D de l'ensemble de Mandelbrot.
<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/2/21/Mandel_zoom_00_mandelbrot_set.jpg" width="200">
  </br>
  <em>Ensemble de Mandelbrot</em>
</p>

</br> </br>
</br>

## 2. Technologies Utilisées

- C++14, OpenMP
- Python, Numpy, Matplotlib, FFMPEG, Conda (pour mon environnement personnel)

## 3. Références

- [Fractales 3D estimées par distance](http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/)
- [Compréhension de la matrice de vue](https://www.3dgep.com/understanding-the-view-matrix/)
- [Défi Mandelbulb de The Coding Train](https://www.youtube.com/watch?v=NJCiUVGiNyA&ab_channel=TheCodingTrain)

## 4. Génération de données

- Les points de données du Mandelbulb sont assez faciles à générer.
- Mais pour pouvoir afficher ces données, une méthode de ray marching a été implémentée, qui implique le calcul des lumières et des distances selon le mouvement d'une caméra virtuelle. Cela nécessite de faire de la parallélisation avec OpenMP et beaucoup d'algèbre vectorielle tridimensionnelle pour obtenir un temps d'exécution acceptable.
- Il faut environ 30 minutes pour générer les données avec un exposant de base égal à 7.

### Compilation du fichier generate_data.cpp

- Nécessite C++ et gcc/g++ installés.
- Exécutez simplement :
  > g++ -Wall -Wextra -fopenmp generate_data.cpp -o generate_data.exe
- L'exécution de l'.exe générera les données dans le dossier /data/.

## 5. Affichage des données de rendu

### Configuration de votre environnement

- Vous devrez configurer un environnement Python (dans mon cas conda) pour afficher les données générées avec Matplotlib.
- Pour installer les dépendances avec conda, vous pouvez simplement exécuter :
  > conda create --name `<votre_nom_env>` --file requirements.txt
- Ou si vous utilisez pip :
  > pip install -r requirements.txt
- Ensuite, vous pourrez exécuter le script.

### Exécution du script Python

> python .\generate_video.py
- Cela générera tous les PNG rendus à partir des données précédemment générées. Ces images sont enregistrées dans le dossier /images/.
- De plus, le script compile ces images en une vidéo à l'aide de FFMPEG et la sauvegarde dans le même dossier que le script.
</br>

### Exemples d'images et de vidéo générées 

+ Exemple d'une vidéo générée :
   </br>
   </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/mandelbulb.gif" width="400">
  </br>
+ Exemples d'images générées et colorées aléatoirement :
  </br>
  </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/example1.png" width="350">
  </br>
  <img src="https://github.com/NajibXY/Mandelbulb-with-Ray-marching/blob/master/assets/example2.png" width="350"> 

</br></br>

## 5. Améliorations Possibles

- Génération de données plus rapide.
- Utilisation d'autres formules pour la génération de données.
- Utilisation d'autres méthodes de rendu.
- Utilisation d'une bibliothèque C++ efficace pour la visualisation des données conjointement à leurs calculs.
- Fournir la possibilité de tuner les paramètres de génération.
- [...]
