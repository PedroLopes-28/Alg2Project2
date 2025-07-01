//
// Created by Pedro on 30/06/2025.
//

#ifndef RB_H
#define RB_H
// Estruturas de dados
typedef struct noRB {
    int chave;
    char cor; // 'P' = preto, 'V' = vermelho
    struct noRB *esq;
    struct noRB *dir;
    struct noRB *pai;
    int alturaPreto;
} noRB;

typedef struct rb {
    noRB *sentinela;
    int alturaPreto;
} rb;



// --- Funções da Red Black ---

//Alocação
noRB* alocaNo(rb *arv, int chave);
rb*   alocaArvore(void);

//Inserção e remoção
void   insereNo(rb *arv, noRB *novoNo);
int    removeNo(rb *arv, int valor);

//Retorna a raiz da arvore
noRB*  retornaRaiz(rb *arv);

//Balanceamentos e rotações
void   balanceamentoInsercao(rb *arv, noRB *novoNo);
void   balanceamentoRemocao(rb *arv, noRB *no, noRB *pai);
void   rotacaoEsquerda(rb *arv, noRB *no);
void   rotacaoDireita(rb *arv, noRB *no);

//Operações envolvendo altura de preto
int    atualiza_Altura_Preto(rb *arv, noRB *noAux);
void   atualiza_Altura_Preto_RB(rb *arv);
int    alturaRB(noRB *no);

//print da arvore completa
void   imprimeNivelComNull(noRB *no, int nivel);
void   imprimeNosPorNivelRB(rb *arv);


#endif //RB_H
