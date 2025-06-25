#include "234_RB.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct RB{
    noRB* sentinela;

    int alturaPreto;
}rb;
typedef struct NO{
    int chave, alturaPreto;
    char cor;

    noRB *esq, *dir, *pai;
} noRB;

rb *alocaArvore(){
    rb* novaArvore = (rb*) malloc(sizeof(rb));

    noRB* sentinela = alocaNo(novaArvore, -100000);

    sentinela->cor = 'P';

    novaArvore->sentinela = sentinela;
    novaArvore->alturaPreto = 0;

    return novaArvore;
}

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

void insereNo(rb *arv, noRB *novoNo, int *contador_rotacoes_rb, int *contador_merges_rb){
    noRB* aux = arv->sentinela->dir;
    noRB* auxPai = arv->sentinela;

    while(aux != NULL){
        auxPai = aux;

        if(novoNo->chave < aux->chave)
            aux = aux->esq;
        else
            aux = aux->dir;
    }

    if(novoNo->chave < auxPai->chave)   auxPai->esq = novoNo;
    else                                auxPai->dir = novoNo;

    novoNo->pai = auxPai;

    balanceamentoInsercao(arv, novoNo, contador_rotacoes_rb, contador_merges_rb);

    atualiza_Altura_Preto(arv, arv->sentinela->dir);
    atualiza_Altura_Preto_RB(arv);
}


int removeNo(rb *arv, int valor, int *contador_rotacoes_rb, int *contador_merges_rb){
    noRB *noRemover = arv->sentinela->dir;

    while(noRemover && noRemover->chave != valor){
        if(valor < noRemover->chave)
            noRemover = noRemover->esq;
        else
            noRemover = noRemover->dir;
    }

    if(noRemover == NULL) return 0; // Elemento não encontrado

    noRB *y = noRemover;
    char corOriginal = y->cor;
    noRB *x, *xPai;

    if(noRemover->esq == NULL){
        x = noRemover->dir;
        xPai = noRemover->pai;

        if(noRemover == noRemover->pai->esq)
            noRemover->pai->esq = x;
        else
            noRemover->pai->dir = x;

        if(x)
            x->pai = noRemover->pai;

    } else if(noRemover->dir == NULL){
        x = noRemover->esq;
        xPai = noRemover->pai;

        if(noRemover == noRemover->pai->esq)
            noRemover->pai->esq = x;
        else
            noRemover->pai->dir = x;

        if(x)
            x->pai = noRemover->pai;

    } else {
        y = noRemover->dir;
        while(y->esq)
            y = y->esq;

        corOriginal = y->cor;
        x = y->dir;
        xPai = y->pai;

        if(y->pai != noRemover){
            if(x)
                x->pai = y->pai;
            y->pai->esq = x;

            y->dir = noRemover->dir;
            if(y->dir)
                y->dir->pai = y;
        }

        if(noRemover->pai->esq == noRemover)
            noRemover->pai->esq = y;
        else
            noRemover->pai->dir = y;

        y->pai = noRemover->pai;
        y->esq = noRemover->esq;
        if(y->esq)
            y->esq->pai = y;

        y->cor = noRemover->cor;
    }

    if(corOriginal == 'P')
        balanceamentoRemocao(arv, x, xPai, contador_rotacoes_rb, contador_merges_rb);

    free(noRemover);
    atualiza_Altura_Preto_RB(arv);
    return 1;
}

void percorrePreOrdem(rb *arv, noRB *aux){
    if(!aux)
        return;
    
    printf("%d -- %c -- %d\n", aux->chave, aux->cor, aux->alturaPreto);
    percorrePreOrdem(arv, aux->esq);
    percorrePreOrdem(arv, aux->dir);
}

noRB *retornaRaiz(rb *arv){
    return arv->sentinela->dir;
}

