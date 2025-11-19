# WordleSolver_ALGO3
# 🎮 Mon Jeu Wordle
![featured-wordle-solver-tool](https://github.com/user-attachments/assets/0a818433-ea8d-455e-92a2-ccf53889a440)


Ce projet implémente un jeu Wordle complet ainsi qu'un solveur intelligent capable de deviner les mots en un nombre réduit d'essais. Le programme utilise un dictionnaire externe, calcule des feedbacks comme le vrai Wordle (vert, jaune, gris), et propose plusieurs modes d'utilisation.

  # 🚀 Fonctionnalités

 ### 1\ Mode Joueur : 
le joueur essaie de deviner un mot choisi aléatoirement.

### 2\ Mode Solveur : 
l'IA tente de deviner n'importe quel mot du dictionnaire.

### 3\ Benchmark : 
mesure les performances du solveur sur les 10 premiers mots.

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
<img width="1158" height="863" alt="Capture d&#39;écran 2025-11-19 104924" src="https://github.com/user-attachments/assets/5beb2517-b798-43bc-85e9-dd9a8377ea70" />

### 2. Mode Solveur (Le programme devine)

Le programme utilise une stratégie pour trouver le mot secret avec le moins d'essais possible.
<img width="1103" height="521" alt="Capture d&#39;écran 2025-11-19 104943" src="https://github.com/user-attachments/assets/a413a1ce-0c62-4ac5-b311-f3c8b78f450c" />

### 3. Benchmark (Évaluation de la performance)

Évalue la performance du solveur sur une liste de mots.
<img width="1150" height="849" alt="Capture d&#39;écran 2025-11-19 105045" src="https://github.com/user-attachments/assets/6ff35e4a-1b0a-4099-a4cf-c993dee11ced" />
<img width="1147" height="839" alt="Capture d&#39;écran 2025-11-19 105108" src="https://github.com/user-attachments/assets/390f92c1-8f6c-4ee5-8f8b-401f4835c11d" />

