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

Noh *novoNoh(int chave){ // Função para criar um novo nó.
    Noh *novo = malloc(sizeof(Noh));

    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

Noh *buscaRecursiva(Noh *raiz, int chaveDesejada, int digito){
    if(raiz == NULL){
        return NULL;
    }

    if(raiz->esq == NULL && raiz->dir == NULL){ // Se é folha.
        if(raiz->chave == chaveDesejada){
            return raiz;
        }
        return NULL;
    }

    if(pegaDigito(chaveDesejada, digito) == 0){
        return buscaRecursiva(raiz->esq, chaveDesejada, digito + 1);
    }else{
        return buscaRecursiva(raiz->dir, chaveDesejada, digito + 1);
    }
}

Noh *ramifique(Noh *p, Noh *q, int digito){
    Noh *intermediario = malloc(sizeof(Noh));
    intermediario->chave = -1; // Afinal será o nó que irá ajudar o P e Q chegarem a folha.

    if(pegaDigito(p->chave, digito) == pegaDigito(q->chave, digito)){ // Se os digitos são iguais, é que ainda precisa ramificar mais.
        if(pegaDigito(p->chave, digito) == 0){ // Verificamos para ver para qual direção iremos ramificar.
            intermediario->dir = NULL;
            intermediario->esq = ramifique(p, q, digito + 1);
        }else{
            intermediario->esq = NULL;
            intermediario->dir = ramifique(p, q, digito + 1);
        }
    }else{ // Caso as chaves se diferenciaram. Agora só preciso saber: quem vai ficar a esquerda e a direita?
        if(pegaDigito(p->chave, digito) == 0){
            intermediario->esq = p;
            intermediario->dir = q;
        }else{
            intermediario->dir = p;
            intermediario->esq = q;
        }
    }

    return intermediario;
    
}

Noh *insereRecursivo(Noh *raiz, Noh *novo, int digito){ // Bem semelhante à Árvore Digital padrão.
    if(raiz == NULL){
        return novo;
    }

    if(raiz->esq == NULL && raiz->dir == NULL){ // Vamos supor que caimos em uma raiz durante a procura. Teremos que criar os novos nó através do ramifique.
        return ramifique(raiz, novo, digito); // Note RAIZ e NOVO serão os inseridos na folha na ramificação.
    }

    if(pegaDigito(novo->chave, digito) == 0){
        raiz->esq = insereRecursivo(raiz->esq, novo, digito + 1);
    }else{
        raiz->dir = insereRecursivo(raiz->dir, novo, digito + 1);
    }

    return raiz; 
}

Noh *inserir(Noh *raiz, int chave){
    Noh *valor = buscaRecursiva(raiz, chave, 0);

    if(valor != NULL){ /* Verifica pra ver se a chave já existe. */
        printf("Ja existe esta chave.\n");
        return NULL;
    }
    
    Noh *novo = novoNoh(chave);
    return insereRecursivo(raiz, novo, 0);
}





Noh *removerRecursivo(Noh *raiz, int chave, int digito, int *removido) {
    if (raiz == NULL) return NULL;

    // Chegou em uma folha
    if (raiz->esq == NULL && raiz->dir == NULL) {
        if (raiz->chave == chave) {
            free(raiz);
            *removido = 1;
            return NULL;
        }
        return raiz;
    }

    // Desce pela trie
    if (pegaDigito(chave, digito) == 0) {
        raiz->esq = removerRecursivo(raiz->esq, chave, digito + 1, removido);
    } else {
        raiz->dir = removerRecursivo(raiz->dir, chave, digito + 1, removido);
    }

    // Agora faz a limpeza em cadeia
    if (raiz->esq == NULL && raiz->dir == NULL && raiz->chave == -1) { // Depois de encontrar vem direto pra cá e vai retornando e verificando.
        free(raiz);
        return NULL;
    }

    return raiz;
}


// Função para imprimir a Trie (pre-order)
void imprimeTrie(Noh *raiz, int nivel) {
    if (!raiz) return;

    for (int i = 0; i < nivel; i++) printf("  "); // identação
    if (raiz->chave == -1)
        printf("*\n"); // nó intermediário
    else
        printf("%d\n", raiz->chave); // folha

    imprimeTrie(raiz->esq, nivel + 1);
    imprimeTrie(raiz->dir, nivel + 1);
}

// Função para liberar toda a Trie
void limpaTrie(Noh *raiz) {
    if (!raiz) return;
    limpaTrie(raiz->esq);
    limpaTrie(raiz->dir);
    free(raiz);
}


int main() {
    Noh *raiz = NULL;

    // Inserções de folhas que vão compartilhar alguns nós intermediários
    raiz = insereRecursivo(raiz, novoNoh(10), 0); // 10 -> 001010
    raiz = insereRecursivo(raiz, novoNoh(12), 0); // 12 -> 001100
    raiz = insereRecursivo(raiz, novoNoh(14), 0); // 14 -> 001110

    printf("Trie atual:\n");
    imprimeTrie(raiz, 0);

    // Remover a folha 12, que deixará um nó intermediário sem filhos
    int removido = 0;
    raiz = removerRecursivo(raiz, 12, 0, &removido);

    printf("\nTrie apos remover 12:\n");
    imprimeTrie(raiz, 0);

    // Remover a folha 10, possivelmente limpando outro nó intermediário
    removido = 0;
    raiz = removerRecursivo(raiz, 10, 0, &removido);

    printf("\nTrie apos remover 10:\n");
    imprimeTrie(raiz, 0);

    // Remover a última folha 14, que deve limpar toda a Trie
    removido = 0;
    raiz = removerRecursivo(raiz, 14, 0, &removido);

    printf("\nTrie apos remover 14:\n");
    imprimeTrie(raiz, 0);

    // Limpeza final
    limpaTrie(raiz);

    return 0;
}
