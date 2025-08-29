/* avl.c - implementação corrigida da AVL */
#include "avl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int calc_altura(AVLNode *n) {
    return n ? n->height : 0;
}

int cmp_max_int(int a, int b) {
    return a > b ? a : b;
}

AVLNode* novo_no(const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    AVLNode *n = (AVLNode*)malloc(sizeof(AVLNode));
    if (!n) return NULL;
    // inicia tudo
    n->left = n->right = NULL;
    n->height = 1;
    n->data.word[0] = '\0';
    n->data.best_title[0] = '\0';
    n->data.best_composer[0] = '\0';
    n->data.estrofe[0] = '\0';
    n->data.best_freq_in_song = 0;
    n->data.total_freq = 0;

    if (word) {
        strncpy(n->data.word, word, 63); n->data.word[63] = '\0';
    }
    if (title) {
        strncpy(n->data.best_title, title, 255); n->data.best_title[255] = '\0';
    }
    if (composer) {
        strncpy(n->data.best_composer, composer, 255); n->data.best_composer[255] = '\0';
    }
    if (estrofe) {
        strncpy(n->data.estrofe, estrofe, 255);
        n->data.estrofe[255] = '\0';
    }
    n->data.best_freq_in_song = count;
    n->data.total_freq = count;
    return n;
}

// rotações
AVLNode* roda_direita(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    /* rotação */
    x->right = y;
    y->left = T2;

    /* atualiza alturas */
    y->height = cmp_max_int(calc_altura(y->left), calc_altura(y->right)) + 1;
    x->height = cmp_max_int(calc_altura(x->left), calc_altura(x->right)) + 1;

    return x;
}

AVLNode* roda_esquerda(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    /* rotação */
    y->left = x;
    x->right = T2;

    /* atualiza alturas */
    x->height = cmp_max_int(calc_altura(x->left), calc_altura(x->right)) + 1;
    y->height = cmp_max_int(calc_altura(y->left), calc_altura(y->right)) + 1;

    return y;
}
int balanceamento(AVLNode *n) {
    return n ? calc_altura(n->left) - calc_altura(n->right) : 0;
}

AVLNode* insert_rec(AVLNode *node, const char *word, const char *title, const char *composer, const char *estrofe, int count, int *inserido) {
    if (!node) {
        *inserido = 1;
        return novo_no(word, title, composer, estrofe, count);
    }

    int cmp = strcmp(word, node->data.word);
    if (cmp == 0) {
        /* atualiza existente */
        node->data.total_freq += count;
        if (count > node->data.best_freq_in_song) {
            if (title) {
                strncpy(node->data.best_title, title, 255); node->data.best_title[255] = '\0';
            }
            if (composer) {
                strncpy(node->data.best_composer, composer, 255); node->data.best_composer[255] = '\0';
            }
            if (estrofe) {
                strncpy(node->data.estrofe, estrofe, 255); node->data.estrofe[255] = '\0';
            }
            node->data.best_freq_in_song = count;
        }
        return node;
    } else if (cmp < 0) {
        node->left = insert_rec(node->left, word, title, composer, estrofe, count, inserido);
    } else {
        node->right = insert_rec(node->right, word, title, composer, estrofe, count, inserido);
    }

    /* atualiza altura e fator de balanceamento */
    node->height = 1 + cmp_max_int(calc_altura(node->left), calc_altura(node->right));
    int bf = balanceamento(node);

    /* casos de rotação (com checagem de ponteiros por segurança) */
    /* LL */
    if (bf > 1 && node->left != NULL && strcmp(word, node->left->data.word) < 0)
        return roda_direita(node);
    /* RR */
    if (bf < -1 && node->right != NULL && strcmp(word, node->right->data.word) > 0)
        return roda_esquerda(node);
    /* LR */
    if (bf > 1 && node->left != NULL && strcmp(word, node->left->data.word) > 0) {
        node->left = roda_esquerda(node->left);
        return roda_direita(node);
    }
    /* RL */
    if (bf < -1 && node->right != NULL && strcmp(word, node->right->data.word) < 0) {
        node->right = roda_direita(node->right);
        return roda_esquerda(node);
    }

    return node;
}

void inicia_avl(AVLTree *t) {
    if (t) {
        t->root = NULL;
        t->size = 0;
    }
}

void libera_rec(AVLNode *n) {
    if (!n) return;
    libera_rec(n->left);
    libera_rec(n->right);
    free(n);
}
void libera_avl(AVLTree *t) {
    if (!t) return;
    libera_rec(t->root);
    t->root = NULL;
    t->size = 0;
}

void atualiza_avl(AVLTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    if (!t || !word) return;
    int inserido = 0;
    t->root = insert_rec(t->root, word, title, composer, estrofe, count, &inserido);
    if (inserido) t->size++;
}

AVLNode* busca_rec(AVLNode *n, const char *word) {
    if (!n) return NULL;
    int cmp = strcmp(word, n->data.word);
    if (cmp == 0) return n;
    if (cmp < 0) return busca_rec(n->left, word);
    return busca_rec(n->right, word);
}
Palavra_Dados *busca_avl(AVLTree *t, const char *word) {
    if (!t || !word) return NULL;
    AVLNode *n = busca_rec(t->root, word);
    return n ? &n->data : NULL;
}

void print(AVLTree *t, const char *word) {
    Palavra_Dados *e = busca_avl(t, word);
    if (!e) {
        printf("Palavra \"%s\" não encontrada no repositório (AVL).\n", word);
        return;
    }
    printf("Palavra: %s\n", e->word);
    printf("Música (maior frequência): %s\n", e->best_title);
    printf("Compositor(a): %s\n", e->best_composer);
    printf("Frequência nessa música: %d\n", e->best_freq_in_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
