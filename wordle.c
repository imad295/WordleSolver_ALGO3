#include "wordle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **dictionary = NULL;
int dict_size = 0;

int load_dictionary(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("open dictionary");
        return -1;
    }
    dictionary = malloc(sizeof(char*) * MAX_WORDS);
    char buf[64];
    dict_size = 0;
    while (fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\n")] = 0;
        if (strlen(buf) == WORD_LEN) {
            dictionary[dict_size] = strdup(buf);
            dict_size++;
        }
    }
    fclose(f);
    return dict_size;
}

bool is_valid_word(const char *w) {
    for (int i=0;i<dict_size;i++)
        if (strcmp(dictionary[i], w)==0)
            return true;
    return false;
}

void compute_feedback(const char *guess, const char *target, Color colors[]) {
    int i, count[26]={0};
    for (i=0;i<WORD_LEN;i++) colors[i]=GRAY;
    for (i=0;i<WORD_LEN;i++) {
        if (guess[i]==target[i]) colors[i]=GREEN;
        else count[target[i]-'a']++;
    }
    for (i=0;i<WORD_LEN;i++) {
        if (colors[i]==GREEN) continue;
        if (count[guess[i]-'a']>0) {
            colors[i]=YELLOW;
            count[guess[i]-'a']--;
        }
    }
}

static bool compatible(const char *cand, const char *guess, Color colors[]) {
    Color test[WORD_LEN];
    compute_feedback(guess, cand, test);
    for (int i=0;i<WORD_LEN;i++)
        if (test[i]!=colors[i]) return false;
    return true;
}

int filter_candidates(char **cand, int size, const char *guess, Color colors[]) {
    int newsize=0;
    for (int i=0;i<size;i++)
        if (compatible(cand[i], guess, colors))
            cand[newsize++]=cand[i];
    return newsize;
}

void free_dictionary() {
    for (int i=0;i<dict_size;i++) free(dictionary[i]);
    free(dictionary);
}
