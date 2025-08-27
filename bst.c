#include "bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// helpers
static BSTNode* new_node(const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song) {
    BSTNode *n = (BSTNode*)malloc(sizeof(BSTNode));
    if (!n) return NULL;
    strncpy(n->data.word, word, MAX_WORD_LEN-1); n->data.word[MAX_WORD_LEN-1] = '\0';
    strncpy(n->data.best_title, title, MAX_TITLE-1); n->data.best_title[MAX_TITLE-1] = '\0';
    strncpy(n->data.best_composer, composer, MAX_COMPOSER-1); n->data.best_composer[MAX_COMPOSER-1] = '\0';
    if (estrofe) strncpy(n->data.estrofe, estrofe, MAX_ESTROFE-1);
    n->data.estrofe[MAX_ESTROFE-1] = '\0';
    n->data.best_freq_in_song = count_in_song;
    n->data.total_freq = count_in_song;
    n->left = n->right = NULL;
    return n;
}

void bst_init(BSTree *t) {
    t->root = NULL;
    t->size = 0;
}

static void free_node_recursive(BSTNode *n) {
    if (!n) return;
    free_node_recursive(n->left);
    free_node_recursive(n->right);
    free(n);
}

void bst_free(BSTree *t) {
    if (!t) return;
    free_node_recursive(t->root);
    t->root = NULL;
    t->size = 0;
}

// recursive upsert
static BSTNode* bst_upsert_rec(BSTNode *node, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song, int *did_insert) {
    if (node == NULL) {
        *did_insert = 1;
        return new_node(word, title, composer, estrofe, count_in_song);
    }

    int cmp = strcmp(word, node->data.word);
    if (cmp == 0) {
        // update existing
        node->data.total_freq += count_in_song;
        if (count_in_song > node->data.best_freq_in_song) {
            // update best music info
            strncpy(node->data.best_title, title, MAX_TITLE-1); node->data.best_title[MAX_TITLE-1] = '\0';
            strncpy(node->data.best_composer, composer, MAX_COMPOSER-1); node->data.best_composer[MAX_COMPOSER-1] = '\0';
            if (estrofe) {
                strncpy(node->data.estrofe, estrofe, MAX_ESTROFE-1); node->data.estrofe[MAX_ESTROFE-1] = '\0';
            }
            node->data.best_freq_in_song = count_in_song;
        }
        return node;
    } else if (cmp < 0) {
        node->left = bst_upsert_rec(node->left, word, title, composer, estrofe, count_in_song, did_insert);
    } else {
        node->right = bst_upsert_rec(node->right, word, title, composer, estrofe, count_in_song, did_insert);
    }
    return node;
}

void bst_upsert(BSTree *t, const char *word, const char *title, const char *composer, const char *estrofe, int count_in_song) {
    if (!t || !word) return;
    int did_insert = 0;
    t->root = bst_upsert_rec(t->root, word, title, composer, estrofe, count_in_song, &did_insert);
    if (did_insert) t->size++;
}

// search
static BSTNode* bst_search_rec(BSTNode *node, const char *word) {
    if (!node) return NULL;
    int cmp = strcmp(word, node->data.word);
    if (cmp == 0) return node;
    if (cmp < 0) return bst_search_rec(node->left, word);
    return bst_search_rec(node->right, word);
}

WordEntry *bst_search(BSTree *t, const char *word) {
    if (!t) return NULL;
    BSTNode *n = bst_search_rec(t->root, word);
    if (!n) return NULL;
    return &n->data;
}

void bst_print_info(BSTree *t, const char *word) {
    WordEntry *e = bst_search(t, word);
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
