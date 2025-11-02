#include "wordle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char **build_candidates_copy(int *out_size) {
    char **cand = malloc(sizeof(char*) * dict_size);
    for (int i=0;i<dict_size;i++) cand[i] = strdup(dictionary[i]);
    *out_size = dict_size;
    return cand;
}

static void letter_freq(char **cand, int size, int freq[26]) {
    for (int i=0;i<26;i++) freq[i]=0;
    for (int i=0;i<size;i++) {
        bool seen[26]={0};
        for (int j=0;j<WORD_LEN;j++) {
            int idx = cand[i][j]-'a';
            if (!seen[idx]) {
                freq[idx]++;
                seen[idx]=true;
            }
        }
    }
}

static int word_score(const char *w, int freq[26]) {
    bool seen[26]={0};
    int s=0;
    for (int i=0;i<WORD_LEN;i++) {
        int idx=w[i]-'a';
        if(!seen[idx]){ s+=freq[idx]; seen[idx]=true; }
    }
    return s;
}

char *choose_guess(char **cand, int size) {
    int freq[26];
    letter_freq(cand, size, freq);
    int best=-1;
    char *bestw=cand[0];
    for(int i=0;i<size;i++){
        int s=word_score(cand[i], freq);
        if(s>best){ best=s; bestw=cand[i]; }
    }
    return bestw;
}

int solver_play_against(const char *target) {
    int size;
    char **cand = build_candidates_copy(&size);
    Color colors[WORD_LEN];
    for(int t=1;t<=MAX_GUESSES;t++){
        char *guess = choose_guess(cand, size);
        compute_feedback(guess, target, colors);
        if(strcmp(guess,target)==0){
            for(int i=0;i<size;i++) free(cand[i]);
            free(cand);
            return t;
        }
        size = filter_candidates(cand, size, guess, colors);
        if(size<=0) break;
    }
    for(int i=0;i<size;i++) free(cand[i]);
    free(cand);
    return -1;
}

double benchmark_all(int limit) {
    if(limit<=0 || limit>dict_size) limit=dict_size;
    long total=0; int ok=0;
    for(int i=0;i<limit;i++){
        int r=solver_play_against(dictionary[i]);
        if(r>0){ total+=r; ok++; }
    }
    return (ok>0)? (double)total/ok : -1.0;
}
