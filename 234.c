#include "234.h"
#include <stdio.h>
#include <stdlib.h>




// ===== OPERAÇÕES 2-3-4 =====

// Cria uma nova árvore 234
arv234* criaArvore234() {
    arv234* arv = (arv234*) malloc(sizeof(arv234));
    arv->raiz = NULL;
    arv->altura = 0;
    arv->splits = 0;
    arv->merges=0;
    arv->rotacoes=0;
    arv->comparacoes = 0;
    return arv;
}

no234 *retornaRaiz234(arv234 *arv) {
    return arv->raiz;
}

// Cria um novo nó 234
no234* criaNo234(int folha) {
    no234* novo = (no234*) malloc(sizeof(no234));
    novo->nChaves = 0;
    for (int i = 0; i < 3; i++) novo->chaves[i] = 0;
    for (int i = 0; i < 4; i++) novo->filhos[i] = NULL;
    novo->pai = NULL;
    novo->folha = folha;
    return novo;
}

// Libera a árvore 234 recursivamente
void libera234(no234* raiz) {
    if (raiz == NULL) return;
    for (int i = 0; i <= raiz->nChaves; i++) {
        if(raiz->filhos[i] != NULL){
            libera234(raiz->filhos[i]);
        }
    }
    free(raiz);
}

// Encontra o índice da primeira chave ≥ key
int encontrarIndice(no234 *x, int key) {
    int i = 0;
    while (i < x->nChaves && key > x->chaves[i]) i++;
    return i;
}

// Retorna o predecessor “profundo” no filho x->filhos[idx]
int getPredecessor(no234 *x, int idx) {
    no234 *cur = x->filhos[idx];
    while (!cur->folha)
        cur = cur->filhos[cur->nChaves];
    return cur->chaves[cur->nChaves - 1];
}



//Remove uma chave da arvore 234 e realiza as operações necessárias se precisar
int remove234(arv234 *arv, no234 *x, int key) {
    if (!x)
        return 0;

    int idx = encontrarIndice(x, key);
    int removed = 0;

    // A) chave está no nó x?
    if (idx < x->nChaves && x->chaves[idx] == key) {
        if (x->folha) {
            // A1) nó folha: shift simples
            for (int j = idx + 1; j < x->nChaves; j++) {
                x->chaves[j - 1] = x->chaves[j];
            }
            x->nChaves--;
            removed = 1;
        } else {
            // A2) nó interno: sobe predecessor e remove recursivamente
            removed = removeDeInterno(arv, x, idx);
        }
    }
    // B) não achou e é folha → nada a remover
    else if (x->folha) {
        return 0;
    }
    // C) descer para o filho adequado
    else {
        removed = remove234(arv, x->filhos[idx], key);
    }

    // D) corrigir underflow: se o filho ficou vazio, preencher
    if (removed && !x->folha) {
        no234 *filho = x->filhos[idx];
        if (filho->nChaves == 0) {
            preencher(x, idx, arv);
        }
    }

    return removed;
}


// remove a chave de um nó interno x na posição idx
//  1) sobe o predecessor
//  2) remove recursivamente daquele predecessor
//  3) se o filho ficar com 0 chaves, corrige underflow via preencher()

int removeDeInterno(arv234 *arv, no234 *x, int idx) {
    // 1) pega predecessor
    int pred = getPredecessor(x, idx);
    // 2) substitui em x e remove de fato no filho
    x->chaves[idx] = pred;
    int removed = remove234(arv, x->filhos[idx], pred);
    // 3) só agora: se de fato aquele filho ficou vazio, corrige
    if (x->filhos[idx]->nChaves == 0) {
        preencher(x, idx, arv);
    }
    return removed;
}


// Remover em folha e corrigir underflow só em 0 chaves
void removePredecessor(arv234 *arv, no234 *x, int idx) {
    no234 *cur = x->filhos[idx];
    // desce até a folha
    while (!cur->folha) {
        cur = cur->filhos[cur->nChaves];
    }
    no234 *pai = cur->pai;

    // encontra posição em pai->filhos
    int i = 0;
    while (pai->filhos[i] != cur) i++;

    // remove a última chave de cur
    for (int j = i + 1; j < cur->nChaves; j++)
        cur->chaves[j - 1] = cur->chaves[j];
    cur->nChaves--;

    // só preenche quando realmente ficou com 0 chaves
    if (cur->nChaves == 0) {
        preencher(pai, i, arv);
    }
}



