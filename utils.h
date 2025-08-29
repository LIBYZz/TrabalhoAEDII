#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>


// estrutura temporária para contar palavras dentro de uma música
typedef struct {
    char palavra[64];
    int count;
    char estrofe[256]; // primeira estrofe que contém a palavra
    int has_estrofe;
} Palavra_Musica;

// processa um arquivo de música e preenche arrays de Palavra_Musica
// retorna número de palavras distintas encontradas no vetor songWords
int processar_arquivo_musica(const char *filepath, Palavra_Musica **palavra_saida, int *cont_saida, char titulo[256], char autor[256]);

// normaliza palavra (minusculas, remove pontuação). Retorna 1 se palavra válida (>3 chars), 0 caso contrário.
int normalizar_palavra(const char *stringEntrada, char *stringSaida);

// retorna o tempo corrente em segundos (double) para medir tempo
double agora_segundos();

#endif
