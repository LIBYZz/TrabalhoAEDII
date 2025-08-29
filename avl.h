#ifndef AVL_H
#define AVL_H

#include "vetor.h"

typedef struct AVLNo {
    Palavra_Dados data;
    struct AVLNo *left;
    struct AVLNo *right;
    int height;
} AVLNo;

typedef struct {
    AVLNo *root;
    int tamanho;
} AVLArv;

void inicia_avl(AVLArv *t);
void libera_avl(AVLArv *t);

// insere ou atualiza (mesma política do vetor/bst)
void atualiza_avl(AVLArv *t, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count);

// busca por palavra
Palavra_Dados *busca_avl(AVLArv *t, const char *palavra);

// imprime info
void print_avl(AVLArv *t, const char *palavra);

#endif
