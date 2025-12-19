# WordleSolver_ALGO3
# 🎮 Mon Jeu Wordle
![featured-wordle-solver-tool](https://github.com/user-attachments/assets/0a818433-ea8d-455e-92a2-ccf53889a440)


Ce projet implémente un jeu Wordle complet ainsi qu'un solveur intelligent capable de deviner les mots en un nombre réduit d'essais. Le programme utilise un dictionnaire externe, calcule des feedbacks comme le vrai Wordle (vert, jaune, gris), et propose plusieurs modes d'utilisation.

  # 🚀 Fonctionnalités

 ### 1\ Mode Joueur : 
le joueur essaie de deviner un mot choisi aléatoirement.

### 2\ Mode Solveur : 
l'IA tente de deviner n'importe quel mot du dictionnaire.

  🎨 Couleurs ANSI pour simuler Wordle en console (vert, jaune, gris).

  📁 Dictionnaire externe : facile à modifier.

# ▶️ Compilation et exécution
 ### Compiler : 
 gcc wordle.c -o wordle
 
### Lancer le jeu : 
 ./wordle words.txt
 # 🎯 Règles du jeu Wordle

Le mot contient WORD_LEN lettres.

À chaque essai :

🟩 Vert : bonne lettre, bonne position.

🟨 Jaune : bonne lettre, mauvaise position.

⬜ Gris : lettre absente.

Le joueur a MAX_GUESSES tentatives.
# 📣 Auteur

Projet réalisé par Imad Eddine Bara et Alim Kamel
# 🧩execution
### 1. Mode Joueur (Deviner le mot secret) 
L'utilisateur tente de deviner le mot secret en 6 essais.
<img width="1224" height="544" alt="Capture d&#39;écran 2025-12-19 153452" src="https://github.com/user-attachments/assets/6b6d8060-3dc7-4564-8386-716fcb7287dd" />

### 2. Mode Solveur (Le programme devine)

Le programme utilise une stratégie pour trouver le mot secret avec le moins d'essais possible.

<img width="1314" height="702" alt="Capture d&#39;écran 2025-12-19 155241" src="https://github.com/user-attachments/assets/f9d51eac-3f9d-460f-ad2f-e98ddcc22aeb" />




