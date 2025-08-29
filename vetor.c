#include "vetor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void inicia_vetor(Palavra_Vetor *v) {
    v->data = NULL;
    v->tamanho = 0;
    v->capacity = 0;
}

void libera_vetor(Palavra_Vetor *v) {
    if (v->data) free(v->data);
    v->data = NULL;
    v->tamanho = 0;
    v->capacity = 0;
}

void garantir_capacidade(Palavra_Vetor *v, int need) {
    if (v->capacity >= need) return;
    int new_cap = v->capacity == 0 ? 16 : v->capacity * 2;
    while (new_cap < need) new_cap *= 2;
    v->data = (Palavra_Dados*)realloc(v->data, sizeof(Palavra_Dados)*new_cap);
    v->capacity = new_cap;
}

// busca binária por posição de inserção (retorna index >=0 se encontrado, ou -posicao_minima-1)
int busca_binaria(Palavra_Vetor *v, const char *palavra) {
    int min = 0, max = v->tamanho - 1;
    while (min <= max) {
        int med = (min + max) / 2;
        int cmp = strcmp(v->data[med].palavra, palavra);
        if (cmp == 0) return med;
        if (cmp < 0) min = med + 1;
        else max = med - 1;
    }
    return -min - 1;
}

void atualiza_vetor(Palavra_Vetor *v, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count) {
    int pos = busca_binaria(v, palavra);
    if (pos >= 0) {
        // existente: atualiza total e potencialmente dados da música com maior frequência
        v->data[pos].total_freq += count;
        if (count > v->data[pos].max_freq_song) {
            // atualiza infos
            strncpy(v->data[pos].max_titulo, titulo, 255); v->data[pos].max_titulo[255]='\0';
            strncpy(v->data[pos].max_autor, autor, 255); v->data[pos].max_autor[255]='\0';
            if (estrofe) { strncpy(v->data[pos].estrofe, estrofe, 255); v->data[pos].estrofe[255]='\0'; }
            v->data[pos].max_freq_song = count;
        }
    } else {
        int insere = -pos - 1;
        garantir_capacidade(v, v->tamanho + 1);
        // move os elementos da frente para a posição correta
        if (insere < v->tamanho) {
            memmove(&v->data[insere+1], &v->data[insere], sizeof(Palavra_Dados)*(v->tamanho - insere));
        }
        // insere a nova entrada
        Palavra_Dados *e = &v->data[insere];
        strncpy(e->palavra, palavra, 63); e->palavra[63]='\0';
        strncpy(e->max_titulo, titulo, 255); e->max_titulo[255]='\0';
        strncpy(e->max_autor, autor, 255); e->max_autor[255]='\0';
        if (estrofe) strncpy(e->estrofe, estrofe, 255); e->estrofe[255]='\0';
        e->max_freq_song = count;
        e->total_freq = count;
        v->tamanho++;
    }
}

void print_vetor(Palavra_Vetor *v, const char *palavra) {
    int idx = busca_binaria(v, palavra);
    if (idx < 0) {
        printf("Palavra \"%s\" não encontrada no repositório (vetor).\n", palavra);
        return;
    }
    Palavra_Dados *e = &v->data[idx];
    printf("Palavra: %s\n", e->palavra);
    printf("Música (maior frequência): %s\n", e->max_titulo);
    printf("Compositor(a): %s\n", e->max_autor);
    printf("Frequência nessa música: %d\n", e->max_freq_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
