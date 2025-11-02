#include "wordle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int solver_play_against(const char *target);
double benchmark_all(int limit);

void print_feedback(const char *guess, Color colors[]) {
    for(int i=0;i<WORD_LEN;i++){
        if(colors[i]==GREEN) printf("[G %c] ", toupper(guess[i]));
        else if(colors[i]==YELLOW) printf("[Y %c] ", toupper(guess[i]));
        else printf("[  %c] ", toupper(guess[i]));
    }
    printf("\n");
}

void human_play(){
    srand(time(NULL));
    int idx = rand()%dict_size;
    char *target=dictionary[idx];
    char guess[64];
    printf("Devinez le mot secret de 5 lettres !\n");
    for(int t=1;t<=MAX_GUESSES;t++){
        printf("Essai %d> ",t);
        scanf("%s",guess);
        for(int i=0;i<WORD_LEN;i++) guess[i]=tolower(guess[i]);
        if(!is_valid_word(guess)){ printf("Mot non valide !\n"); t--; continue; }
        Color c[WORD_LEN];
        compute_feedback(guess,target,c);
        print_feedback(guess,c);
        if(strcmp(guess,target)==0){
            printf("Bravo ! Trouvé en %d essais !\n",t);
            return;
        }
    }
    printf("Perdu ! Le mot était %s\n",target);
}

void solver_demo(){
    char mot[32];
    printf("Entrez un mot secret ou 'random': ");
    scanf("%s",mot);
    if(strcmp(mot,"random")==0){
        srand(time(NULL));
        strcpy(mot,dictionary[rand()%dict_size]);
    }
    int r=solver_play_against(mot);
    if(r>0) printf("Solveur a trouvé %s en %d essais.\n", mot,r);
    else printf("Échec du solveur.\n");
}

int main(int argc, char **argv){
    if(argc<2){ printf("Usage: %s words.txt\n",argv[0]); return 1; }
    if(load_dictionary(argv[1])<=0){ printf("Erreur: dictionnaire invalide.\n"); return 1; }
    while(1){
        printf("\n--- Wordle Solver ---\n");
        printf("1. Jouer (humain)\n");
        printf("2. Solveur automatique\n");
        printf("3. Benchmark solveur\n");
        printf("4. Quitter\n> ");
        int ch; scanf("%d",&ch);
        if(ch==1) human_play();
        else if(ch==2) solver_demo();
        else if(ch==3){
            printf("Moyenne solveur = %.2f essais\n", benchmark_all(100));
        } else break;
    }
    free_dictionary();
    return 0;
}
