#ifndef BST_H
#define BST_H

#include "vetor.h" // reutiliza Palavra_Dados definition

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

// insere ou atualiza (mesma política do vetor):
// - se palavra já existe: total_freq += count; se count > max_freq_song atualiza dados da música
// - se não existe: cria nó com os dados fornecidos
void atualiza_bst(BSTArv *t, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count);

// busca por palavra (retorna ponteiro para Palavra_Dados ou NULL)
Palavra_Dados *busca_bst(BSTArv *t, const char *palavra);

// imprime info se encontrada
void print_bst(BSTArv *t, const char *palavra);

#endif
