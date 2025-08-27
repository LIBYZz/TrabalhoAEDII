#include "vetor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void vetor_init(WordVector *v) {
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

void vetor_free(WordVector *v) {
    if (v->data) free(v->data);
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}

static void ensure_capacity(WordVector *v, int need) {
    if (v->capacity >= need) return;
    int newcap = v->capacity == 0 ? 16 : v->capacity * 2;
    while (newcap < need) newcap *= 2;
    v->data = (WordEntry*)realloc(v->data, sizeof(WordEntry)*newcap);
    v->capacity = newcap;
}

// busca binária por posição de inserção (retorna index >=0 se encontrado, ou -insertion_point-1)
static int binary_search(WordVector *v, const char *word) {
    int lo = 0, hi = v->size - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        int cmp = strcmp(v->data[mid].word, word);
        if (cmp == 0) return mid;
        if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -lo - 1;
}

void vetor_upsert(WordVector *v, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song) {
    int pos = binary_search(v, word);
    if (pos >= 0) {
        // existente: atualiza total e potencialmente dados da música com maior frequência
        v->data[pos].total_freq += count_in_song;
        if (count_in_song > v->data[pos].best_freq_in_song) {
            // atualiza infos
            strncpy(v->data[pos].best_title, title, MAX_TITLE-1); v->data[pos].best_title[MAX_TITLE-1]='\0';
            strncpy(v->data[pos].best_composer, composer, MAX_COMPOSER-1); v->data[pos].best_composer[MAX_COMPOSER-1]='\0';
            if (estrofe) { strncpy(v->data[pos].estrofe, estrofe, MAX_ESTROFE-1); v->data[pos].estrofe[MAX_ESTROFE-1]='\0'; }
            v->data[pos].best_freq_in_song = count_in_song;
        }
    } else {
        int insert_at = -pos - 1;
        ensure_capacity(v, v->size + 1);
        // move items
        if (insert_at < v->size) {
            memmove(&v->data[insert_at+1], &v->data[insert_at], sizeof(WordEntry)*(v->size - insert_at));
        }
        // fill new entry
        WordEntry *e = &v->data[insert_at];
        strncpy(e->word, word, MAX_WORD_LEN-1); e->word[MAX_WORD_LEN-1]='\0';
        strncpy(e->best_title, title, MAX_TITLE-1); e->best_title[MAX_TITLE-1]='\0';
        strncpy(e->best_composer, composer, MAX_COMPOSER-1); e->best_composer[MAX_COMPOSER-1]='\0';
        if (estrofe) strncpy(e->estrofe, estrofe, MAX_ESTROFE-1); e->estrofe[MAX_ESTROFE-1]='\0';
        e->best_freq_in_song = count_in_song;
        e->total_freq = count_in_song;
        v->size++;
    }
}

int vetor_search(WordVector *v, const char *word) {
    int pos = binary_search(v, word);
    if (pos >= 0) return pos;
    return -1;
}

void vetor_print_info(WordVector *v, const char *word) {
    int idx = vetor_search(v, word);
    if (idx < 0) {
        printf("Palavra \"%s\" não encontrada no repositório (vetor).\n", word);
        return;
    }
    WordEntry *e = &v->data[idx];
    printf("Palavra: %s\n", e->word);
    printf("Música (maior frequência): %s\n", e->best_title);
    printf("Compositor(a): %s\n", e->best_composer);
    printf("Frequência nessa música: %d\n", e->best_freq_in_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
