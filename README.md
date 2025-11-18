# WordleSolver_ALGO3
# 🎮 Mon Jeu Wordle
![Description](featured-wordle-solver-tool.jpg)

Ce projet implémente un jeu Wordle complet ainsi qu'un solveur intelligent capable de deviner les mots en un nombre réduit d'essais. Le programme utilise un dictionnaire externe, calcule des feedbacks comme le vrai Wordle (vert, jaune, gris), et propose plusieurs modes d'utilisation.

  # 🚀 Fonctionnalités

1\ Mode Joueur : le joueur essaie de deviner un mot choisi aléatoirement.

2\ Mode Solveur : l'IA tente de deviner n'importe quel mot du dictionnaire.

3\ Benchmark : mesure les performances du solveur sur les 100 premiers mots.

  🎨 Couleurs ANSI pour simuler Wordle en console (vert, jaune, gris).

  📁 Dictionnaire externe : facile à modifier.

# ▶️ Compilation et exécution
 Compiler : 
   -----gcc main.c wordle.c -o wordle
 
 Lancer le jeu : 
   ------./wordle words.txt
 # 🎯 Règles du jeu Wordle

Le mot contient WORD_LEN lettres.

À chaque essai :

🟩 Vert : bonne lettre, bonne position.

🟨 Jaune : bonne lettre, mauvaise position.

⬜ Gris : lettre absente.

Le joueur a MAX_GUESSES tentatives.
# 📣 Auteur

Projet réalisé par Imad Eddine Bara et Alim Kamel
# execution
### 1. Mode Joueur (Deviner le mot secret) 
L'utilisateur tente de deviner le mot secret en 6 essais.
  <img width="1290" height="594" alt="Capture d&#39;écran 2025-11-18 115817" src="https://github.com/user-attachments/assets/1d088adf-166b-48af-9bd3-8fad96585571" />
  <img width="1294" height="596" alt="Capture d&#39;écran 2025-11-18 115903" src="https://github.com/user-attachments/assets/8da5cea3-a493-4ebe-abb9-d2c8ff1bed33" />
### 2. Mode Solveur (Le programme devine)

Le programme utilise une stratégie pour trouver le mot secret avec le moins d'essais possible.
<img width="1237" height="422" alt="Capture d&#39;écran 2025-11-18 120026" src="https://github.com/user-attachments/assets/d257f67d-64ed-47ac-900c-0dc9e1281b7a" />
### 3. Benchmark (Évaluation de la performance)

Évalue la performance du solveur sur une liste de mots.
<img width="1217" height="823" alt="Capture d&#39;écran 2025-11-18 120144" src="https://github.com/user-attachments/assets/fd15109f-c202-409e-a862-b66fd2c049a5" />
<img width="1254" height="712" alt="Capture d&#39;écran 2025-11-18 120155" src="https://github.com/user-attachments/assets/a274f3ab-0667-4442-add0-33af50bba381" />


