#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

// remove pontuação e deixa minúsculo
int normalizar_palavra(const char *stringEntrada, char *stringSaida) {
    int j = 0;
    for (int i = 0; stringEntrada[i] != '\0' && j < 63; ++i) {
        unsigned char c = (unsigned char)stringEntrada[i];
        if (isalnum(c) || c == '\'' || c == '-') { // permite números e ' - (ajustar se quiser)
            stringSaida[j++] = (char)tolower(c);
        } 
    }
    stringSaida[j] = '\0';
    int len = strlen(stringSaida);
    if (len <= 3) return 0;
    return 1;
}

// evita problemas de comparação tirando caracteres finais de pular linha (L:\n, W:\r\n)
void remove_newline(char *s) {
    size_t tam = strlen(s);
    while (tam > 0 && (s[tam-1]=='\n' || s[tam-1]=='\r')){
        s[tam-1] = '\0';
        tam--;
    }
}


double agora_segundos() {
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

// processa o arquivo e produz um vetor de Palavra_Musica (alocado aqui; caller deve free)
// ... código anterior ...

int processar_arquivo_musica(const char *filepath, Palavra_Musica **palavra_saida, int *cont_saida, char titulo[256], char autor[256]) {
    FILE *f = fopen(filepath, "r");
    if (!f) return -1;

    char line[1024];
    if (!fgets(line, sizeof(line), f)) { fclose(f); return -1; }
    remove_newline(line);
    strncpy(titulo, line, 256-1); titulo[256-1]='\0';

    if (!fgets(line, sizeof(line), f)) { autor[0]='\0'; }
    else { remove_newline(line); strncpy(autor, line, 256-1); autor[256-1]='\0'; }

    char **linhas = NULL;
    int nlin = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        remove_newline(line);
        linhas = (char**)realloc(linhas, sizeof(char*)*(nlin+1));
        linhas[nlin] = strdup(line);
        nlin++;
    }
    fclose(f);

    Palavra_Musica *words = NULL;
    int wcount = 0;

    // Variáveis para controlar a estrofe atual
    char estrofe_buffer[4096] = {0};
    int *current_stanza_word_indices = NULL;
    int current_stanza_word_count = 0;

    for (int i=0;i<nlin;i++) {
        char *ln = linhas[i];
        if (ln[0] == '\0') {
            // Final da estrofe - atualizar todas as palavras desta estrofe
            for (int j = 0; j < current_stanza_word_count; j++) {
                int idx = current_stanza_word_indices[j];
                if (!words[idx].has_estrofe) {
                    strncpy(words[idx].estrofe, estrofe_buffer, 255);
                    words[idx].estrofe[255] = '\0';
                    words[idx].has_estrofe = 1;
                }
            }
            // Resetar para a próxima estrofe
            free(current_stanza_word_indices);
            current_stanza_word_indices = NULL;
            current_stanza_word_count = 0;
            estrofe_buffer[0] = '\0';
        } else {
            // Adicionar linha ao buffer da estrofe
            if (estrofe_buffer[0] != '\0') {
                strncat(estrofe_buffer, "\n", sizeof(estrofe_buffer)-strlen(estrofe_buffer)-1);
            }
            strncat(estrofe_buffer, ln, sizeof(estrofe_buffer)-strlen(estrofe_buffer)-1);

            // Processar palavras da linha
            char tmp[1024];
            strncpy(tmp, ln, sizeof(tmp)-1); tmp[sizeof(tmp)-1]='\0';
            for (int k=0; tmp[k]; ++k) {
                unsigned char c = (unsigned char)tmp[k];
                if (!isalnum(c) && tmp[k] != '\'' && tmp[k] != '-') tmp[k] = ' ';
                else tmp[k] = (char)tolower(c);
            }
            char *tok = strtok(tmp, " ");
            while (tok) {
                if ((int)strlen(tok) > 3) {
                    int found = -1;
                    for (int u=0; u<wcount; ++u) {
                        if (strcmp(words[u].palavra, tok) == 0) { found = u; break; }
                    }
                    if (found == -1) {
                        // Nova palavra
                        words = (Palavra_Musica*)realloc(words, sizeof(Palavra_Musica)*(wcount+1));
                        strncpy(words[wcount].palavra, tok, 63);
                        words[wcount].palavra[63] = '\0';
                        words[wcount].count = 1;
                        words[wcount].has_estrofe = 0;
                        words[wcount].estrofe[0] = '\0';

                        // Adicionar à lista de palavras da estrofe atual
                        current_stanza_word_count++;
                        current_stanza_word_indices = (int*)realloc(current_stanza_word_indices, 
                            current_stanza_word_count * sizeof(int));
                        current_stanza_word_indices[current_stanza_word_count-1] = wcount;

                        wcount++;
                    } else {
                        // Palavra existente - incrementar contador
                        words[found].count++;

                        // Verificar se já está na estrofe atual
                        int in_current = 0;
                        for (int k = 0; k < current_stanza_word_count; k++) {
                            if (current_stanza_word_indices[k] == found) {
                                in_current = 1;
                                break;
                            }
                        }
                        if (!in_current) {
                            current_stanza_word_count++;
                            current_stanza_word_indices = (int*)realloc(current_stanza_word_indices, 
                                current_stanza_word_count * sizeof(int));
                            current_stanza_word_indices[current_stanza_word_count-1] = found;
                        }
                    }
                }
                tok = strtok(NULL, " ");
            }
        }
    }

    // Processar a última estrofe se não terminou com linha vazia
    if (current_stanza_word_count > 0) {
        for (int j = 0; j < current_stanza_word_count; j++) {
            int idx = current_stanza_word_indices[j];
            if (!words[idx].has_estrofe) {
                strncpy(words[idx].estrofe, estrofe_buffer, 255);
                words[idx].estrofe[255] = '\0';
                words[idx].has_estrofe = 1;
            }
        }
        free(current_stanza_word_indices);
    }

    for (int i=0;i<nlin;i++) free(linhas[i]);
    free(linhas);

    *palavra_saida = words;
    *cont_saida = wcount;
    return 0;
}
