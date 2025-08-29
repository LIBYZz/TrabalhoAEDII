#include "avl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int calc_altura(AVLNo *n) {
    return n ? n->height : 0;
}

int cmp_max_int(int a, int b) {
    return a > b ? a : b;
}

AVLNo* novo_no_avl(const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count) {
    AVLNo *n = (AVLNo*)malloc(sizeof(AVLNo));
    if (!n) return NULL;
    // inicia tudo
    n->left = n->right = NULL;
    n->height = 1;
    n->data.palavra[0] = '\0';
    n->data.max_titulo[0] = '\0';
    n->data.max_autor[0] = '\0';
    n->data.estrofe[0] = '\0';
    n->data.max_freq_song = 0;
    n->data.total_freq = 0;

    if (palavra) {
        strncpy(n->data.palavra, palavra, 63); n->data.palavra[63] = '\0';
    }
    if (titulo) {
        strncpy(n->data.max_titulo, titulo, 255); n->data.max_titulo[255] = '\0';
    }
    if (autor) {
        strncpy(n->data.max_autor, autor, 255); n->data.max_autor[255] = '\0';
    }
    if (estrofe) {
        strncpy(n->data.estrofe, estrofe, 255);
        n->data.estrofe[255] = '\0';
    }
    n->data.max_freq_song = count;
    n->data.total_freq = count;
    return n;
}

// rotações
AVLNo* roda_direita(AVLNo *y) {
    AVLNo *x = y->left;
    AVLNo *T2 = x->right;

    /* rotação */
    x->right = y;
    y->left = T2;

    /* atualiza alturas */
    y->height = cmp_max_int(calc_altura(y->left), calc_altura(y->right)) + 1;
    x->height = cmp_max_int(calc_altura(x->left), calc_altura(x->right)) + 1;

    return x;
}

AVLNo* roda_esquerda(AVLNo *x) {
    AVLNo *y = x->right;
    AVLNo *T2 = y->left;

    /* rotação */
    y->left = x;
    x->right = T2;

    /* atualiza alturas */
    x->height = cmp_max_int(calc_altura(x->left), calc_altura(x->right)) + 1;
    y->height = cmp_max_int(calc_altura(y->left), calc_altura(y->right)) + 1;

    return y;
}
int balanceamento(AVLNo *n) {
    return n ? calc_altura(n->left) - calc_altura(n->right) : 0;
}

AVLNo* insert_rec(AVLNo *no, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count, int *inserido) {
    if (!no) {
        *inserido = 1;
        return novo_no_avl(palavra, titulo, autor, estrofe, count);
    }

    int cmp = strcmp(palavra, no->data.palavra);
    if (cmp == 0) {
        /* atualiza existente */
        no->data.total_freq += count;
        if (count > no->data.max_freq_song) {
            if (titulo) {
                strncpy(no->data.max_titulo, titulo, 255); no->data.max_titulo[255] = '\0';
            }
            if (autor) {
                strncpy(no->data.max_autor, autor, 255); no->data.max_autor[255] = '\0';
            }
            if (estrofe) {
                strncpy(no->data.estrofe, estrofe, 255); no->data.estrofe[255] = '\0';
            }
            no->data.max_freq_song = count;
        }
        return no;
    } else if (cmp < 0) {
        no->left = insert_rec(no->left, palavra, titulo, autor, estrofe, count, inserido);
    } else {
        no->right = insert_rec(no->right, palavra, titulo, autor, estrofe, count, inserido);
    }

    /* atualiza altura e fator de balanceamento */
    no->height = 1 + cmp_max_int(calc_altura(no->left), calc_altura(no->right));
    int bf = balanceamento(no);

    /* casos de rotação (com checagem de ponteiros por segurança) */
    /* LL */
    if (bf > 1 && no->left != NULL && strcmp(palavra, no->left->data.palavra) < 0)
        return roda_direita(no);
    /* RR */
    if (bf < -1 && no->right != NULL && strcmp(palavra, no->right->data.palavra) > 0)
        return roda_esquerda(no);
    /* LR */
    if (bf > 1 && no->left != NULL && strcmp(palavra, no->left->data.palavra) > 0) {
        no->left = roda_esquerda(no->left);
        return roda_direita(no);
    }
    /* RL */
    if (bf < -1 && no->right != NULL && strcmp(palavra, no->right->data.palavra) < 0) {
        no->right = roda_direita(no->right);
        return roda_esquerda(no);
    }

    return no;
}

void inicia_avl(AVLArv *t) {
    if (t) {
        t->root = NULL;
        t->tamanho = 0;
    }
}

void libera_rec(AVLNo *n) {
    if (!n) return;
    libera_rec(n->left);
    libera_rec(n->right);
    free(n);
}
void libera_avl(AVLArv *t) {
    if (!t) return;
    libera_rec(t->root);
    t->root = NULL;
    t->tamanho = 0;
}

void atualiza_avl(AVLArv *t, const char *palavra, const char *titulo, const char *autor, const char *estrofe, int count) {
    if (!t || !palavra) return;
    int inserido = 0;
    t->root = insert_rec(t->root, palavra, titulo, autor, estrofe, count, &inserido);
    if (inserido) t->tamanho++;
}

AVLNo* busca_rec(AVLNo *n, const char *palavra) {
    if (!n) return NULL;
    int cmp = strcmp(palavra, n->data.palavra);
    if (cmp == 0) return n;
    if (cmp < 0) return busca_rec(n->left, palavra);
    return busca_rec(n->right, palavra);
}
Palavra_Dados *busca_avl(AVLArv *t, const char *palavra) {
    if (!t || !palavra) return NULL;
    AVLNo *n = busca_rec(t->root, palavra);
    return n ? &n->data : NULL;
}

void print_avl(AVLArv *t, const char *palavra) {
    Palavra_Dados *e = busca_avl(t, palavra);
    if (!e) {
        printf("Palavra \"%s\" não encontrada no repositório (AVL).\n", palavra);
        return;
    }
    printf("Palavra: %s\n", e->palavra);
    printf("Música (maior frequência): %s\n", e->max_titulo);
    printf("Compositor(a): %s\n", e->max_autor);
    printf("Frequência nessa música: %d\n", e->max_freq_song);
    printf("Total no repositório: %d\n", e->total_freq);
    printf("Trecho da estrofe: %s\n", e->estrofe);
}
