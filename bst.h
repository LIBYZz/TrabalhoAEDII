#ifndef BST_H
#define BST_H

#include "vetor.h" // reutiliza Palavra_Dados definition

typedef struct BSTNode {
    Palavra_Dados data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
    int size;
} BSTree;

void iniciar_bst(BSTree *t);
void liberar_bst(BSTree *t);

// insere ou atualiza (mesma política do vetor):
// - se palavra já existe: total_freq += count_in_song; se count_in_song > best_freq_in_song atualiza dados da música
// - se não existe: cria nó com os dados fornecidos
void atualiza_bst(BSTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra (retorna ponteiro para Palavra_Dados ou NULL)
Palavra_Dados *buscar_bst(BSTree *t, const char *word);

// imprime info se encontrada
void print_bst(BSTree *t, const char *word);

#endif
