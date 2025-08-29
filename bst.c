#include "bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BSTNo* novo_no_bst(const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count) {
    BSTNo *n = (BSTNo*)malloc(sizeof(BSTNo));
    if (!n) return NULL;
    strncpy(n->data.palavra, palavra, 63); 
    n->data.palavra[63] = '\0';
    strncpy(n->data.max_titulo, titulo, 255); 
    n->data.max_titulo[255] = '\0';
    strncpy(n->data.max_autor, autor, 255); 
    n->data.max_autor[255] = '\0';
    if (estrofe) strncpy(n->data.estrofe, estrofe, 255);
    n->data.estrofe[255] = '\0';
    n->data.max_freq_song = count;
    n->data.total_freq = count;
    n->left = n->right = NULL;
    return n;
}

void iniciar_bst(BSTArv *t) {
    t->root = NULL;
    t->tamanho = 0;
}

void liberar_no_recursivo(BSTNo *n) {
    if (!n) return;
    liberar_no_recursivo(n->left);
    liberar_no_recursivo(n->right);
    free(n);
}

void liberar_bst(BSTArv *t) {
    if (!t) return;
    liberar_no_recursivo(t->root);
    t->root = NULL;
    t->tamanho = 0;
}

// atualiza recursivamente
BSTNo* atualiza_rec_bst(BSTNo *no, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count, int *inserido) {
    if (no == NULL) {
        *inserido = 1;
        return novo_no_bst(palavra, titulo, autor, estrofe, count);
    }
    int cmp = strcmp(palavra, no->data.palavra);
    if (cmp == 0) {
        no->data.total_freq += count;
        if (count > no->data.max_freq_song) {
            strncpy(no->data.max_titulo, titulo, 255); 
            no->data.max_titulo[255] = '\0';
            strncpy(no->data.max_autor, autor, 255); 
            no->data.max_autor[255] = '\0';
            if (estrofe) {
                strncpy(no->data.estrofe, estrofe, 255); 
                no->data.estrofe[255] = '\0';
            }
            no->data.max_freq_song = count;
        }
        return no;
    } else if (cmp < 0) {
        no->left = atualiza_rec_bst(no->left, palavra, titulo, autor, estrofe, count, inserido);
    } else {
        no->right = atualiza_rec_bst(no->right, palavra, titulo, autor, estrofe, count, inserido);
    }
    return no;
}

void atualiza_bst(BSTArv *t, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count) {
    if (!t || !palavra) return;
    int inserido = 0;
    t->root = atualiza_rec_bst(t->root, palavra, titulo, autor, estrofe, count, &inserido);
    if (inserido) t->tamanho++;
}


BSTNo* busca_rec_bst(BSTNo *no, const char *palavra) {
    if (!no) return NULL;
    int cmp = strcmp(palavra, no->data.palavra);
    if (cmp == 0) return no;
    if (cmp < 0) return busca_rec_bst(no->left, palavra);
    return busca_rec_bst(no->right, palavra);
}

Palavra_Dados *busca_bst(BSTArv *t, const char *palavra) {
    if (!t) return NULL;
    BSTNo *n = busca_rec_bst(t->root, palavra);
    if (!n) return NULL;
    return &n->data;
}

void print_bst(BSTArv *t, const char *palavra) {
    Palavra_Dados *e = busca_bst(t, palavra);
    if (!e) {
        printf("Palavra \"%s\" não encontrada no repositório (BST).\n", palavra);
        return;
    }
    printf("Palavra: %s\n", e->palavra);
    printf("Música (maior frequência): %s\n", e->max_titulo);
    printf("Compositor(a): %s\n", e->max_autor);
    printf("Frequência nessa música: %d\n", e->max_freq_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
