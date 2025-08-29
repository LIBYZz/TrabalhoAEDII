#ifndef AVL_H
#define AVL_H

#include "vetor.h"

typedef struct AVLNode {
    Palavra_Dados data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct {
    AVLNode *root;
    int size;
} AVLTree;

void inicia_avl(AVLTree *t);
void libera_avl(AVLTree *t);

// insere ou atualiza (mesma política do vetor/bst)
void atualiza_avl(AVLTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra
Palavra_Dados *busca_avl(AVLTree *t, const char *word);

// imprime info
void print_avl(AVLTree *t, const char *word);

#endif
