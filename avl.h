#ifndef AVL_H
#define AVL_H

#include "vetor.h"

typedef struct AVLNode {
    WordEntry data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

typedef struct {
    AVLNode *root;
    int size;
} AVLTree;

void avl_init(AVLTree *t);
void avl_free(AVLTree *t);

// insere ou atualiza (mesma política do vetor/bst)
void avl_upsert(AVLTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra
WordEntry *avl_search(AVLTree *t, const char *word);

// imprime info
void avl_print_info(AVLTree *t, const char *word);

#endif
