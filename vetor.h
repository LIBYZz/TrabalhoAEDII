#ifndef VETOR_H
#define VETOR_H

#include "utils.h"

typedef struct {
    char word[MAX_WORD_LEN];
    char best_title[MAX_TITLE];
    char best_composer[MAX_COMPOSER];
    char estrofe[MAX_ESTROFE]; // up to 100 chars stored
    int best_freq_in_song;
    int total_freq; // across all songs in repository
} WordEntry;

typedef struct {
    WordEntry *data;
    int size;
    int capacity;
} WordVector;

void vetor_init(WordVector *v);
void vetor_free(WordVector *v);

// insere ou atualiza uma palavra com dados da música atual
void vetor_upsert(WordVector *v, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra (binary search). retorna index ou -1 se não encontrou
int vetor_search(WordVector *v, const char *word);

// imprime informação da palavra (se encontrada)
void vetor_print_info(WordVector *v, const char *word);

#endif
