#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "wordle.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

char **dictionary = NULL;
int dict_size = 0;

/* ---------------- Gestion du Dictionnaire ---------------- */
int load_dictionary(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return -1;
    dictionary = malloc(sizeof(char *) * MAX_WORDS);
    char buf[128];
    dict_size = 0;
    while (fgets(buf, sizeof(buf), f))
    {
        buf[strcspn(buf, "\r\n")] = 0;
        for (int k = 0; buf[k]; k++)
            buf[k] = tolower((unsigned char)buf[k]);
        if ((int)strlen(buf) == WORD_LEN && dict_size < MAX_WORDS)
            dictionary[dict_size++] = strdup(buf);
    }
    fclose(f);
    return dict_size;
}

void free_dictionary()
{
    if (!dictionary)
        return;
    for (int i = 0; i < dict_size; i++)
        free(dictionary[i]);
    free(dictionary);
}

bool is_valid_word(const char *w)
{
    for (int i = 0; i < dict_size; i++)
        if (strcmp(dictionary[i], w) == 0)
            return true;
    return false;
}

/* ---------------- Logique du Feedback ---------------- */
// Cette fonction simule le résultat de Wordle
void compute_feedback(const char *guess, const char *target, Color colors[])
{
    int count[26] = {0};
    for (int i = 0; i < WORD_LEN; i++)
    {
        colors[i] = GRAY;
        if (guess[i] == target[i])
            colors[i] = GREEN;
        else
            count[target[i] - 'a']++;
    }
    for (int i = 0; i < WORD_LEN; i++)
    {
        if (colors[i] != GREEN && count[guess[i] - 'a'] > 0)
        {
            colors[i] = YELLOW;
            count[guess[i] - 'a']--;
        }
    }
}

void print_feedback(const char *guess, Color colors[])
{
    for (int i = 0; i < WORD_LEN; i++)
    {
        if (colors[i] == GREEN)
            printf("\x1b[42m\x1b[30m%c\x1b[0m", toupper(guess[i]));
        else if (colors[i] == YELLOW)
            printf("\x1b[43m\x1b[30m%c\x1b[0m", toupper(guess[i]));
        else
            printf("\x1b[100m\x1b[37m%c\x1b[0m", toupper(guess[i]));
        printf(" ");
    }
    printf("\n");
}

/* ---------------- SOLVEUR LOGIQUE PAR ÉLIMINATION ---------------- */
int solver_user_feedback()
{
    bool *possible = malloc(dict_size * sizeof(bool));
    for (int i = 0; i < dict_size; i++)
        possible[i] = true;

    printf("\n--- SOLVEUR LOGIQUE ---\nIndices : g=vert, y=jaune, b=gris\n");

    for (int turn = 1; turn <= MAX_GUESSES; turn++)
    {
        int remaining = 0;
        int best_idx = -1;

        // Compter les mots restants et choisir le premier disponible
        for (int i = 0; i < dict_size; i++)
        {
            if (possible[i])
            {
                if (best_idx == -1)
                    best_idx = i;
                remaining++;
            }
        }

        if (best_idx == -1)
        {
            printf("\x1b[31mErreur : Aucun mot trouvé ! Vérifiez vos indices.\x1b[0m\n");
            free(possible);
            return 1;
        }

        printf("\n(Mots possibles restants : %d)\n", remaining);
        char *guess = dictionary[best_idx];
        printf("Essai %d : \x1b[1m%s\x1b[0m\nFeedback : ", turn, guess);

        char fb[WORD_LEN + 1];
        if (scanf("%5s", fb) != 1)
        {
            free(possible);
            return 1;
        }

        if (strcmp(fb, "ggggg") == 0)
        {
            printf("\x1b[32mVictoire ! Le mot était bien : %s\x1b[0m\n", guess);
            free(possible);
            return 0;
        }

        // ÉLIMINATION : On ne garde que les mots qui produiraient le MÊME feedback
        for (int i = 0; i < dict_size; i++)
        {
            if (!possible[i])
                continue;

            Color sim_colors[WORD_LEN];
            compute_feedback(guess, dictionary[i], sim_colors);

            char sim_fb[WORD_LEN + 1];
            for (int j = 0; j < WORD_LEN; j++)
            {
                if (sim_colors[j] == GREEN)
                    sim_fb[j] = 'g';
                else if (sim_colors[j] == YELLOW)
                    sim_fb[j] = 'y';
                else
                    sim_fb[j] = 'b';
            }
            sim_fb[WORD_LEN] = '\0';

            if (strcmp(fb, sim_fb) != 0)
            {
                possible[i] = false;
            }
        }
    }
    free(possible);
    return 0;
}

/* ---------------- Menu Principal ---------------- */
void human_play()
{
    char *target = dictionary[rand() % dict_size];
    char guess[64];
    printf("\nDevinez le mot (%d lettres)\n", WORD_LEN);
    for (int t = 1; t <= MAX_GUESSES; t++)
    {
        printf("Essai %d> ", t);
        scanf("%63s", guess);
        for (int i = 0; guess[i]; i++)
            guess[i] = tolower((unsigned char)guess[i]);
        if (!is_valid_word(guess))
        {
            printf("Mot absent du dico.\n");
            t--;
            continue;
        }
        Color c[WORD_LEN];
        compute_feedback(guess, target, c);
        print_feedback(guess, c);
        if (strcmp(guess, target) == 0)
        {
            printf("Gagné !\n");
            return;
        }
    }
    printf("Perdu ! C'était : %s\n", target);
}
// ---------------- Programme Principal main ---------------- */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <dico.txt>\n", argv[0]);
        return 1;
    }
    if (load_dictionary(argv[1]) <= 0)
    {
        printf("Dictionnaire vide.\n");
        return 1;
    }
    srand((unsigned int)time(NULL));

    int choice;
    while (1)
    {
        printf("\n=== WORDLE C ===\n");
        printf("\n | 1. Jouer\n | 2. Solveur\n | 3. Quitter \n Choix : ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }
        if (choice == 1)
            human_play();
        else if (choice == 2)
            solver_user_feedback();
        else if (choice == 3)
            break;
    }
    free_dictionary();
    return 0;
}
