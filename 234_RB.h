#ifndef _234_RB_H_
#define _234_RB_H_

#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
// Typedefs para facilitar uso das estruturas
// ---------------------------------------------------------------------------

typedef struct NO noRB;   // Nó de árvore Rubro-Negra
typedef struct RB rb;     // Árvore Rubro-Negra
typedef struct NO234 no234;  // Nó de árvore 2-3-4
typedef struct arvore234 arv234; // Árvore 2-3-4

// ---------------------------------------------------------------------------
// Protótipos das funções Rubro-Negra
// ---------------------------------------------------------------------------

// Aloca e retorna uma nova árvore Rubro-Negra vazia
rb* alocaArvore(void);

// Aloca e retorna um novo nó com chave dada em uma RB-tree
noRB* alocaNo(rb *arv, int chave);

// Insere 'novoNo' na árvore, atualizando contadores de rotações e merges
void insereNo(rb *arv, noRB *novoNo, int *contador_rotacoes_rb, int *contador_merges_rb);

// Remove o nó com valor 'valor'; retorna 1 se removido, 0 se não encontrado
int removeNo(rb *arv, int valor, int *contador_rotacoes_rb, int *contador_merges_rb);

// Percorre a árvore em pré-ordem imprimindo chave, cor e altura preta
void percorrePreOrdem(rb *arv, noRB *aux);

// Retorna ponteiro para a raiz real da RB-tree
noRB* retornaRaiz(rb *arv);

// Rebalanceia após inserção, a partir de 'novoNo'
void balanceamentoInsercao(rb *arv, noRB *novoNo, int *contador_rotacoes_rb, int *contador_merges_rb);

// Rebalanceia após remoção, a partir de 'no' e seu 'pai'
void balanceamentoRemocao(rb *arv, noRB *no, noRB *pai);

// Rotação à esquerda em torno de 'noDesbalanceado'
void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado);

// Rotação à direita em torno de 'noDesbalanceado'
void rotacaoDireita(rb *arv, noRB *noDesbalanceado);

// Atualiza black-height em todo o subárvore de 'noAux'; retorna altura preta
int atualiza_Altura_Preto(rb *arv, noRB *noAux);

// Atualiza o campo alturaPreto da RB-tree com valor da raiz
void atualiza_Altura_Preto_RB(rb *arv);

// ---------------------------------------------------------------------------
// Protótipos das funções 2-3-4
// ---------------------------------------------------------------------------

// Cria e retorna uma nova árvore 2-3-4 vazia
arv234* criaArvore234(void);

// Cria e retorna um novo nó 2-3-4; 'folha' = 1 se for folha, 0 caso contrário
no234* criaNo234(int folha);

// Libera recursivamente todos os nós da subárvore em 'raiz'
void libera234(no234* raiz);

// Impressão em ordem para depuração
void emOrdem234(no234* raiz);

// Divide o filho 'filhos[i]' de 'pai'; incrementa splits, rotacoes e merges
no234* splitFilho(no234* pai, int i, arv234* arv);

// Insere 'chave' em nó não-cheio; incrementa comparações
void insereNaoCheio(no234* no, int chave, arv234* arv);

// Insere 'chave' na árvore 2-3-4; retorna ponteiro para a raiz após inserção
no234* insere234(arv234* arv, int chave);

// Converte recursivamente nó 2-3-4 em subárvore RB, retornando nó RB raiz
noRB* converteDireto(no234* no, rb* arv, noRB* pai);

// Converte toda a 2-3-4 para uma RB-tree nova; retorna nova RB-tree
rb* converte234paraRB(arv234* arvOrigem);

// Remove 'chave' da subárvore apontada por 'no' na árvore 2-3-4
void remove234(arv234 *arv, no234 *no, int chave);

// Versão alternativa: remove 'chave' de subárvore com raiz 'raiz'
void remove234(no234 *raiz, int chave, arv234 *arv);

// Conta nós ocupados na subárvore com raiz 'raiz'
int contaBlocosOcupados(no234* raiz);

#endif // _234_RB_H_
