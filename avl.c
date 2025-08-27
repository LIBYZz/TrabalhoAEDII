/* avl.c - implementação corrigida da AVL */
#include "avl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int height(AVLNode *n) { return n ? n->height : 0; }
static int max_int(int a, int b) { return a > b ? a : b; }

static AVLNode* new_node(const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    AVLNode *n = (AVLNode*)malloc(sizeof(AVLNode));
    if (!n) return NULL;
    /* inicializa tudo */
    n->left = n->right = NULL;
    n->height = 1;
    n->data.word[0] = '\0';
    n->data.best_title[0] = '\0';
    n->data.best_composer[0] = '\0';
    n->data.estrofe[0] = '\0';
    n->data.best_freq_in_song = 0;
    n->data.total_freq = 0;

    if (word) {
        strncpy(n->data.word, word, MAX_WORD_LEN-1); n->data.word[MAX_WORD_LEN-1] = '\0';
    }
    if (title) {
        strncpy(n->data.best_title, title, MAX_TITLE-1); n->data.best_title[MAX_TITLE-1] = '\0';
    }
    if (composer) {
        strncpy(n->data.best_composer, composer, MAX_COMPOSER-1); n->data.best_composer[MAX_COMPOSER-1] = '\0';
    }
    if (estrofe) {
        strncpy(n->data.estrofe, estrofe, MAX_ESTROFE-1);
        n->data.estrofe[MAX_ESTROFE-1] = '\0';
    }
    n->data.best_freq_in_song = count;
    n->data.total_freq = count;
    return n;
}

/* rotações */
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    /* rotação */
    x->right = y;
    y->left = T2;

    /* atualiza alturas */
    y->height = max_int(height(y->left), height(y->right)) + 1;
    x->height = max_int(height(x->left), height(x->right)) + 1;

    return x;
}
static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    /* rotação */
    y->left = x;
    x->right = T2;

    /* atualiza alturas */
    x->height = max_int(height(x->left), height(x->right)) + 1;
    y->height = max_int(height(y->left), height(y->right)) + 1;

    return y;
}
static int balance_factor(AVLNode *n) { return n ? height(n->left) - height(n->right) : 0; }

static AVLNode* insert_rec(AVLNode *node, const char *word, const char *title, const char *composer, const char *estrofe, int count, int *did_insert) {
    if (!node) {
        *did_insert = 1;
        return new_node(word, title, composer, estrofe, count);
    }

    int cmp = strcmp(word, node->data.word);
    if (cmp == 0) {
        /* atualiza existente */
        node->data.total_freq += count;
        if (count > node->data.best_freq_in_song) {
            if (title) {
                strncpy(node->data.best_title, title, MAX_TITLE-1); node->data.best_title[MAX_TITLE-1] = '\0';
            }
            if (composer) {
                strncpy(node->data.best_composer, composer, MAX_COMPOSER-1); node->data.best_composer[MAX_COMPOSER-1] = '\0';
            }
            if (estrofe) {
                strncpy(node->data.estrofe, estrofe, MAX_ESTROFE-1); node->data.estrofe[MAX_ESTROFE-1] = '\0';
            }
            node->data.best_freq_in_song = count;
        }
        return node;
    } else if (cmp < 0) {
        node->left = insert_rec(node->left, word, title, composer, estrofe, count, did_insert);
    } else {
        node->right = insert_rec(node->right, word, title, composer, estrofe, count, did_insert);
    }

    /* atualiza altura e fator de balanceamento */
    node->height = 1 + max_int(height(node->left), height(node->right));
    int bf = balance_factor(node);

    /* casos de rotação (com checagem de ponteiros por segurança) */
    /* LL */
    if (bf > 1 && node->left != NULL && strcmp(word, node->left->data.word) < 0)
        return rotate_right(node);
    /* RR */
    if (bf < -1 && node->right != NULL && strcmp(word, node->right->data.word) > 0)
        return rotate_left(node);
    /* LR */
    if (bf > 1 && node->left != NULL && strcmp(word, node->left->data.word) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    /* RL */
    if (bf < -1 && node->right != NULL && strcmp(word, node->right->data.word) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

void avl_init(AVLTree *t) { if (t) { t->root = NULL; t->size = 0; } }

static void free_rec(AVLNode *n) {
    if (!n) return;
    free_rec(n->left);
    free_rec(n->right);
    free(n);
}
void avl_free(AVLTree *t) { if (!t) return; free_rec(t->root); t->root = NULL; t->size = 0; }

void avl_upsert(AVLTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count) {
    if (!t || !word) return;
    int did_insert = 0;
    t->root = insert_rec(t->root, word, title, composer, estrofe, count, &did_insert);
    if (did_insert) t->size++;
}

static AVLNode* search_rec(AVLNode *n, const char *word) {
    if (!n) return NULL;
    int cmp = strcmp(word, n->data.word);
    if (cmp == 0) return n;
    if (cmp < 0) return search_rec(n->left, word);
    return search_rec(n->right, word);
}
WordEntry *avl_search(AVLTree *t, const char *word) {
    if (!t || !word) return NULL;
    AVLNode *n = search_rec(t->root, word);
    return n ? &n->data : NULL;
}

void avl_print_info(AVLTree *t, const char *word) {
    WordEntry *e = avl_search(t, word);
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
