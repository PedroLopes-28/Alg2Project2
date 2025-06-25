#ifndef RB_RB_H
#define RB_RB_H

// ===== DECLARAÇÕES DA ÁRVORE 2-3-4 =====

// ===== DECLARAÇÕES DA ÁRVORE 2-3-4 =====

typedef struct NO234 no234;
typedef struct arvore234 arv234;

// Aloca e inicializa uma árvore 2-3-4
arv234 *criaArvore234();

// Aloca e inicializa um novo nó 2-3-4
no234 *criaNo234(int folha);

// Insere uma chave na árvore 2-3-4 com contadores
no234 *insere234(arv234 *arv, int chave);

// Remove uma chave na árvore 2-3-4 (baseado no Cormen)
no234 *remove234(no234 *raiz, int chave, arv234 *arv);

// Conta blocos ocupados na árvore 2-3-4
int contaBlocosOcupados(no234 *raiz);

// Encontra o índice da chave em um nó 2-3-4 (como no Cormen)
int encontrarIndice(no234 *no, int chave);

// Impressão em ordem (debug)
void emOrdem234(no234 *raiz);

// Libera memória da árvore 2-3-4
void libera234(no234 *raiz);

// Split da raiz (se cheia)
void splitRaiz(arv234 *arv);

// Split do filho i do nó pai
void splitFilho(no234 *pai, int idx, arv234 *arv);

// ==== Funções auxiliares da remoção (modelo Cormen) ====
no234 *removeDeFolha(no234 *raiz, int idx);
no234 *removeDeNaoFolha(no234 *raiz, int idx, arv234 *arv);
void preencher(no234 *raiz, int idx, arv234 *arv);
void funde(no234 *raiz, int idx, arv234 *arv);
void pegarDoAnterior(no234 *raiz, int idx);
void pegarDoProximo(no234 *raiz, int idx);



// ===== DECLARAÇÕES DA ÁRVORE RUBRO-NEGRA =====

typedef struct RB rb;
typedef struct NO noRB;

// Aloca árvore RB
rb *alocaArvore();

// Aloca nó RB
noRB *alocaNo(rb *arv, int chave);

// Insere nó RB
void insereNo(rb *arv, noRB *novoNo);

// Remove nó da árvore
int removeNo(rb *arv, int valor);


// Percorre em pré-ordem
void percorrePreOrdem(rb *arv, noRB *aux);

// Retorna raiz
noRB *retornaRaiz(rb *arv);


// ===== BALANCEAMENTO RB =====

void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado);
void rotacaoDireita(rb *arv, noRB *noDesbalanceado);
void balanceamentoInsercao(rb *arv, noRB *novoNo);
void balanceamentoRemocao(rb *arv, noRB *no, noRB *pai);
int atualiza_Altura_Preto(rb *arv, noRB *noAux);
void atualiza_Altura_Preto_RB(rb *arv);
void atualiza_Sentinela_Folha(rb *T1, rb *T2, rb *novaRB, noRB *aux);


// ===== CONVERSÃO 2-3-4 PARA RB =====

rb *converte234paraRB(arv234 *arvoreOrigem);
void insereChaves234emRB(rb *arvRB, no234 *no);

#endif // RB_RB_H
