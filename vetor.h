#ifndef VETOR_H
#define VETOR_H

#include "utils.h"

typedef struct {
    char word[64];
    char best_title[256];
    char best_composer[256];
    char estrofe[256];
    int best_freq_in_song;
    int total_freq;
} Palavra_Dados;

typedef struct {
    Palavra_Dados *data;
    int size;
    int capacity;
} WordVector;

void inicia_vetor(WordVector *v);
void libera_vetor(WordVector *v);

// insere ou atualiza uma palavra com dados da música atual
void vetor_upsert(WordVector *v, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra (binary search). retorna index ou um número negativo se não encontrou
int busca_binaria(WordVector *v, const char *word);

// imprime informação da palavra (se encontrada)
void print_vetor(WordVector *v, const char *word);

#endif
