#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils.h"
#include "vetor.h"
#include "bst.h"
#include "avl.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    WordVector repo;
    vetor_init(&repo);

    BSTree bstRepo;
    bst_init(&bstRepo);

    AVLTree avlRepo;
    avl_init(&avlRepo);

    while (1) {
        printf("\n=== Repositório de Letras - (Vetor / BST / AVL) ===\n");
        printf("1) Carregar arquivo de música (.txt)\n");
        printf("2) Buscar palavra (compara as 3 estruturas)\n");
        printf("3) Listar primeiras N palavras (vetor)\n");
        printf("4) Sair\n");
        printf("Escolha: ");
        int opt;
        if (scanf("%d%*c", &opt) != 1) break;

        if (opt == 1) {
            char path[512];
            printf("Caminho do arquivo (sem aspas): ");
            if (!fgets(path, sizeof(path), stdin)) continue;
            path[strcspn(path, "\n")] = 0;

            SongWord *songWords = NULL;
            int swcount = 0;
            char title[MAX_TITLE], composer[MAX_COMPOSER];
            if (processar_arquivo_musica(path, &songWords, &swcount, title, composer) != 0) {
                printf("Erro ao abrir/processar o arquivo '%s'\n", path);
                continue;
            }

            double t0 = agora_segundos();
            for (int i=0;i<swcount;i++) {
                vetor_upsert(&repo, songWords[i].word, title, composer, songWords[i].has_estrofe ? songWords[i].estrofe : "", songWords[i].count);
            }
            double t1 = agora_segundos();

            double t2 = agora_segundos();
            for (int i=0;i<swcount;i++) {
                bst_upsert(&bstRepo, songWords[i].word, title, composer, songWords[i].has_estrofe ? songWords[i].estrofe : "", songWords[i].count);
            }
            double t3 = agora_segundos();

            double t4 = agora_segundos();
            for (int i=0;i<swcount;i++) {
                avl_upsert(&avlRepo, songWords[i].word, title, composer, songWords[i].has_estrofe ? songWords[i].estrofe : "", songWords[i].count);
            }
            double t5 = agora_segundos();

            printf("Arquivo carregado. Inseridas/atualizadas %d palavras.\n", swcount);
            printf("Tempo (vetor insert): %.6f s\n", t1 - t0);
            printf("Tempo (BST insert):   %.6f s\n", t3 - t2);
            printf("Tempo (AVL insert):   %.6f s\n", t5 - t4);

            free(songWords);
        } else if (opt == 2) {
            char term[256];
            printf("Palavra para buscar: ");
            if (!fgets(term, sizeof(term), stdin)) continue;
            term[strcspn(term, "\n")] = 0;
            char norm[MAX_WORD_LEN];
            if (!normalizar_palavra(term, norm)) {
                printf("Palavra inválida.\n");
                continue;
            }

            double t0 = agora_segundos();
            vetor_print_info(&repo, norm);
            double t1 = agora_segundos();

            double t2 = agora_segundos();
            bst_print_info(&bstRepo, norm);
            double t3 = agora_segundos();

            double t4 = agora_segundos();
            avl_print_info(&avlRepo, norm);
            double t5 = agora_segundos();

            printf("(Tempo busca vetor: %.6f s)\n", t1 - t0);
            printf("(Tempo busca BST:   %.6f s)\n", t3 - t2);
            printf("(Tempo busca AVL:   %.6f s)\n", t5 - t4);
        } else if (opt == 3) {
            int N;
            printf("N: ");
            if (scanf("%d%*c", &N) != 1) continue;
            for (int i=0;i<N && i<repo.size;i++) {
                printf("%s (%d total)\n", repo.data[i].word, repo.data[i].total_freq);
            }
        } else break;
    }

    vetor_free(&repo);
    bst_free(&bstRepo);
    avl_free(&avlRepo);
    return 0;
}