void balanceamentoInsercao(rb *arv, noRB *novoNo, int *contador_rotacoes_rb, int *contador_merges_rb){
    noRB *tio, *avo;

    while(novoNo->pai && novoNo->pai->cor == 'V'){
        avo = novoNo->pai->pai;

        if(novoNo->pai == avo->esq){
            tio = avo->dir;

            if(tio && tio->cor == 'V'){
                (*contador_merges_rb)++;  // merge (fusão de 3 filhos)

                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            } else {
                if(novoNo == novoNo->pai->dir){
                    novoNo = novoNo->pai;
                    rotacaoEsquerda(arv, novoNo, contador_rotacoes_rb);
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoDireita(arv, avo, contador_rotacoes_rb);
            }
        } else {
            tio = avo->esq;

            if(tio && tio->cor == 'V'){
                (*contador_merges_rb)++;  // merge (fusão de 3 filhos)

                novoNo->pai->cor = 'P';
                tio->cor = 'P';
                avo->cor = 'V';

                novoNo = avo;
            } else {
                if(novoNo == novoNo->pai->esq){
                    novoNo = novoNo->pai;
                    rotacaoDireita(arv, novoNo, contador_rotacoes_rb);
                }

                novoNo->pai->cor = 'P';
                avo->cor = 'V';

                rotacaoEsquerda(arv, avo, contador_rotacoes_rb);
            }
        }
    }

    arv->sentinela->dir->cor = 'P';
}


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

void atualiza_Altura_Preto_RB(rb *arv){
    arv->alturaPreto = arv->sentinela->dir->alturaPreto;
}




// ===== ESTRUTURAS INTERNAS =====

typedef struct NO234 {
    int nChaves;
    int chaves[3];
    no234* filhos[4];
    int folha;
}no234;

typedef struct arvore234 {
    no234* raiz;
    int altura;
    int splits;
    int rotacoes;
    int comparacoes;
    int merges;
}arv234;



// ===== OPERAÇÕES 2-3-4 =====

// Cria uma nova árvore 2-3-4
arv234* criaArvore234() {
    arv234* arv = (arv234*) malloc(sizeof(arv234));
    arv->raiz = NULL;
    arv->altura = 0;
    arv->splits = 0;
    arv->merges=0;
    arv->rotacoes=0;
    return arv;
}

// Cria um novo nó 2-3-4
no234* criaNo234(int folha) {
    no234* novo = (no234*) malloc(sizeof(no234));
    novo->nChaves = 0;
    for (int i = 0; i < 3; i++) novo->chaves[i] = 0;
    for (int i = 0; i < 4; i++) novo->filhos[i] = NULL;
    novo->folha = folha;
    return novo;
}

// Libera a árvore 2-3-4 recursivamente
void libera234(no234* raiz) {
    if (raiz == NULL) return;
    for (int i = 0; i <= raiz->nChaves; i++) {
        libera234(raiz->filhos[i]);
    }
    free(raiz);
}

// Impressão em ordem (debug)
void emOrdem234(no234* raiz) {
    if (raiz == NULL) return;
    for (int i = 0; i < raiz->nChaves; i++) {
        emOrdem234(raiz->filhos[i]);
        printf("%d ", raiz->chaves[i]);
    }
    emOrdem234(raiz->filhos[raiz->nChaves]);
}

no234* splitFilho(no234* pai, int i, arv234* arv) {
    no234* filhoCheio = pai->filhos[i];
    no234* novoNo = criaNo234(filhoCheio->folha);

    novoNo->nChaves = 1;
    novoNo->chaves[0] = filhoCheio->chaves[2];
    if (!filhoCheio->folha) {
        novoNo->filhos[0] = filhoCheio->filhos[2];
        novoNo->filhos[1] = filhoCheio->filhos[3];
    }

    filhoCheio->nChaves = 1;

    for (int j = pai->nChaves; j > i; j--) {
        pai->chaves[j] = pai->chaves[j - 1];
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->chaves[i] = filhoCheio->chaves[1];
    pai->filhos[i + 1] = novoNo;
    pai->nChaves++;

    arv->splits++;
    arv->rotacoes++;
    arv->merges++;

    return pai;
}

void insereNaoCheio(no234* no, int chave, arv234* arv) {
    int i = no->nChaves - 1;

    if (no->folha) {
        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
            arv->comparacoes++;
        }
        no->chaves[i + 1] = chave;
        no->nChaves++;
    } else {
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
            arv->comparacoes++;
        }
        i++;

        if (no->filhos[i]->nChaves == 3) {
            splitFilho(no, i, arv);
            if (chave > no->chaves[i]) i++;
        }

        insereNaoCheio(no->filhos[i], chave, arv);
    }
}