//realiza as funções de emprestar dependendo da situação
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

//Rotação a esquerda, emprestando do filho da direita
void emprestaDoAnterior(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx - 1];

    for (int i = filho->nChaves - 1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];
    if (!filho->folha)
        for (int i = filho->nChaves; i >= 0; i--)
            filho->filhos[i + 1] = filho->filhos[i];

    filho->chaves[0] = no->chaves[idx - 1];

    if (!filho->folha)
        filho->filhos[0] = irmao->filhos[irmao->nChaves];

    no->chaves[idx - 1] = irmao->chaves[irmao->nChaves - 1];

    filho->nChaves += 1;
    irmao->nChaves -= 1;

    arv->rotacoes++;
}

//Rotação a esquerda, emprestando do filho da direita
void emprestaDoProximo(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx + 1];

    filho->chaves[filho->nChaves] = no->chaves[idx];

    if (!filho->folha)
        filho->filhos[filho->nChaves + 1] = irmao->filhos[0];

    if(!filho->folha && filho->filhos[filho->nChaves + 1] != NULL)
        filho->filhos[filho->nChaves + 1]->pai = filho;

    no->chaves[idx] = irmao->chaves[0];

    for (int i = 1; i < irmao->nChaves; i++)
        irmao->chaves[i - 1] = irmao->chaves[i];

    if (!irmao->folha)
        for (int i = 1; i <= irmao->nChaves; i++)
            irmao->filhos[i - 1] = irmao->filhos[i];

    filho->nChaves++;
    irmao->nChaves--;

    arv->rotacoes++;
}

//Realiza o Merge se necessário
void funde(no234* no, int idx, arv234* arv) {
    no234* filho = no->filhos[idx];
    no234* irmao = no->filhos[idx + 1];

    filho->chaves[filho->nChaves] = no->chaves[idx];

    for (int i = 0; i < irmao->nChaves; i++) {
        filho->chaves[filho->nChaves + 1 + i] = irmao->chaves[i];
    }

    if(!filho->folha){
        for (int i = 0; i <= irmao->nChaves; i++) {
            filho->filhos[filho->nChaves + 1 + i] = irmao->filhos[i];
            if (filho->filhos[filho->nChaves + 1 + i] != NULL)
                filho->filhos[filho->nChaves + 1 + i]->pai = filho;
        }
    }

    filho->nChaves += 1 + irmao->nChaves;

    for (int i = idx + 1; i < no->nChaves; i++) {
        no->chaves[i - 1] = no->chaves[i];
        no->filhos[i] = no->filhos[i + 1];
    }

    no->nChaves--;

    free(irmao);
    arv->merges++;
}

//Conta o numero de blocos (nós) da arvore 234
int contaBlocosOcupados(no234* raiz) {
    if (raiz == NULL)
        return 0;

    int total = 1; // Conta o nó atual
    for (int i = 0; i <= raiz->nChaves; i++) {
        total += contaBlocosOcupados(raiz->filhos[i]);
    }
    return total;
}


//Função que calcula a altura da 2-3-4
int altura234(no234* no) {
    if (!no) return 0;
    if (no->folha) return 1;
    int h = 0;
    // cada nó interno tem até nChaves+1 filhos
    for (int i = 0; i <= no->nChaves; i++) {
        int ch = altura234(no->filhos[i]);
        if (ch > h) h = ch;
    }
    return 1 + h;
}

// 2) Imprime todos os nós que estão exatamente no nível “nivel”
//    (nível 1 = raiz, nível 2 = filhos da raiz, etc.)
void imprimeNivel234(no234* no, int nivel) {
    if (!no) return;
    if (nivel == 1) {
        // imprime as chaves do nó atual
        printf("[");
        for (int j = 0; j < no->nChaves; j++) {
            printf("%d", no->chaves[j]);
            if (j < no->nChaves - 1) printf(",");
        }
        printf("] ");
    } else {
        // desce para o próximo nível em todos os filhos
        for (int j = 0; j <= no->nChaves; j++) {
            imprimeNivel234(no->filhos[j], nivel - 1);
        }
    }
}

// 3) Percorre do nível 1 até o nível máximo e manda imprimir cada nível
void imprimeNosPorNivel234(arv234* arv) {
    int h = altura234(arv->raiz);
    for (int lvl = 1; lvl <= h; lvl++) {
        imprimeNivel234(arv->raiz, lvl);
        printf("\n");
    }
}


