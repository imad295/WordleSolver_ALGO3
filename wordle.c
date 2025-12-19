#include <stdio.h>      // Entrées/sorties standards (printf, scanf, fopen)
#include <stdlib.h>     // Gestion de la mémoire (malloc, free) et utilitaires
#include <string.h>     // Manipulation de chaînes de caractères (strcmp, strlen)
#include <ctype.h>      // Fonctions de test et conversion de caractères (tolower, toupper)
#include <stdbool.h>    // Type booléen (true, false)
#include <time.h>       // Gestion du temps (pour l'aléatoire avec time())
#include "wordle.h"     // Inclusion des définitions propres au projet

#ifdef _WIN32           // Si on est sous Windows
#include <windows.h>    // Inclusion de l'API Windows
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004 // Pour supporter les couleurs ANSI
#endif
#endif

char **dictionary = NULL; // Tableau dynamique de chaînes de caractères pour les mots
int dict_size = 0;        // Compteur du nombre de mots chargés

/* ---------------- Gestion du Dictionnaire ---------------- */
int load_dictionary(const char *filename)
{
    FILE *f = fopen(filename, "r");                     // Ouvre le fichier en mode lecture ("r")
    if (!f) return -1;                                  // Si le fichier n'existe pas, retourne une erreur
    dictionary = malloc(sizeof(char *) * MAX_WORDS);    // Alloue l'espace pour stocker les pointeurs de mots
    char buf[128];                                      // Buffer temporaire pour lire chaque ligne
    dict_size = 0;                                      // Réinitialise la taille à 0
    while (fgets(buf, sizeof(buf), f))                  // Lit le fichier ligne par ligne
    {
        buf[strcspn(buf, "\r\n")] = 0;                  // Supprime les retours à la ligne invisibles (\n ou \r)
        for (int k = 0; buf[k]; k++)                    // Parcourt chaque lettre du mot lu
            buf[k] = tolower((unsigned char)buf[k]);    // Convertit la lettre en minuscule
        if ((int)strlen(buf) == WORD_LEN && dict_size < MAX_WORDS) // Si la longueur est correcte
            dictionary[dict_size++] = strdup(buf);      // Duplique le mot en mémoire et l'ajoute au dico
    }
    fclose(f);                                          // Ferme le fichier proprement
    return dict_size;                                   // Retourne le nombre total de mots valides
}

void free_dictionary()
{
    if (!dictionary) return;                            // Si le dico est déjà vide, ne rien faire
    for (int i = 0; i < dict_size; i++)                 // Parcourt chaque mot stocké
        free(dictionary[i]);                            // Libère la mémoire de chaque chaîne
    free(dictionary);                                   // Libère le tableau de pointeurs principal
}

bool is_valid_word(const char *w)
{
    for (int i = 0; i < dict_size; i++)                 // Parcourt tout le dictionnaire
        if (strcmp(dictionary[i], w) == 0)              // Compare le mot donné avec le mot actuel
            return true;                                // Si trouvé, le mot est valide
    return false;                                       // Sinon, il n'existe pas dans la liste
}

/* ---------------- Logique du Feedback ---------------- */
void compute_feedback(const char *guess, const char *target, Color colors[])
{
    int count[26] = {0};                                // Tableau pour compter les lettres dispos dans target
    for (int i = 0; i < WORD_LEN; i++)                  // 1ère boucle : vérification des lettres bien placées
    {
        colors[i] = GRAY;                               // Par défaut, la couleur est grise
        if (guess[i] == target[i])                      // Si la lettre est à la bonne place
            colors[i] = GREEN;                          // On marque VERT
        else
            count[target[i] - 'a']++;                   // Sinon, on compte cette lettre comme "disponible" ailleurs
    }
    for (int i = 0; i < WORD_LEN; i++)                  // 2ème boucle : vérification des lettres mal placées
    {
        if (colors[i] != GREEN && count[guess[i] - 'a'] > 0) // Si pas vert et lettre présente ailleurs
        {
            colors[i] = YELLOW;                         // On marque JAUNE
            count[guess[i] - 'a']--;                    // On consomme une occurrence de cette lettre
        }
    }
}

void print_feedback(const char *guess, Color colors[])
{
    for (int i = 0; i < WORD_LEN; i++)                  // Parcourt les lettres du mot proposé
    {
        if (colors[i] == GREEN)                         // Si VERT
            printf("\x1b[42m\x1b[30m%c\x1b[0m", toupper(guess[i])); // Fond vert, texte noir
        else if (colors[i] == YELLOW)                   // Si JAUNE
            printf("\x1b[43m\x1b[30m%c\x1b[0m", toupper(guess[i])); // Fond jaune, texte noir
        else                                            // Si GRIS
            printf("\x1b[100m\x1b[37m%c\x1b[0m", toupper(guess[i])); // Fond gris, texte blanc
        printf(" ");                                    // Espace entre les lettres
    }
    printf("\n");                                       // Saut de ligne à la fin du mot
}

