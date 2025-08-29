#include "bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BSTNode* novo_no(const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    BSTNode *n = (BSTNode*)malloc(sizeof(BSTNode));
    if (!n) return NULL;
    strncpy(n->data.word, word, 63); 
    n->data.word[63] = '\0';
    strncpy(n->data.best_title, title, 255); 
    n->data.best_title[255] = '\0';
    strncpy(n->data.best_composer, composer, 255); 
    n->data.best_composer[255] = '\0';
    if (estrofe) strncpy(n->data.estrofe, estrofe, 255);
    n->data.estrofe[255] = '\0';
    n->data.best_freq_in_song = count;
    n->data.total_freq = count;
    n->left = n->right = NULL;
    return n;
}

void iniciar_bst(BSTree *t) {
    t->root = NULL;
    t->size = 0;
}

void liberar_no_recursivo(BSTNode *n) {
    if (!n) return;
    liberar_no_recursivo(n->left);
    liberar_no_recursivo(n->right);
    free(n);
}

void liberar_bst(BSTree *t) {
    if (!t) return;
    liberar_no_recursivo(t->root);
    t->root = NULL;
    t->size = 0;
}

// atualiza recursivamente
BSTNode* atualiza_rec_bst(BSTNode *no, const char *word, const char *title, const char *composer, const char *estrofe, int count, int *inserido) {
    if (no == NULL) {
        *inserido = 1;
        return novo_no(word, title, composer, estrofe, count);
    }
    int cmp = strcmp(word, no->data.word);
    if (cmp == 0) {
        no->data.total_freq += count;
        if (count > no->data.best_freq_in_song) {
            strncpy(no->data.best_title, title, 255); 
            no->data.best_title[255] = '\0';
            strncpy(no->data.best_composer, composer, 255); 
            no->data.best_composer[255] = '\0';
            if (estrofe) {
                strncpy(no->data.estrofe, estrofe, 255); 
                no->data.estrofe[255] = '\0';
            }
            no->data.best_freq_in_song = count;
        }
        return no;
    } else if (cmp < 0) {
        no->left = atualiza_rec_bst(no->left, word, title, composer, estrofe, count, inserido);
    } else {
        no->right = atualiza_rec_bst(no->right, word, title, composer, estrofe, count, inserido);
    }
    return no;
}

void atualiza_bst(BSTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    if (!t || !word) return;
    int inserido = 0;
    t->root = atualiza_rec_bst(t->root, word, title, composer, estrofe, count, &inserido);
    if (inserido) t->size++;
}


BSTNode* busca_rec_bst(BSTNode *node, const char *word) {
    if (!node) return NULL;
    int cmp = strcmp(word, node->data.word);
    if (cmp == 0) return node;
    if (cmp < 0) return busca_rec_bst(node->left, word);
    return busca_rec_bst(node->right, word);
}

Palavra_Dados *busca_bst(BSTree *t, const char *word) {
    if (!t) return NULL;
    BSTNode *n = busca_rec_bst(t->root, word);
    if (!n) return NULL;
    return &n->data;
}

void print_bst(BSTree *t, const char *word) {
    Palavra_Dados *e = busca_bst(t, word);
    if (!e) {
        printf("Palavra \"%s\" não encontrada no repositório (BST).\n", word);
        return;
    }
    printf("Palavra: %s\n", e->word);
    printf("Música (maior frequência): %s\n", e->best_title);
    printf("Compositor(a): %s\n", e->best_composer);
    printf("Frequência nessa música: %d\n", e->best_freq_in_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
