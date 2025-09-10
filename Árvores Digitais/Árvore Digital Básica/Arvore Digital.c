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
    struct no *esq, *dir;
} Noh;

int pegaDigito(int chave, int digito) {

    int pos_real = (bitsPalavra - 1) - digito; /* Queremos pegar o bit mais da esquerda. Logo, fazemos exatamente essa conta pois os computadores leem da direita para esquerda. */


    return (chave >> pos_real) & 1; /* Vai com esse valor uma certa quantidade de vez para direita. Por fim, compara esse bit com 1. */
}

Noh *busca(Noh *raiz, int chaveDesejada, int digito){ //Essencialmente, sequer precisariamos de nó pai. Só vai ser útil para a parte de remoção.
    if(raiz == NULL){
        return raiz;
    }

    if(raiz->chave == chaveDesejada){ /* ????? */
        return raiz;
    }

    if(pegaDigito(chaveDesejada, digito) == 0){
        return busca(raiz->esq, chaveDesejada, digito + 1);
    }else{
        return busca(raiz->dir, chaveDesejada, digito + 1);
    }
    

}

Noh *novoNoh(int chave){ // Função para criar um novo nó.
    Noh *novo = malloc(sizeof(Noh));

    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

Noh *insereRecursivo(Noh *raiz, Noh *novo, int digito){
    if(raiz == NULL){
        return novo;
    }

    if(pegaDigito(novo->chave, digito) == 0){
        raiz->esq = insereRecursivo(raiz->esq, novo, digito + 1);
    }else{
        raiz->dir = insereRecursivo(raiz->dir, novo, digito + 1);
    }

    return raiz; 

}

Noh *inserir(Noh *raiz, int chave){
    Noh *valor = busca(raiz, chave, 0);

    if(valor != NULL){ /* Verifica pra ver se a chave já existe. */
        printf("Ja existe esta chave.\n");
        return NULL;
    }
    
    Noh *novo = novoNoh(chave);
    return insereRecursivo(raiz, novo, 0);
}

Noh *removeRaiz(Noh *alvo){
    Noh *aux = NULL, *pai = NULL;

    if(alvo->esq == NULL && alvo->dir == NULL){ // Caso seja folha.
        free(alvo);
        return NULL;
    }

    aux = alvo;
    while(aux->esq != NULL || aux->dir != NULL){ // Euuanto não ser folha.
        pai = aux;

        if(aux->dir != NULL){ // Prioridade pela direita (sem motivo): se tiver dois filho irá descer na direita.
            aux = aux->dir;
        }else{
            aux = aux->esq;
        }
    }

    // Quando saimos do WHILE, sabemos: aux é uma folha e pai é o pai de aux.

    alvo->chave = aux->chave; // Trocamos as posições.

    if(pai->esq == aux){ // Removemos a folha. Aqui poderia ser um free() também.
        pai->esq = removeRaiz(aux);
    }else{
        pai->dir = removeRaiz(aux);
    }

    return alvo;
}

Noh *remover(Noh *raiz, int chave){
    Noh *alvo, *aux;

    alvo = busca(raiz, chave, 0); // Vamos buscar a chave.

    if(alvo == NULL){ // Nao achamos o valor.
        printf("No momento da remocao, nao encontramos %d.\n", chave);
        return raiz;
    }

    aux = removeRaiz(alvo); // Vai retornar o alvo já com outro valor. Aquele do descendente.

    if(alvo == raiz){ // Caso tenhamos removido a própria raiz.
        return aux; // Retornamos a raiz com o novo valor.
    }

    return raiz;

}

void limparArvore(Noh *raiz) {
    if (raiz != NULL) {
        limparArvore(raiz->esq);
        limparArvore(raiz->dir);
        free(raiz);
    }
}

void imprimir(Noh *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        imprimir(raiz->esq);
        imprimir(raiz->dir);
    }
}


int main() {
    setlocale(LC_ALL, "Portuguese");

    Noh *raiz = NULL;

    // Insercoes
    raiz = inserir(raiz, 25); // 25 = 011001
    raiz = inserir(raiz, 15); // 15 = 001111
    raiz = inserir(raiz, 37); // 37 = 100101
    raiz = inserir(raiz, 5);  // 5  = 000101

    printf("Insercoes concluidas!\n");

    printf("\n");
    imprimir(raiz);
    printf("\n\n");

    // Buscas
    int chave = 15;
    Noh *res = busca(raiz, chave, 0);
    if (res != NULL) {
        printf("Chave %d encontrada na arvore.\n", chave);
    } else {
        printf("Chave %d NAO encontrada.\n", chave);
    }

    chave = 30;
    res = busca(raiz, chave, 0);
    if (res != NULL) {
        printf("Chave %d encontrada na arvore.\n", chave);
    } else {
        printf("Chave %d NAO encontrada.\n", chave);
    }

    // Remocao
    raiz = remover(raiz, 15);
    printf("Removida a chave 15.\n");

    res = busca(raiz, 15, 0);
    if (res != NULL) {
        printf("Chave 15 ainda esta na arvore (erro na remocao).\n");
    } else {
        printf("Chave 15 removida com sucesso.\n");
    }

    printf("\n\n");
    imprimir(raiz);
    limparArvore(raiz);
    printf("\n\n");

    return 0;
}