/* ---------------- SOLVEUR LOGIQUE PAR ÉLIMINATION ---------------- */
int solver_user_feedback()
{
    bool *possible = malloc(dict_size * sizeof(bool));  // Alloue un tableau de drapeaux (vrai/faux)
    for (int i = 0; i < dict_size; i++)                 // Initialise tous les mots
        possible[i] = true;                             // Tous les mots sont possibles au départ

    printf("\n--- SOLVEUR LOGIQUE ---\nIndices : g=vert, y=jaune, b=gris\n");

    for (int turn = 1; turn <= MAX_GUESSES; turn++)     // Limite à 6 essais maximum
    {
        int remaining = 0;                              // Compteur de mots restants
        int best_idx = -1;                              // Index du premier mot suggéré

        for (int i = 0; i < dict_size; i++)             // Parcourt le dico pour compter les candidats
        {
            if (possible[i])                            // Si le mot est toujours valide
            {
                if (best_idx == -1) best_idx = i;       // Sélectionne le premier mot valide trouvé
                remaining++;                            // Incrémente le compteur
            }
        }

        if (best_idx == -1)                             // Si plus aucun mot ne correspond
        {
            printf("\x1b[31mErreur : Aucun mot trouvé ! Vérifiez vos indices.\x1b[0m\n");
            free(possible);                             // Libère la mémoire
            return 1;                                   // Quitte avec une erreur
        }

        printf("\n(Mots possibles restants : %d)\n", remaining); // Affiche l'état du filtrage
        char *guess = dictionary[best_idx];             // Récupère le mot suggéré
        printf("Essai %d : \x1b[1m%s\x1b[0m\nFeedback : ", turn, guess); // Affiche la suggestion

        char fb[WORD_LEN + 1];                          // Buffer pour le feedback utilisateur (ex: "gbybb")
        if (scanf("%5s", fb) != 1) { free(possible); return 1; } // Lit la réponse

        if (strcmp(fb, "ggggg") == 0)                   // Si tout est vert (g)
        {
            printf("\x1b[32mVictoire ! Le mot était bien : %s\x1b[0m\n", guess);
            free(possible);                             // Libère la mémoire
            return 0;                                   // Succès
        }

        for (int i = 0; i < dict_size; i++)             // FILTRAGE : Compare chaque mot possible au feedback reçu
        {
            if (!possible[i]) continue;                 // Saute les mots déjà éliminés

            Color sim_colors[WORD_LEN];                 // Tableau pour simulation
            compute_feedback(guess, dictionary[i], sim_colors); // Simule le feedback si dico[i] était la cible

            char sim_fb[WORD_LEN + 1];                  // Convertit les couleurs simulées en chaîne (g/y/b)
            for (int j = 0; j < WORD_LEN; j++)
            {
                if (sim_colors[j] == GREEN) sim_fb[j] = 'g';
                else if (sim_colors[j] == YELLOW) sim_fb[j] = 'y';
                else sim_fb[j] = 'b';
            }
            sim_fb[WORD_LEN] = '\0';                    // Termine la chaîne simulée

            if (strcmp(fb, sim_fb) != 0)                // Si le feedback réel est différent de la simulation
                possible[i] = false;                    // Alors ce mot du dictionnaire est impossible
        }
    }
    free(possible);                                     // Libère la mémoire à la fin
    return 0;
}

/* ---------------- Menu Principal ---------------- */
void human_play()
{
    char *target = dictionary[rand() % dict_size];      // Choisit un mot cible aléatoirement
    char guess[64];                                     // Buffer pour la saisie utilisateur
    printf("\nDevinez le mot (%d lettres)\n", WORD_LEN);
    for (int t = 1; t <= MAX_GUESSES; t++)              // Boucle des 6 essais
    {
        printf("Essai %d> ", t);
        scanf("%63s", guess);                           // Lit la proposition du joueur
        for (int i = 0; guess[i]; i++)                  // Met en minuscule
            guess[i] = tolower((unsigned char)guess[i]);
        if (!is_valid_word(guess))                      // Vérifie si le mot existe dans le dictionnaire
        {
            printf("Mot absent du dico.\n");
            t--;                                        // Ne compte pas l'essai
            continue;                                   // Relance la saisie
        }
        Color c[WORD_LEN];                              // Tableau de couleurs
        compute_feedback(guess, target, c);             // Calcule le résultat
        print_feedback(guess, c);                       // Affiche le mot coloré
        if (strcmp(guess, target) == 0)                 // Si trouvé
        {
            printf("Gagné !\n");
            return;                                     // Sort du mode jeu
        }
    }
    printf("Perdu ! C'était : %s\n", target);           // Affiche la solution en cas de défaite
}

int main(int argc, char **argv)
{
    if (argc < 2)                                       // Si aucun fichier n'est passé en paramètre
    {
        printf("Usage: %s <dico.txt>\n", argv[0]);      // Affiche l'aide
        return 1;
    }
    if (load_dictionary(argv[1]) <= 0)                  // Tente de charger les mots
    {
        printf("Dictionnaire vide.\n");                 // Si échec ou vide
        return 1;
    }
    srand((unsigned int)time(NULL));                    // Initialise l'aléatoire sur l'horloge système

    int choice;
    while (1)                                           // Boucle infinie du menu
    {
        printf("\n=== WORDLE C ===\n");                 // Titre
        printf("\n | 1. Jouer\n | 2. Solveur\n | 3. Quitter \n Choix : ");
        if (scanf("%d", &choice) != 1)                  // Lit le choix. Si ce n'est pas un nombre :
        {
            while (getchar() != '\n');                  // Vide le buffer d'entrée pour éviter boucle infinie
            continue;                                   // Relance le menu
        }
        if (choice == 1) human_play();                  // Lance le jeu
        else if (choice == 2) solver_user_feedback();   // Lance l'assistant
        else if (choice == 3) break;                    // Quitte la boucle
    }
    free_dictionary();                                  // Nettoie la mémoire avant de quitter
    return 0;                                           // Fin du programme
}
