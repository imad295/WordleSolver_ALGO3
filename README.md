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

### 2. Mode Solveur (Le programme devine)

Le programme utilise une stratégie pour trouver le mot secret avec le moins d'essais possible.
<img width="988" height="528" alt="Capture d&#39;écran 2025-11-23 225959" src="https://github.com/user-attachments/assets/25cd7b4a-d4a9-4561-8c50-04e6aa73e5e6" />


