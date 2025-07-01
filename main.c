#include <stdio.h>
#include <stdlib.h>
#include "234.h"
#include "conversao.h"
#include "Rb.h"

// Inclua os headers com as funções da 2-3-4 e RB

void menu234(arv234 *arv);
void menuRB(rb *arv);

int main() {
    char nomeArquivo[100];
    FILE *arquivo;
    int valor;

    arv234* arv = criaArvore234();

    printf("Digite o nome do arquivo de entrada: ");
    scanf("%s", nomeArquivo);

    arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (fscanf(arquivo, "%d", &valor) == 1) {
        insere234(arv, valor);
    }

    fclose(arquivo);

    printf("\nÁrvore 2-3-4 criada a partir do arquivo:\n");
    imprimeNosPorNivel234(arv);
    printf("\n");

    menu234(arv);

    libera234(arv->raiz);
    free(arv);

    return 0;
}

