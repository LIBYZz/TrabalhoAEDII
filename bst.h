#ifndef BST_H
#define BST_H

#include "vetor.h"

typedef struct BSTNo {
    Palavra_Dados data;
    struct BSTNo *left;
    struct BSTNo *right;
} BSTNo;

typedef struct {
    BSTNo *root;
    int tamanho;
} BSTArv;

void iniciar_bst(BSTArv *t);
void liberar_bst(BSTArv *t);

void atualiza_bst(BSTArv *t, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count);

Palavra_Dados *busca_bst(BSTArv *t, const char *palavra);

void print_bst(BSTArv *t, const char *palavra);

#endif
