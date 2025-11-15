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
