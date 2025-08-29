#ifndef VETOR_H
#define VETOR_H

#include "utils.h"

typedef struct {
    char palavra[64];
    char max_titulo[256];
    char max_autor[256];
    char estrofe[256];
    int max_freq_song;
    int total_freq;
} Palavra_Dados;

typedef struct {
    Palavra_Dados *data;
    int tamanho;
    int capacity;
} Palavra_Vetor;

void inicia_vetor(Palavra_Vetor *v);
void libera_vetor(Palavra_Vetor *v);

// insere ou atualiza uma palavra com dados da música atual
void atualiza_vetor(Palavra_Vetor *v, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count);

// busca por palavra (binary search). retorna index ou um número negativo se não encontrou
int busca_binaria(Palavra_Vetor *v, const char *palavra);

// imprime informação da palavra (se encontrada)
void print_vetor(Palavra_Vetor *v, const char *palavra);

#endif