no234* insere234(arv234* arv, int chave) {
    if (!arv->raiz) {
        arv->raiz = criaNo234(1);
        arv->raiz->chaves[0] = chave;
        arv->raiz->nChaves = 1;
        arv->altura = 1;
        return arv->raiz;
    }

    if (arv->raiz->nChaves == 3) {
        no234* novaRaiz = criaNo234(0);
        novaRaiz->filhos[0] = arv->raiz;
        splitFilho(novaRaiz, 0, arv);
        insereNaoCheio(novaRaiz, chave, arv);
        arv->raiz = novaRaiz;
        arv->altura++;
    } else {
        insereNaoCheio(arv->raiz, chave, arv);
    }

    return arv->raiz;
}





// Conversão recursiva direta de um nó 2-3-4 para Rubro-Negra
noRB* converteDireto(no234* no, rb* arv, noRB* pai) {
    if (!no) return NULL;

    if (no->nChaves == 1) {
        noRB* r = alocaNo(arv, no->chaves[0]);
        r->cor = 'P';
        r->pai = pai;
        r->esq = converteDireto(no->filhos[0], arv, r);
        r->dir = converteDireto(no->filhos[1], arv, r);
        return r;
    }

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
// Função que remove nós da arvore 234
void remove234(arv234 *arv, no234 *no, int chave) {
    int idx = 0;

    // Localiza o índice da chave ou do filho onde ela deveria estar
    while (idx < no->nChaves && chave > no->chaves[idx])
        idx++;

    // CASO 1: A chave está neste nó
    if (idx < no->nChaves && no->chaves[idx] == chave) {
        if (no->folha) {
            // É uma folha: apenas remover
            for (int i = idx + 1; i < no->nChaves; i++)
                no->chaves[i - 1] = no->chaves[i];
            no->nChaves--;
        } else {
            // É nó interno → delega para removeDeInterno
            removeDeInterno(no, idx, arv);
        }
    } else {
        // CASO 2: A chave está em um nó descendente
        if (no->folha) {
            // Chave não encontrada
            return;
        }

        int descendente = idx;

        no234 *filho = no->filhos[descendente];

        // Se o filho tiver apenas 1 chave, precisamos garantir que ele terá ao menos 2
        if (filho->nChaves == 1) {
            no234 *irmaoEsq = (descendente > 0) ? no->filhos[descendente - 1] : NULL;
            no234 *irmaoDir = (descendente < no->nChaves) ? no->filhos[descendente + 1] : NULL;

            // Tenta redistribuir com irmão esquerdo
            if (irmaoEsq && irmaoEsq->nChaves >= 2) {
                // Move chave do pai para o filho
                for (int i = filho->nChaves; i > 0; i--)
                    filho->chaves[i] = filho->chaves[i - 1];
                if (!filho->folha)
                    for (int i = filho->nChaves + 1; i > 0; i--)
                        filho->filhos[i] = filho->filhos[i - 1];

                filho->chaves[0] = no->chaves[descendente - 1];
                if (!filho->folha)
                    filho->filhos[0] = irmaoEsq->filhos[irmaoEsq->nChaves];

                no->chaves[descendente - 1] = irmaoEsq->chaves[irmaoEsq->nChaves - 1];
                irmaoEsq->nChaves--;
                filho->nChaves++;
            }
            // Tenta redistribuir com irmão direito
            else if (irmaoDir && irmaoDir->nChaves >= 2) {
                filho->chaves[filho->nChaves] = no->chaves[descendente];
                if (!filho->folha)
                    filho->filhos[filho->nChaves + 1] = irmaoDir->filhos[0];

                no->chaves[descendente] = irmaoDir->chaves[0];

                for (int i = 1; i < irmaoDir->nChaves; i++)
                    irmaoDir->chaves[i - 1] = irmaoDir->chaves[i];
                if (!irmaoDir->folha)
                    for (int i = 1; i <= irmaoDir->nChaves; i++)
                        irmaoDir->filhos[i - 1] = irmaoDir->filhos[i];

                irmaoDir->nChaves--;
                filho->nChaves++;
            }
            // Caso nenhum irmão possa ajudar → funde
            else {
                if (irmaoEsq) {
                    // funde filho + chave do pai + irmão esquerdo
                    for (int i = 0; i < filho->nChaves; i++)
                        irmaoEsq->chaves[irmaoEsq->nChaves + 1 + i] = filho->chaves[i];
                    irmaoEsq->chaves[irmaoEsq->nChaves] = no->chaves[descendente - 1];

                    if (!filho->folha) {
                        for (int i = 0; i <= filho->nChaves; i++)
                            irmaoEsq->filhos[irmaoEsq->nChaves + 1 + i] = filho->filhos[i];
                    }

                    irmaoEsq->nChaves += 1 + filho->nChaves;

                    for (int i = descendente; i < no->nChaves; i++)
                        no->chaves[i - 1] = no->chaves[i];
                    for (int i = descendente + 1; i <= no->nChaves; i++)
                        no->filhos[i - 1] = no->filhos[i];

                    no->nChaves--;
                    free(filho);
                    filho = irmaoEsq;
                }
                else if (irmaoDir) {
                    // funde filho + chave do pai + irmão direito
                    filho->chaves[filho->nChaves] = no->chaves[descendente];
                    for (int i = 0; i < irmaoDir->nChaves; i++)
                        filho->chaves[filho->nChaves + 1 + i] = irmaoDir->chaves[i];

                    if (!filho->folha) {
                        for (int i = 0; i <= irmaoDir->nChaves; i++)
                            filho->filhos[filho->nChaves + 1 + i] = irmaoDir->filhos[i];
                    }

                    filho->nChaves += 1 + irmaoDir->nChaves;

                    for (int i = descendente + 1; i < no->nChaves; i++)
                        no->chaves[i - 1] = no->chaves[i];
                    for (int i = descendente + 2; i <= no->nChaves; i++)
                        no->filhos[i - 1] = no->filhos[i];

                    no->nChaves--;
                    free(irmaoDir);
                }

                arv->merges++;
            }
        }

        // Descendo para o filho (agora seguro)
        remove234(arv, filho, chave);
    }
}

// Encontra o índice da chave no nó ou onde ela deveria estar
int encontrarIndice(no234* no, int chave) {
    int i = 0;
    while (i < no->nChaves && chave > no->chaves[i]) {
        i++;
    }
    return i;
}


void removeDeInterno(no234 *no, int idx, arv234 *arv) {
    int chave = no->chaves[idx];

    no234 *esq = no->filhos[idx];
    no234 *dir = no->filhos[idx + 1];

    if (esq->nChaves >= 2) {
        // Encontrar o antecessor
        no234 *atual = esq;
        while (!atual->folha)
            atual = atual->filhos[atual->nChaves];

        int antecessor = atual->chaves[atual->nChaves - 1];
        no->chaves[idx] = antecessor;
        remove234(arv, esq, antecessor);  // Remove do filho esquerdo
    }
    else if (dir->nChaves >= 2) {
        // Encontrar o sucessor
        no234 *atual = dir;
        while (!atual->folha)
            atual = atual->filhos[0];

        int sucessor = atual->chaves[0];
        no->chaves[idx] = sucessor;
        remove234(arv, dir, sucessor);  // Remove do filho direito
    }
    else {
        // Ambos têm apenas 1 chave → fusão antes de descer
        for (int i = 1; i < dir->nChaves + 1; i++)
            esq->chaves[esq->nChaves + i] = dir->chaves[i - 1];

        esq->chaves[esq->nChaves] = chave;
        esq->nChaves += 1 + dir->nChaves;

        if (!esq->folha) {
            for (int i = 0; i <= dir->nChaves; i++) {
                esq->filhos[esq->nChaves - dir->nChaves + i] = dir->filhos[i];
            }
        }

        // Remove ponteiro do filho direito e chave do nó pai
        for (int i = idx + 1; i < no->nChaves; i++)
            no->chaves[i - 1] = no->chaves[i];
        for (int i = idx + 2; i <= no->nChaves; i++)
            no->filhos[i - 1] = no->filhos[i];

        no->nChaves--;

        free(dir);

        remove234(arv, esq, chave);
        arv->merges++;
    }
}

void preencher(no234* no, int idx, arv234* arv) {
    if (idx != 0 && no->filhos[idx - 1]->nChaves >= 2)
        emprestaDoAnterior(no, idx, arv);
    else if (idx != no->nChaves && no->filhos[idx + 1]->nChaves >= 2)
        emprestaDoProximo(no, idx, arv);
    else {
        if (idx != no->nChaves)
            funde(no, idx, arv);
        else
            funde(no, idx - 1, arv);
    }
}

void emprestaDoAnterior(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx - 1];

    for (int i = filho->nChaves - 1; i >= 0; --i)
        filho->chaves[i + 1] = filho->chaves[i];
    if (!filho->folha)
        for (int i = filho->nChaves; i >= 0; --i)
            filho->filhos[i + 1] = filho->filhos[i];

    filho->chaves[0] = no->chaves[idx - 1];
    if (!filho->folha)
        filho->filhos[0] = irmao->filhos[irmao->nChaves];

    no->chaves[idx - 1] = irmao->chaves[irmao->nChaves - 1];

    filho->nChaves += 1;
    irmao->nChaves -= 1;

    arv->rotacoes++;
}

