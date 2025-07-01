#include "234.h"
#include "Rb.h"
#include "conversao.h"
#include <stdlib.h>
#include <stdio.h>


// Conversão recursiva direta de um nó 2-3-4 para Rubro-Negra
noRB* converteDireto(no234* no, rb* arv, noRB* pai) {
    if (!no) return NULL;

    // Caso com 1 chave
    if (no->nChaves == 1) {
        noRB* r = alocaNo(arv, no->chaves[0]);
        r->cor = 'P';
        r->pai = pai;

        r->esq = converteDireto(no->filhos[0], arv, r);
        r->dir = converteDireto(no->filhos[1], arv, r);
        return r;
    }

    // Caso com 2 chaves
    if (no->nChaves == 2) {
        noRB* r = alocaNo(arv, no->chaves[1]);
        r->cor = 'P';
        r->pai = pai;

        noRB* esq = alocaNo(arv, no->chaves[0]);
        esq->cor = 'V';
        esq->pai = r;

        r->esq = esq;
        r->dir = converteDireto(no->filhos[2], arv, r);

        esq->esq = converteDireto(no->filhos[0], arv, esq);
        esq->dir = converteDireto(no->filhos[1], arv, esq);

        return r;
    }

    // Caso 3 chaves
    if (no->nChaves == 3) {
        noRB* r = alocaNo(arv, no->chaves[1]);
        r->cor = 'P';
        r->pai = pai;

        noRB* esq = alocaNo(arv, no->chaves[0]);
        esq->cor = 'V';
        esq->pai = r;

        noRB* dir = alocaNo(arv, no->chaves[2]);
        dir->cor = 'V';
        dir->pai = r;

        r->esq = esq;
        r->dir = dir;

        esq->esq = converteDireto(no->filhos[0], arv, esq);
        esq->dir = converteDireto(no->filhos[1], arv, esq);

        dir->esq = converteDireto(no->filhos[2], arv, dir);
        dir->dir = converteDireto(no->filhos[3], arv, dir);

        return r;
    }

    return NULL;
}

// Função principal de conversão 2-3-4 → Rubro-Negra
rb* converte234paraRB(arv234* arvOrigem) {
    rb* novaRB = alocaArvore();
    novaRB->sentinela->dir = converteDireto(arvOrigem->raiz, novaRB, novaRB->sentinela);
    atualiza_Altura_Preto(novaRB, novaRB->sentinela->dir);
    atualiza_Altura_Preto_RB(novaRB);
    return novaRB;
}

void menu234(arv234* arv) {
    int opcao, valor;
    rb *arvRB = NULL;

    do {
        printf("\nÁrvores Balanceadas: Menu (2-3-4)\n");
        printf("1. Inserir novo elemento na árvore 2-3-4\n");
        printf("2. Remover elemento da árvore 2-3-4\n");
        printf("3. Imprimir árvore 2-3-4\n");
        printf("4. Converter em uma árvore rubro-negra\n");
        printf("5. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                insere234(arv, valor);
                printf("\n");
                printf("Altura: %d\nSplits: %d\nMerges: %d\nRotacoes: %d\nComparacoes: %d", arv->altura, arv->splits, arv->merges, arv->rotacoes, arv->comparacoes);
                printf("\n");
                break;
            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                int ok = removeKey(arv, valor);
                printf("\n");
                if(ok == 1){
                    printf("Elemento Removido com Sucesso!\n");
                }
                else if(ok == 0){
                    printf("Esse Elemento Não Existe!\n");
                }
                printf("\n");
                printf("Altura: %d\nSplits: %d\nMerges: %d\nRotacoes: %d\nComparacoes: %d", arv->altura, arv->splits, arv->merges, arv->rotacoes, arv->comparacoes);
                printf("\n");
                break;
            case 3:
                printf("Árvore 2-3-4:\n");
                imprimeNosPorNivel234(arv);
                printf("\n");
                printf("\n");
                altura234(retornaRaiz234(arv));
                printf("Altura: %d\nSplits: %d\nMerges: %d\nRotacoes: %d\nComparacoes: %d\nBlocos: %d", arv->altura, arv->splits, arv->merges, arv->rotacoes, arv->comparacoes,contaBlocosOcupados(arv->raiz));
                printf("\n");
                break;
            case 4:
                arvRB = converte234paraRB(arv);
                printf("Árvore Rubro-Negra (Pré-Ordem):\n");
                imprimeNosPorNivelRB(arvRB);
                menuRB(arvRB);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            case 6:
                char nomeArquivo[50];
                printf("Digite o nome do arquivo com os números a remover: ");
                scanf("%s", nomeArquivo);
                remove_numeros_do_arquivo(arv, nomeArquivo);
            break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 5);
}

void menuRB(rb* arv) {
    int opcao, valor;

    do {
        printf("\nÁrvores Balanceadas: Menu (Rubro-Negra)\n");
        printf("1. Inserir novo elemento na árvore rubro-negra\n");
        printf("2. Remover elemento da árvore rubro-negra\n");
        printf("3. Imprimir árvore rubro-negra (Pré-Ordem)\n");
        printf("4. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                noRB* novoNo = alocaNo(arv, valor);
                insereNo(arv, novoNo);
                break;
            }
            case 2:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                removeNo(arv, valor);
                break;
            case 3:
                printf("Árvore Rubro-Negra:\n");
                imprimeNosPorNivelRB(arv);
                break;
            case 4:
                printf("Saindo da árvore rubro-negra...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 4);
}

