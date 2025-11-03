#ifndef WORDLE_H
#define WORDLE_H

#include <stdbool.h>

#define MAX_WORDS 20000
#define WORD_LEN 5
#define MAX_GUESSES 6

typedef enum { GRAY=0, YELLOW=1, GREEN=2 } Color;

extern char **dictionary;
extern int dict_size;

int load_dictionary(const char *filename);
bool is_valid_word(const char *w);
void compute_feedback(const char *guess, const char *target, Color colors[]);
int filter_candidates(char **candidates, int cand_size, const char *guess, Color colors[]);
void free_dictionary();

#endif
