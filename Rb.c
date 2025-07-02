
#include "Rb.h"
#include <stdio.h>
#include <stdlib.h>


//Aloca um no da arvore rb
noRB *alocaNo(rb *arv, int chave){
    noRB* novoNo = (noRB*) malloc(sizeof(noRB));

    novoNo->chave = chave;
    novoNo->cor = 'V';

    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->pai = NULL;

    novoNo->alturaPreto = 0;

    return novoNo;
}
//Aloca a arvore RB
rb *alocaArvore(){
    rb* novaArvore = (rb*) malloc(sizeof(rb));

    noRB* sentinela = alocaNo(novaArvore, -100000);

    sentinela->cor = 'P';

    novaArvore->sentinela = sentinela;
    novaArvore->alturaPreto = 0;

    return novaArvore;
}

//Insere um nó na arvore rb
void insereNo(rb *arv, noRB *novoNo) {
    noRB* aux = arv->sentinela->dir;
    noRB* auxPai = arv->sentinela;

    // Desce até NULL para encontrar posição de inserção
    while (aux != NULL) {
        auxPai = aux;
        if (novoNo->chave < aux->chave)
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    // Insere como filho de auxPai
    if (novoNo->chave < auxPai->chave)
        auxPai->esq = novoNo;
    else
        auxPai->dir = novoNo;
    novoNo->pai = auxPai;

    // Ajusta cores e rotações após inserir
    balanceamentoInsercao(arv, novoNo);

    // Atualiza altura preta de cada nó e da árvore
    atualiza_Altura_Preto(arv, arv->sentinela->dir);
    atualiza_Altura_Preto_RB(arv);
}





int removeNo(rb *arv, int valor){
    noRB *noRemover = arv->sentinela->dir;
    while(noRemover && noRemover->chave != valor){
        if(valor < noRemover->chave)
            noRemover = noRemover->esq;
        else
            noRemover = noRemover->dir;
    }
    if(!noRemover) return 0;
    noRB *y = noRemover;
    char corOriginal = y->cor;
    noRB *x, *xPai;
    if(noRemover->esq == NULL){
        x = noRemover->dir;
        xPai = noRemover->pai;
        if(noRemover == xPai->esq) xPai->esq = x;
        else                       xPai->dir = x;
        if(x) x->pai = xPai;
    } else if(noRemover->dir == NULL){
        x = noRemover->esq;
        xPai = noRemover->pai;
        if(noRemover == xPai->esq) xPai->esq = x;
        else                       xPai->dir = x;
        if(x) x->pai = xPai;
    } else {
        y = noRemover->esq;
        while(y->dir) y = y->dir;
        corOriginal = y->cor;
        x    = y->esq;
        xPai = y->pai;
        if(y->pai != noRemover){
            if(x) x->pai = y->pai;
            y->pai->dir = x;
            y->esq = noRemover->esq;
            y->esq->pai = y;
        }
        if(noRemover->pai->esq == noRemover)
            noRemover->pai->esq = y;
        else
            noRemover->pai->dir = y;
        y->pai = noRemover->pai;
        y->dir = noRemover->dir;
        if(y->dir) y->dir->pai = y;
        y->cor = noRemover->cor;
    }
    if(corOriginal == 'P')
        balanceamentoRemocao(arv, x, xPai);
    free(noRemover);
    atualiza_Altura_Preto_RB(arv);
    return 1;
}


//Retorna a raiz da arvore
noRB *retornaRaiz(rb *arv){
    return arv->sentinela->dir;
}
//Executa o balanceamento da arvore pós inserção
void balanceamentoInsercao(rb *arv, noRB *novoNo){
    noRB *tio, *avo;

    while(novoNo->pai && novoNo->pai->cor == 'V'){
        avo = novoNo->pai->pai;

        if(novoNo->pai == avo->esq){
            tio = avo->dir;

            if(tio && tio->cor == 'V'){


                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            } else {
                if(novoNo == novoNo->pai->dir){
                    novoNo = novoNo->pai;
                    rotacaoEsquerda(arv, novoNo);
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoDireita(arv, avo);
            }
        } else {
            tio = avo->esq;

            if(tio && tio->cor == 'V'){

                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            } else {
                if(novoNo == novoNo->pai->esq){
                    novoNo = novoNo->pai;
                    rotacaoDireita(arv, novoNo);
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoEsquerda(arv, avo);
            }
        }
    }

    arv->sentinela->dir->cor = 'P';
}

//Executa o balanceamento após a remoção de um nó
void balanceamentoRemocao(rb *arv, noRB *no, noRB *pai){
    noRB *irmao;

    while(no != arv->sentinela->dir && (!no || no->cor == 'P')){
        if(no == pai->esq){
            irmao = pai->dir;

            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                pai->cor = 'V';
                rotacaoEsquerda(arv, pai);
                irmao = pai->dir;
            }

            if((!irmao->esq || irmao->esq->cor == 'P') &&
               (!irmao->dir || irmao->dir->cor == 'P')){
                irmao->cor = 'V';

                no = pai;
                pai = no->pai;
            } else {
                if(!irmao->dir || irmao->dir->cor == 'P'){
                    if(irmao->esq)
                        irmao->esq->cor = 'P';

                    irmao->cor = 'V';
                    rotacaoDireita(arv, irmao);
                    irmao = pai->dir;
                }

                irmao->cor = pai->cor;
                pai->cor = 'P';
                if(irmao->dir)
                    irmao->dir->cor = 'P';

                rotacaoEsquerda(arv, pai);
                no = arv->sentinela->dir;
                break;
            }
        } else {
            irmao = pai->esq;

            if(irmao && irmao->cor == 'V'){
                irmao->cor = 'P';
                pai->cor = 'V';
                rotacaoDireita(arv, pai);
                irmao = pai->esq;
            }

            if((!irmao->esq || irmao->esq->cor == 'P') &&
               (!irmao->dir || irmao->dir->cor == 'P')){
                irmao->cor = 'V';

                no = pai;
                pai = no->pai;
            } else {
                if(!irmao->esq || irmao->esq->cor == 'P'){
                    if(irmao->dir)
                        irmao->dir->cor = 'P';

                    irmao->cor = 'V';
                    rotacaoEsquerda(arv, irmao);
                    irmao = pai->esq;
                }

                irmao->cor = pai->cor;
                pai->cor = 'P';
                if(irmao->esq)
                    irmao->esq->cor = 'P';

                rotacaoDireita(arv, pai);
                no = arv->sentinela->dir;
                break;
            }
        }
    }

    if(no)
        no->cor = 'P';
}

//rotaciona a esquerda de acordo com o no desbalanceado
void rotacaoEsquerda(rb *arv, noRB *noDesbalanceado) {


    noRB *pai = noDesbalanceado->pai;
    noRB *filho = noDesbalanceado->dir;
    noRB *neto = filho->esq;

    noDesbalanceado->dir = neto;
    if (neto)
        neto->pai = noDesbalanceado;

    if (noDesbalanceado == pai->esq)
        pai->esq = filho;
    else
        pai->dir = filho;

    filho->pai = pai;
    filho->esq = noDesbalanceado;
    noDesbalanceado->pai = filho;
}

//rotaciona a direita de acordo com o no desbalanceado
void rotacaoDireita(rb *arv, noRB *noDesbalanceado) {

    noRB *pai = noDesbalanceado->pai;
    noRB *filho = noDesbalanceado->esq;
    noRB *neto = filho->dir;

    noDesbalanceado->esq = neto;
    if (neto)
        neto->pai = noDesbalanceado;

    if (noDesbalanceado == pai->esq)
        pai->esq = filho;
    else
        pai->dir = filho;

    filho->pai = pai;
    filho->dir = noDesbalanceado;
    noDesbalanceado->pai = filho;
}

//Atualiza a altura de preto da arvore
int atualiza_Altura_Preto(rb *arv, noRB *noAux){
    if(!noAux)
        return 0;

    int alturaEsq = atualiza_Altura_Preto(arv, noAux->esq);
    int alturaDir = atualiza_Altura_Preto(arv, noAux->dir);

    int ehPreto = 0;
    if(noAux->cor == 'P')
        ehPreto = 1;

    noAux->alturaPreto = alturaEsq + ehPreto;

    return noAux->alturaPreto;
}
//Atualiza a altura na struct da arvore
void atualiza_Altura_Preto_RB(rb *arv){
    arv->alturaPreto = arv->sentinela->dir->alturaPreto;
}

//Calcula a “altura” (nível máximo) da RB
int alturaRB(noRB *no) {
    if (!no) return 0;
    int he = alturaRB(no->esq);
    int hd = alturaRB(no->dir);
    return 1 + (he > hd ? he : hd);
}

// Imprime todos os nós (reais ou nulos) exatamente no nível “nivel”
//  (nível 1 = raiz). Quando chega num NULL, propaga dois NULL abaixo
void imprimeNivelComNull(noRB *no, int nivel) {
    if (nivel == 1) {
        if (no) {
            printf("[%d,%c,%d]", no->chave, no->cor, no->alturaPreto);
        } else {
            printf("[]");
        }
    } else {
        if (!no) {
            // nó nulo gera dois nós nulos no próximo nível
            imprimeNivelComNull(NULL, nivel - 1);
            imprimeNivelComNull(NULL, nivel - 1);
        } else {
            imprimeNivelComNull(no->esq, nivel - 1);
            imprimeNivelComNull(no->dir, nivel - 1);
        }
    }
}

//Percorre do nível 1 até o nível máximo e imprime cada nível, com quebra de linha
void imprimeNosPorNivelRB(rb *arv) {
    noRB *root = retornaRaiz(arv);
    int h = alturaRB(root);
    for (int lvl = 1; lvl <= h; lvl++) {
        imprimeNivelComNull(root, lvl);
        printf("\n");
    }
}