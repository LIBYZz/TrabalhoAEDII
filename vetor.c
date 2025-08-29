#include "vetor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void inicia_vetor(WordVector *v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void libera_vetor(WordVector *v) {
    if (v->data) free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void garantir_capacidade(WordVector *v, int need) {
    if (v->capacity >= need) return;
    int new_cap = v->capacity == 0 ? 16 : v->capacity * 2;
    while (new_cap < need) new_cap *= 2;
    v->data = (Palavra_Dados*)realloc(v->data, sizeof(Palavra_Dados)*new_cap);
    v->capacity = new_cap;
}

// busca binária por posição de inserção (retorna index >=0 se encontrado, ou -posicao_minima-1)
int busca_binaria(WordVector *v, const char *word) {
    int min = 0, max = v->size - 1;
    while (min <= max) {
        int med = (min + max) / 2;
        int cmp = strcmp(v->data[med].word, word);
        if (cmp == 0) return med;
        if (cmp < 0) min = med + 1;
        else max = med - 1;
    }
    return -min - 1;
}

void atualiza_palavra(WordVector *v, const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    int pos = busca_binaria(v, word);
    if (pos >= 0) {
        // existente: atualiza total e potencialmente dados da música com maior frequência
        v->data[pos].total_freq += count;
        if (count > v->data[pos].best_freq_in_song) {
            // atualiza infos
            strncpy(v->data[pos].best_title, title, 255); v->data[pos].best_title[255]='\0';
            strncpy(v->data[pos].best_composer, composer, 255); v->data[pos].best_composer[255]='\0';
            if (estrofe) { strncpy(v->data[pos].estrofe, estrofe, 255); v->data[pos].estrofe[255]='\0'; }
            v->data[pos].best_freq_in_song = count;
        }
    } else {
        int insere = -pos - 1;
        garantir_capacidade(v, v->size + 1);
        // move os elementos da frente para a posição correta
        if (insere < v->size) {
            memmove(&v->data[insere+1], &v->data[insere], sizeof(Palavra_Dados)*(v->size - insere));
        }
        // insere a nova entrada
        Palavra_Dados *e = &v->data[insere];
        strncpy(e->word, word, 63); e->word[63]='\0';
        strncpy(e->best_title, title, 255); e->best_title[255]='\0';
        strncpy(e->best_composer, composer, 255); e->best_composer[255]='\0';
        if (estrofe) strncpy(e->estrofe, estrofe, 255); e->estrofe[255]='\0';
        e->best_freq_in_song = count;
        e->total_freq = count;
        v->size++;
    }
}

void print_vetor(WordVector *v, const char *word) {
    int idx = busca_binaria(v, word);
    if (idx < 0) {
        printf("Palavra \"%s\" não encontrada no repositório (vetor).\n", word);
        return;
    }
    Palavra_Dados *e = &v->data[idx];
    printf("Palavra: %s\n", e->word);
    printf("Música (maior frequência): %s\n", e->best_title);
    printf("Compositor(a): %s\n", e->best_composer);
    printf("Frequência nessa música: %d\n", e->best_freq_in_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
