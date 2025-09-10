#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*

*/

#define bitsPalavra 6

typedef struct no{
    int chave;
    int digito; // Única mudança em relação a estrutura de outras árvores. É o digito que vamos olhar.
    struct no *esq, *dir;
} Noh;

int pegaDigito(int chave, int digito) {

    int pos_real = (bitsPalavra - 1) - digito; /* Queremos pegar o bit mais da esquerda. Logo, fazemos exatamente essa conta pois os computadores leem da direita para esquerda. */


    return (chave >> pos_real) & 1; /* Vai com esse valor uma certa quantidade de vez para direita. Por fim, compara esse bit com 1. */
}


Noh *arvoreInicializa(){
    Noh *raiz = malloc(sizeof(Noh)); // Nó DUMMY.

    raiz->chave = __UINT32_MAX__; // 111111.
    raiz->esq = raiz;
    raiz->dir = raiz;
    raiz->digito = -1;

    return raiz;
}

Noh *buscaRecursiva(Noh *raiz, int chave, int digito_anterior){
    if(raiz->digito <= digito_anterior){ // É folha.
        return raiz;
    }

    if(pegaDigito(chave, raiz->digito) == 0){
        return buscaRecursiva(raiz->esq, chave, raiz->digito); // CORREÇÂO: MUDEI DE raiz->chave para CHAVE. ????????
    }else{
        return buscaRecursiva(raiz->dir, chave, raiz->digito);
    }
}

Noh *busca(Noh *raiz, int chave){
    Noh *alvo = buscaRecursiva(raiz->esq, chave, -1); // Raiz a esquerda porque começa no dó dummy. -1 pois é o digito do nó dummy.
    
    if(alvo->chave == chave){
        return alvo;
    }else{
        return NULL;
    }
}

Noh *novoNoh(int chave, int digito){
    Noh *novo = malloc(sizeof(Noh));

    novo->chave = chave;
    novo->digito = digito;

    novo->dir = NULL;
    novo->esq = NULL;

    return novo;
}

Noh *insereRecursivo(Noh *raiz, Noh *novo, int digitoOndeDifere, Noh *pai){
    if((raiz->digito >= digitoOndeDifere) || (raiz->digito <= pai->digito)){ 
        // Parte esquerda: passamos do ponto onde deveriamos inserir.
        // Parte direita: quando subiu na árvore ou apontou para si mesmo.. Logo, quando é folha.

        if(pegaDigito(novo->chave, digitoOndeDifere) == 1){ // Se o bit onde difere for 1, colocaremos na esquerda o outro. Se não, é a direita. Pois o nó tem sempre que se apontar se ele for para seu caminho.
            novo->esq = raiz;
            novo->dir = novo;
        }else{
            novo->esq = novo;
            novo->dir = raiz;
        }

        return novo;
    }

    if(pegaDigito(novo->chave, raiz->digito) == 0){
        raiz->esq = insereRecursivo(raiz->esq, novo, digitoOndeDifere, raiz);
    }else{
        raiz->dir = insereRecursivo(raiz->dir, novo, digitoOndeDifere, raiz);
    }

    return raiz;
}

void inserir(Noh *raiz, int chave){ // Invoca a busca, detectando o primeiro valor distinto entre os digitos. Raiz aqui é realmente a raiz dummy. Por isso o ->esq.
    Noh *aux = buscaRecursiva(raiz->esq, chave, -1); // Se não achar uma chave que já existe, irá apontar para o nó pai da posição que deveriamos inserir.

    if(aux->chave == chave){ // Já existe esse valor = não insere.
        return;
    }

    int i = 0;
    for(i = 0; i < bitsPalavra && pegaDigito(chave, i) == pegaDigito(aux->chave, i); i++); // Vamos achar a posição em que se diferem. CORREÇÂO: antes eu tava passando pegaDigito(raiz->chave, i) na primeira parte.
    // BitsPalavra é para que não passe de seis.

    Noh *novo = novoNoh(chave, i); // i é para o digito que deveriamos olhar.
    raiz->esq = insereRecursivo(raiz->esq, novo, i, raiz);

}

void imprimirRecursivo(Noh *raiz, int nivel, int digito_anterior){
    if(raiz->digito <= digito_anterior){ // Folha
        for(int i=0;i<nivel;i++) printf("   ");
        printf("Chave: %d (folha)\n", raiz->chave);
        return;
    }
    imprimirRecursivo(raiz->esq, nivel+1, raiz->digito);
    for(int i=0;i<nivel;i++) printf("   ");
    printf("Digito: %d (chave %d)\n", raiz->digito, raiz->chave);
    imprimirRecursivo(raiz->dir, nivel+1, raiz->digito);
}

void imprimir(Noh *raiz){
    if(raiz->esq == raiz){
        printf("Árvore vazia!\n");
        return;
    }
    printf("Árvore digital:\n");
    imprimirRecursivo(raiz->esq, 0, -1);
}

void limparRecursivo(Noh *raiz, int digito_anterior){
    if(raiz->digito <= digito_anterior){
        free(raiz);
        return;
    }
    limparRecursivo(raiz->esq, raiz->digito);
    limparRecursivo(raiz->dir, raiz->digito);
    free(raiz);
}

void limpar(Noh *raiz){
    if(raiz->esq != raiz){
        limparRecursivo(raiz->esq, -1);
    }
    free(raiz); // libera o nó dummy
}

/* ---------- MAIN COM EXEMPLOS ---------- */

int main() {
    setlocale(LC_ALL,"");

    Noh *raiz = arvoreInicializa();

    inserir(raiz, 25);
    inserir(raiz, 10);
    inserir(raiz, 50);
    inserir(raiz, 7);
    inserir(raiz, 30);

    imprimir(raiz);

    printf("\nBuscando chave 30...\n");
    Noh *b = busca(raiz, 30);
    if(b) printf("Chave %d encontrada!\n", b->chave);
    else printf("Chave não encontrada.\n");

    printf("\nBuscando chave 99...\n");
    b = busca(raiz, 99);
    if(b) printf("Chave %d encontrada!\n", b->chave);
    else printf("Chave não encontrada.\n");

    limpar(raiz);

    return 0;
}