void emprestaDoProximo(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx + 1];

    filho->chaves[filho->nChaves] = no->chaves[idx];
    if (!filho->folha)
        filho->filhos[filho->nChaves + 1] = irmao->filhos[0];

    no->chaves[idx] = irmao->chaves[0];
    for (int i = 1; i < irmao->nChaves; ++i)
        irmao->chaves[i - 1] = irmao->chaves[i];
    if (!irmao->folha)
        for (int i = 1; i <= irmao->nChaves; ++i)
            irmao->filhos[i - 1] = irmao->filhos[i];

    filho->nChaves += 1;
    irmao->nChaves -= 1;

    arv->rotacoes++;
}

void funde(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx + 1];

    filho->chaves[1] = no->chaves[idx];
    filho->chaves[2] = irmao->chaves[0];

    if (!filho->folha) {
        filho->filhos[2] = irmao->filhos[0];
        filho->filhos[3] = irmao->filhos[1];
    }

    for (int i = idx + 1; i < no->nChaves; ++i) {
        no->chaves[i - 1] = no->chaves[i];
        no->filhos[i] = no->filhos[i + 1];
    }

    filho->nChaves = 3;
    no->nChaves--;

    free(irmao);
    arv->merges++;
}

void remove234(no234 *raiz, int chave, arv234 *arv) {
    if (!raiz) return;

    int idx = encontrarIndice(raiz, chave);

    if (idx < raiz->nChaves && raiz->chaves[idx] == chave) {
        if (raiz->folha)
            removeDeFolha(raiz, idx);
        else
            removeDeInterno(raiz, idx, arv);
    } else {
        if (raiz->folha) {
            // Chave não encontrada
            return;
        }

        int flag = (idx == raiz->nChaves);

        if (raiz->filhos[idx]->nChaves < 2)
            preencher(raiz, idx, arv);

        if (flag && idx > raiz->nChaves)
            remove234(raiz->filhos[idx - 1], chave, arv);
        else
            remove234(raiz->filhos[idx], chave, arv);
    }

    // Verifica se a raiz ficou sem chaves após a remoção
    if (arv->raiz->nChaves == 0) {
        no234 *antigaRaiz = arv->raiz;

        if (antigaRaiz->folha) {
            // A árvore ficou vazia
            arv->raiz = NULL;
        } else {
            // Promove o único filho para ser a nova raiz
            arv->raiz = antigaRaiz->filhos[0];
        }

        free(antigaRaiz);
    }
}


int contaBlocosOcupados(no234* raiz) {
    if (raiz == NULL)
        return 0;

    int total = 1; // Conta o nó atual
    for (int i = 0; i <= raiz->nChaves; i++) {
        total += contaBlocosOcupados(raiz->filhos[i]);
    }
    return total;
}
