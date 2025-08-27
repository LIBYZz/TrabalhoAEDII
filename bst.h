#ifndef BST_H
#define BST_H

#include "vetor.h" // reutiliza WordEntry definition

typedef struct BSTNode {
    WordEntry data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
    int size;
} BSTree;

void bst_init(BSTree *t);
void bst_free(BSTree *t);

// insere ou atualiza (mesma política do vetor):
// - se palavra já existe: total_freq += count_in_song; se count_in_song > best_freq_in_song atualiza dados da música
// - se não existe: cria nó com os dados fornecidos
void bst_upsert(BSTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song);

// busca por palavra (retorna ponteiro para WordEntry ou NULL)
WordEntry *bst_search(BSTree *t, const char *word);

// imprime info se encontrada
void bst_print_info(BSTree *t, const char *word);

#endif
