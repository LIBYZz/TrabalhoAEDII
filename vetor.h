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

void atualiza_vetor(Palavra_Vetor *v, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count);

int busca_binaria(Palavra_Vetor *v, const char *palavra);

void print_vetor(Palavra_Vetor *v, const char *palavra);

#endif
