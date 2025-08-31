#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>


typedef struct {
    char palavra[64];
    int count;
    char estrofe[256];
    int has_estrofe;
} Palavra_Musica;

int processar_arquivo_musica(const char *filepath, Palavra_Musica **palavra_saida, int *cont_saida, char titulo[256], char autor[256]);

int normalizar_palavra(const char *stringEntrada, char *stringSaida);

double agora_segundos();

#endif
