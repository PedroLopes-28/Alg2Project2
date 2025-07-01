//
// Created by Pedro on 30/06/2025.
//

#ifndef INC_234_H
#define INC_234_H

#include <stddef.h>


// Estruturas de dados

typedef struct no234 {
    int nChaves;       // número de chaves atualmente no nó (0–3)
    int chaves[3];     // as chaves armazenadas no nó
    struct no234 *filhos[4];     // ponteiros para até 4 filhos
    struct no234 *pai;           // ponteiro para o pai (NULL na raiz)
    int folha;         // 1 = nó folha, 0 = nó interno
} no234 ;

typedef struct arv234{
    no234 *raiz;      // ponteiro para o nó raiz
    int    altura;    // número de níveis da árvore
    int    splits;    // contagem de splits realizados
    int    merges;    // contagem de merges realizados
    int    rotacoes;  // contagem de empréstimos (rotacoes conceitualmente)
    int    comparacoes;
}arv234;


// --- Funções da arvore 234 ---
//Alocação e liberação
arv234* criaArvore234(void);
no234*  criaNo234(int folha);
void    libera234(no234 *raiz);
no234 *retornaRaiz234(arv234 *arv);


//Funções de remoção
void    removePredecessor(arv234 *arv, no234 *x, int idx);
int     encontrarIndice(no234 *x, int key);
int     getPredecessor(no234 *x, int idx);
int     removeDeInterno(arv234 *arv, no234 *x, int idx);
int     removeKey(arv234 *arv, int key);
void remove_numeros_do_arquivo(arv234 *arv, const char *nomeArquivo);


//Balanceamento da remoção
void    preencher(no234 *no, int idx, arv234 *arv);
void    emprestaDoAnterior(no234 *no, int idx, arv234 *arv);
void    emprestaDoProximo(no234 *no, int idx, arv234 *arv);
void    funde(no234 *no, int idx, arv234 *arv);


//Conta dados da arvore
int     contaBlocosOcupados(no234 *raiz);
int     altura234(no234 *no);

//Imprime a arvore por completo
void    imprimeNivel234(no234 *no, int nivel);
void    imprimeNosPorNivel234(arv234 *arv);


//Operações de inserção
void    splitFilho(no234 *parent, int i, arv234 *arv);
void    splitChild(no234 *x, int i, arv234 *arv);
void    insertNonFull(no234 *x, int k, arv234 *arv);
no234*  insere234(arv234 *arv, int k);




#endif //INC_234_H
