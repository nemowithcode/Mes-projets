# 🚗 RoadRush

**RoadRush** est un jeu développé en C++ avec Qt, dans lequel le joueur contrôle une voiture qui doit éviter des obstacles, ramasser des bonus (*Perks*) et survivre le plus longtemps possible. Il dispose d'une interface graphique moderne, d'une version textuelle, et d’un système complet de sauvegardes.

---

## 🧩 Fonctionnalités

- Déplacement du véhicule avec détection de collision
- Obstacles dynamiques générés aléatoirement
- Système de bonus : turbo, ralentissement, invincibilité
- Interface graphique via Qt (`MainWindow`, `MainTextuel`)
- Système de sauvegarde automatique et manuelle avec JSON
- Tests unitaires `testRegression()` pour chaque classe
- Documentation technique générée avec Doxygen

### Prérequis

- Doxygen (optionnel pour la doc)

## ⚙️ Lancement du jeu

**Exécuter le programme facilement sans make**

1.  **Générer le Makefile (si nécessaire) :**
    Si aucun fichier `Makefile` n'est présent à la racine du projet, exécutez :
    ```powershell
    .\qt_tools\qmake.exe TESTRUSH.pro
    ```
    Si un `Makefile` existe déjà dans le dossier `build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug`, vous pouvez passer à l'étape suivante.

2.  **Compiler le projet avec `mingw32-make` :**
    ```powershell
    cd build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug
    ..\..\qt_tools\mingw32-make.exe -f Makefile -j8 # Ajustez le nombre de cœurs si nécessaire
    ```
    **Important :** Assurez-vous d'exécuter ces commandes dans l'ordre, en naviguant d'abord vers le dossier de build.

3.  **Exécuter l'exécutable :**
    ```powershell
    cd debug
    .\TESTRUSH.exe
    ```