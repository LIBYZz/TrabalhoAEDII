#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

// Removeu includes específicos do Windows

int normalizar_palavra(const char *stringEntrada, char *stringSaida) {
    int j = 0;
    for (int i = 0; stringEntrada[i] != '\0' && j < 63; ++i) {
        unsigned char c = (unsigned char)stringEntrada[i];
        if (isalnum(c) || c == '\'' || c == '-') {
            stringSaida[j++] = (char)tolower(c);
        } 
    }
    stringSaida[j] = '\0';
    int len = strlen(stringSaida);
    if (len <= 3) return 0;
    return 1;
}

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

int processar_arquivo_musica(const char *filepath, Palavra_Musica **palavra_saida, int *cont_saida, char titulo[256], char autor[256]) {
    FILE *f = fopen(filepath, "r");
    if (!f) return -1;

    // Configura locale para UTF-8 (funciona em Windows/Linux)
    setlocale(LC_ALL, "en_US.UTF-8");

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

    char estrofe_buffer[4096];
    estrofe_buffer[0] = '\0';

    for (int i=0;i<nlin;i++) {
        char *ln = linhas[i];
        if (ln[0] == '\0') {
            estrofe_buffer[0] = '\0';
        } else {
            if (estrofe_buffer[0] != '\0') {
                strncat(estrofe_buffer, "\n", sizeof(estrofe_buffer)-strlen(estrofe_buffer)-1);
            }
            strncat(estrofe_buffer, ln, sizeof(estrofe_buffer)-strlen(estrofe_buffer)-1);
        }
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
                    words = (Palavra_Musica*)realloc(words, sizeof(Palavra_Musica)*(wcount+1));
                    strncpy(words[wcount].palavra, tok, 63);
                    words[wcount].palavra[63] = '\0';
                    words[wcount].count = 1;
                    words[wcount].has_estrofe = 0;
                    words[wcount].estrofe[0] = '\0';
                    if (estrofe_buffer[0] != '\0') {
                        int len = (int)strlen(estrofe_buffer);
                        int copylen = len < 100 ? len : 100;
                        strncpy(words[wcount].estrofe, estrofe_buffer, copylen);
                        words[wcount].estrofe[copylen] = '\0';
                        words[wcount].has_estrofe = 1;
                    }
                    wcount++;
                } else {
                    words[found].count++;
                    if (!words[found].has_estrofe && estrofe_buffer[0] != '\0') {
                        int len = (int)strlen(estrofe_buffer);
                        int copylen = len < 100 ? len : 100;
                        strncpy(words[found].estrofe, estrofe_buffer, copylen);
                        words[found].estrofe[copylen] = '\0';
                        words[found].has_estrofe = 1;
                    }
                }
            }
            tok = strtok(NULL, " ");
        }
    }

    for (int i=0;i<nlin;i++) free(linhas[i]);
    free(linhas);

    *palavra_saida = words;
    *cont_saida = wcount;
    return 0;
}