// Divide x->filhos[i] (que tem 3 chaves) em y e z, sobe y->chaves[1] para x->chaves[i].

void splitChild(no234* x, int i, arv234* arv) {
    no234* y = x->filhos[i];
    no234* z = criaNo234(y->folha);
    z->pai = x;

    // z recebe a 3ª chave de y
    z->chaves[0] = y->chaves[2];
    z->nChaves   = 1;

    // se não for folha, transfere também y->filhos[2] e y->filhos[3]
    if (!y->folha) {
        z->filhos[0] = y->filhos[2];
        z->filhos[1] = y->filhos[3];
        if (z->filhos[0]) z->filhos[0]->pai = z;
        if (z->filhos[1]) z->filhos[1]->pai = z;
    }

    // y mantém só a chave[0] e seus filhos[0..1]
    y->nChaves = 1;

    // abre espaço em x para z e a chave média
    for (int j = x->nChaves; j >= i+1; j--)
        x->filhos[j+1] = x->filhos[j];
    x->filhos[i+1] = z;

    for (int j = x->nChaves-1; j >= i; j--)
        x->chaves[j+1] = x->chaves[j];
    // sobe a chave do meio de y
    x->chaves[i] = y->chaves[1];
    x->nChaves++;

    arv->splits++;
}


// Insere k em x, sabendo que x->nChaves < 3. antes de descer, splitChild se o filho estiver cheio.

void insertNonFull(no234* x, int k, arv234* arv) {
    int i = x->nChaves - 1;

    if (x->folha) {
        // insere k em x, mantendo chaves ordenadas
        while (i >= 0 && k < x->chaves[i]) {
            x->chaves[i+1] = x->chaves[i];
            i--;
        }
        x->chaves[i+1] = k;
        x->nChaves++;
    } else {
        // descobre o filho correto
        while (i >= 0 && k < x->chaves[i]) i--;
        i++;
        // se x->filhos[i] estiver cheio, preemptivamente divida
        if (x->filhos[i]->nChaves == 3) {
            splitChild(x, i, arv);
            // após o split, se k for maior que a chave que subiu, desce ao irmão direito
            if (k > x->chaves[i]) i++;
        }
        insertNonFull(x->filhos[i], k, arv);
    }
}


//rotina pública de inserção com split preemptivo, se a raiz estiver cheia, primeiro splitChild na raiz.

no234* insere234(arv234* arv, int k) {
    if (!arv->raiz) {
        // árvore vazia: raiz folha
        arv->raiz = criaNo234(1);
        arv->raiz->chaves[0] = k;
        arv->raiz->nChaves   = 1;
        arv->altura          = 1;
        return arv->raiz;
    }

    no234* r = arv->raiz;
    // se raiz cheia, rode splitChild para cima
    if (r->nChaves == 3) {
        no234* s = criaNo234(0);
        arv->raiz    = s;
        s->filhos[0] = r;
        r->pai       = s;
        splitChild(s, 0, arv);
        arv->altura++;
        insertNonFull(s, k, arv);
    } else {
        insertNonFull(r, k, arv);
    }

    return arv->raiz;
}

// Função que chama as funções de remove da árvore e trata underflow da raiz

int removeKey(arv234 *arv, int key) {
    if (!arv->raiz)
        return 0;

    // 1) faz a remoção recursiva
    int ok = remove234(arv, arv->raiz, key);

    if (!ok)
        return 0;

    // 2) se a raiz ficou sem chaves e não é folha, “encolhe” a árvore
    if (arv->raiz->nChaves == 0 && !arv->raiz->folha) {
        no234 *antiga = arv->raiz;
        arv->raiz = antiga->filhos[0];
        arv->raiz->pai = NULL;
        free(antiga);
        arv->altura--;
    }
    // 3) se chegou a árvore vazia (tudo removido), limpa raiz
    else if (arv->raiz->nChaves == 0 && arv->raiz->folha) {
        free(arv->raiz);
        arv->raiz = NULL;
        arv->altura = 0;
    }

    return 1;
}

void remove_numeros_do_arquivo(arv234 *arv, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de remoção");
        return;
    }

    int valor;
    while (fscanf(arquivo, "%d", &valor) == 1) {
        int ok = removeKey(arv, valor);
        if (ok)
            printf("Removido: %d\n", valor);
        else
            printf("Não encontrado: %d\n", valor);
    }

    fclose(arquivo);
}