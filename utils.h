#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_WORD_LEN 64
#define MAX_TITLE 256
#define MAX_COMPOSER 256
#define MAX_ESTROFE 256

// estrutura temporária para contar palavras dentro de uma música
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
    char estrofe[MAX_ESTROFE]; // primeira estrofe que contém a palavra
    int has_estrofe;
} SongWord;

// processa um arquivo de música e preenche arrays de SongWord
// retorna número de palavras distintas encontradas no vetor songWords
int processar_arquivo_musica(const char *filepath, SongWord **outWords, int *outCount, char title[MAX_TITLE], char composer[MAX_COMPOSER]);

// normaliza palavra (minusculas, remove pontuação). Retorna 1 se palavra válida (>3 chars), 0 caso contrário.
int normalizar_palavra(const char *in, char *out);

// retorna o tempo corrente em segundos (double) para medir tempo
double agora_segundos();

#endif
