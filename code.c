#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "wordle.h"

char **dictionary = NULL;
int dict_size = 0;

int load_dictionary(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        perror("open dictionary");
        return -1;
    }
    dictionary = malloc(sizeof(char *) * MAX_WORDS);
    char buf[64];
    dict_size = 0;
    while (fgets(buf, sizeof(buf), f))
    {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) == WORD_LEN && dict_size < MAX_WORDS)
            dictionary[dict_size++] = strdup(buf);
    }
    fclose(f);
    return dict_size;
}

bool is_valid_word(const char *w)
{
    for (int i = 0; i < dict_size; i++)
        if (strcmp(dictionary[i], w) == 0)
            return true;
    return false;
}
void free_dictionary()
{
    for (int i = 0; i < dict_size; i++)
        free(dictionary[i]);
    free(dictionary);
    dictionary = NULL;
    dict_size = 0;
}

void compute_feedback(const char *guess, const char *target, Color colors[])
{
    int count[26] = {0};
    for (int i = 0; i < WORD_LEN; i++)
        colors[i] = GRAY;
    for (int i = 0; i < WORD_LEN; i++)
    {
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

static bool compatible(const char *cand, const char *guess, Color colors[])
{
    Color test[WORD_LEN];
    compute_feedback(guess, cand, test);
    for (int i = 0; i < WORD_LEN; i++)
        if (test[i] != colors[i])
            return false;
    return true;
}

int filter_candidates(char **cand, int size, const char *guess, Color colors[])
{
    int newsize = 0;
    char **tmp = malloc(sizeof(char *) * size);
    for (int i = 0; i < size; i++)
    {
        if (compatible(cand[i], guess, colors))
            tmp[newsize++] = cand[i];
        else
            free(cand[i]);
    }
    for (int i = 0; i < newsize; i++)
        cand[i] = tmp[i];
    free(tmp);
    return newsize;
}

char **build_candidates_copy(int *out_size)
{
    char **cand = malloc(sizeof(char *) * dict_size);
    for (int i = 0; i < dict_size; i++)
        cand[i] = strdup(dictionary[i]);
    *out_size = dict_size;
    return cand;
}

static void letter_freq(char **cand, int size, int freq[26])
{
    for (int i = 0; i < 26; i++)
        freq[i] = 0;
    for (int i = 0; i < size; i++)
    {
        bool seen[26] = {0};
        for (int j = 0; j < WORD_LEN; j++)
        {
            int idx = cand[i][j] - 'a';
            if (!seen[idx])
            {
                freq[idx]++;
                seen[idx] = true;
            }
        }
    }
}

static int word_score(const char *w, int freq[26])
{
    bool seen[26] = {0};
    int s = 0;
    for (int i = 0; i < WORD_LEN; i++)
    {
        int idx = w[i] - 'a';
        if (!seen[idx])
        {
            s += freq[idx];
            seen[idx] = true;
        }
    }
    return s;
}

char *choose_guess(char **cand, int size)
{
    if (size == 0)
        return NULL;
    int freq[26];
    letter_freq(cand, size, freq);
    int best = -1;
    char *bestw = cand[0];
    for (int i = 0; i < size; i++)
    {
        int s = word_score(cand[i], freq);
        if (s > best)
        {
            best = s;
            bestw = cand[i];
        }
    }
    return bestw;
}

int solver_play_against(const char *target)
{
    int size;
    char **cand = build_candidates_copy(&size);
    Color colors[WORD_LEN];
    printf("Solveur: Mot secret = %s\n", target);
    for (int t = 1; t <= MAX_GUESSES; t++)
    {
        if (size <= 0)
            break;
        char *guess = choose_guess(cand, size);
        printf("Essai %d (Candidats restants: %d): %s\n", t, size, guess);
        compute_feedback(guess, target, colors);
        printf("Feedback: ");
        for (int i = 0; i < WORD_LEN; i++)
            printf("[%c %c] ", colors[i] == GREEN ? 'G' : colors[i] == YELLOW ? 'Y'
                                                                              : ' ',
                   toupper(guess[i]));
        printf("\n");
        if (strcmp(guess, target) == 0)
        {
            for (int i = 0; i < size; i++)
                free(cand[i]);
            free(cand);
            return t;
        }
        size = filter_candidates(cand, size, guess, colors);
    }
    for (int i = 0; i < size; i++)
        free(cand[i]);
    free(cand);
    return -1;
}

double benchmark_all(int limit)
{
    if (limit <= 0 || limit > dict_size)
        limit = dict_size;
    long total = 0;
    int ok = 0;
    for (int i = 0; i < limit; i++)
    {
        int r = solver_play_against(dictionary[i]);
        if (r > 0)
        {
            total += r;
            ok++;
        }
        if ((i + 1) % 50 == 0 || i + 1 == limit)
            printf("... %d/%d Moyenne=%.2f\n", i + 1, limit, (ok > 0) ? (double)total / ok : 0);
    }
    return (ok > 0) ? (double)total / ok : -1;
}

void print_feedback(const char *guess, Color colors[])
{
    for (int i = 0; i < WORD_LEN; i++)
        printf("[%c %c] ", colors[i] == GREEN ? 'G' : colors[i] == YELLOW ? 'Y'
                                                                          : ' ',
               toupper(guess[i]));
    printf("\n");
}

void human_play()
{
    srand(time(NULL));
    int idx = rand() % dict_size;
    char *target = dictionary[idx];
    char guess[64];

    printf("\nDevinez le mot secret  (%d lettres) parmis ces 8 mots (messi,kroos,paris,kamel,ramos,milan,usthb,roben)\n", WORD_LEN);
    for (int t = 1; t <= MAX_GUESSES; t++)
    {
        printf("Essai %d> ", t);
        if (scanf("%s", guess) != 1)
        {
            printf("Erreur\n");
            return;
        }
        if (strlen(guess) != WORD_LEN)
        {
            printf("Mot de %d lettres !\n", WORD_LEN);
            t--;
            continue;
        }
        for (int i = 0; i < WORD_LEN; i++)
            guess[i] = tolower(guess[i]);
        if (!is_valid_word(guess))
        {
            printf("Mot invalide\n");
            t--;
            continue;
        }
        Color c[WORD_LEN];
        compute_feedback(guess, target, c);
        print_feedback(guess, c);
        if (strcmp(guess, target) == 0)
        {
            printf("Bravo Tu as gagné en %d essais !\n", t);
            return;
        }
    }
    printf("Perdu , a la prochain  ! Mot = %s\n", target);
}

void solver_demo()
{
    char mot[32];
    printf("Mot secret (%d lettres) ou 'random': ", WORD_LEN);
    if (scanf("%s", mot) != 1)
        return;
    for (int i = 0; i < strlen(mot); i++)
        mot[i] = tolower(mot[i]);
    if (strcmp(mot, "random") == 0)
        strcpy(mot, dictionary[rand() % dict_size]);
    else if (strlen(mot) != WORD_LEN || !is_valid_word(mot))
    {
        printf("Mot invalide\n");
        return;
    }
    int r = solver_play_against(mot);
    if (r > 0)
        printf("\nSolveur a trouvé %s en %d essais\n ", mot, r);
    else
        printf("\nÉchec احمق\n");
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <words.txt>\n", argv[0]);
        return 1;
    }
    int loaded = load_dictionary(argv[1]);
    if (loaded <= 0)
    {
        printf("Dictionnaire vide\n");
        return 1;
    }
    printf("%d mots chargés (%d lettres)\n", loaded, WORD_LEN);
    while (1)
    {
        printf("\n-------bienvenue dans mon jeu wordle solver ");
        printf("\nchoisissez l'un des choix suivants:");
        printf("\n1.Jouer");
        printf("\n2.solveur");
        printf("\n3.benchmark");
        printf("\n4.quitter");
        printf("\nvotre choix est :");

        int ch;
        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }
        if (ch == 1)
            human_play();
        else if (ch == 2)
            solver_demo();
        else if (ch == 3)
        {
            double avg = benchmark_all(100);
            if (avg > 0)
                printf("\nMoyenne=%.2f essais\n", avg);
        }
        else if (ch == 4)
            break;
        else
            printf("Choix invalide\n");
    }
    free_dictionary();
    return 0;
}
