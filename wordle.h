#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

/* --- Constantes de configuration --- */
#define WORD_LEN 5      // Longueur des mots (standard Wordle)
#define MAX_GUESSES 6   // Nombre maximum d'essais autorisés
#define MAX_WORDS 20000 // Capacité maximale du dictionnaire en mémoire

/* --- Types de données --- */

/**
 * Énumération des couleurs pour le feedback.
 * GRAY   : La lettre n'est pas dans le mot.
 * YELLOW : La lettre est présente mais mal placée.
 * GREEN  : La lettre est à la bonne place.
 */
typedef enum
{
    GRAY,
    YELLOW,
    GREEN
} Color;

/* --- Prototypes des fonctions --- */

// Gestion du dictionnaire
int load_dictionary(const char *filename);
void free_dictionary();
bool is_valid_word(const char *w);

// Logique de jeu et Feedback
void compute_feedback(const char *guess, const char *target, Color colors[]);
void print_feedback(const char *guess, Color colors[]);

// Helpers
bool has_correct_pos_somewhere(int pos, bool correct_pos[WORD_LEN][26]);

// Modes de jeu
void human_play();
int solver_user_feedback();

#endif // WORDLE_H
