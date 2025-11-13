#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define MAX_WORDS 20000
#define WORD_LEN 5
#define MAX_GUESSES 6

typedef enum
{
    GRAY,
    YELLOW,
    GREEN
} Color;

extern char **dictionary;
extern int dict_size;

int load_dictionary(const char *filename);
bool is_valid_word(const char *w);
void free_dictionary();
void compute_feedback(const char *guess, const char *target, Color colors[]);
int filter_candidates(char **cand, int size, const char *guess, Color colors[]);
char *choose_guess(char **cand, int size);
int solver_play_against(const char *target);
double benchmark_all(int limit);

#endif
