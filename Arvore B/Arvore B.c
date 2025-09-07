#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define T 3
#define MAX (2*T - 1)
/*
Qualquer dúvida só olhar o resumo. Mas faremos algumas anotações aqui para lembrar:

    - b = 2t - 1;
    - t = minimo.
*/

typedef struct No {
    int n; /* Quantidade já preenchida. */
    int folha; /* Define se é folha. Se 1, é. Se 0, não. */
    int chave[MAX]; /* Valores dos nós. Poderia ter o +1 para contar o buffer, mas não faremos. */
    struct No *filho[MAX+1]; /* Filho: precisa ter o +1 para englobar todos as possibilidades (menor e maior). */
} NoArvB, *ArvoreB;

NoArvB* criaNo(int folha) { /* Cria um nó. */
    NoArvB* novo = (NoArvB*) malloc(sizeof(NoArvB));
    novo->n = 0;
    novo->folha = folha;
    for (int i = 0; i < MAX + 1; i++) {
        novo->filho[i] = NULL;
    }
    return novo;
}

NoArvB* BuscaArvoreB(NoArvB *r, int k, int *pos) { /* Função de busca. */
    int i = 1;
    

    // Busca sequencial para encontrar a posição ou ultrapassar
    while (i <= r->n && k > r->chave[i - 1]) {
        i++;
    }

    // Se encontrou a chave no nó atual
    if (i <= r->n && k == r->chave[i - 1]) { /* Precisa do i - 1 para pegar a posição real do vetor. */
        if (pos){
            *pos = i - 1;
        }
        return r;
    }

    // Se é folha, não existe
    if (r->folha) {
        return NULL;
    } else {
        // Lê o filho e continua a busca recursivamente
        return BuscaArvoreB(r->filho[i - 1], k, pos);
    }
} /* Daria para usar busca binária aqui. Um exercício do slide pede isso, inclusive. */


void split(NoArvB *pai, int pos, NoArvB *y){ /* Y = nó que será dividido. Pai = nó pai do que será dividido. Pos = indice onde y está como filho de pai [(pai->filho[pos] == y)]. */
    NoArvB *z = criaNo(y->folha); /* z = novo nó. */

    int meio = T - 1; /* Pegar o valor que está na posição mediana. */

    for (int j = meio + 1, k = 0; j < y->n; j++, k++) { /* Vamos copiar os valores da direita da mediana ára o novo nó. */
        z->chave[k] = y->chave[j];
        z->n++;
    }

    if(y->folha != 1){ /* Caso y não seja folha, vamos copiar seus filhos a partir da direita da mediana para o novo nó. */
        for (int j = meio + 1, k = 0; j <= y->n; j++, k++) {
            z->filho[k] = y->filho[j];
        }
    }

    int mediana = y->chave[meio]; /* Valor que será inserido no nó pai. */
    y->n = meio; /* Atualizando a quantidade de elementos do nó com os valores da esquerda. */

    for (int j = pai->n; j >= pos + 1; j--) { /* Vamos começar do fim dos ponteiros dos filhos e ir voltando até achar o ponteiro dianteiro da posição original de y. */
        pai->filho[j + 1] = pai->filho[j]; /* Enquanto fazemos a parte de cima, jogamos os ponteiros do pai para direita, abrindo o espaço para botar o novo Z. */
    }
    pai->filho[pos + 1] = z;

    for (int j = pai->n - 1; j >= pos; j--) { /* Faz a mesma coisa, só que para as chaves. */
        pai->chave[j + 1] = pai->chave[j];
    }
    pai->chave[pos] = mediana;
    pai->n++;

}

void insereRecursivo(NoArvB *x, int k, NoArvB **novaRaiz){
    int i = x->n - 1; /* Pegando último elemento. */

    if(x->folha == 1){
        while(i >= 0 && k < x->chave[i]){
            x->chave[i + 1] = x->chave[i];
            i--;
        }
        x->chave[i + 1] = k; /* i + 1 aqui pois irá parar justamente na anterior ao espaço vazio. */
        x->n++;
    }else{
        while(i >= 0 && k < x->chave[i]){
            i--;
        }

        insereRecursivo(x->filho[++i], k, novaRaiz);

        if(x->filho[i]->n >= MAX){
            split(x, i, x->filho[i]); /* Se o filho está cheio, divida-o. */
        }
    }



}

NoArvB* insereArvoreB(NoArvB *raiz, int k) { /* Fazemos efetivamente a inserção nessa função. A outra é só uma parte. */
    
    // CASO 1: A árvore está completamente vazia.
    if (raiz == NULL) {
        raiz = criaNo(1); // Cria o primeiro nó (que é raiz e folha)
        raiz->chave[0] = k;
        raiz->n = 1;
        return raiz;
    }

    if((BuscaArvoreB(raiz, k, 0)) != NULL){
        printf("\nErro. Já existe esse valor na árvore.\n");
        return raiz;
    }

    // CASO 2: A árvore já existe. Delega a inserção para a função recursiva.
    insereRecursivo(raiz, k, &raiz);

    // CASO 3: A raiz ficou cheia após a inserção? (Ocorrem múltiplas divisões, chegou na raiz e agora a árvore precisa crescer em altura)
    if (raiz->n == MAX) {
        NoArvB *nova = criaNo(0); // 1. Cria uma NOVA raiz (que não é folha)
        nova->filho[0] = raiz;     // 2. A raiz ANTIGA vira filha da nova
        
        // 3. Divide a raiz antiga, promovendo a mediana para a nova raiz
        split(nova, 0, raiz);
        
        raiz = nova; // 4. O ponteiro principal da árvore agora aponta para a nova raiz
    }

    return raiz;
}

void imprimir2(ArvoreB raiz){
    printf("\n\n");

    if(raiz == NULL){
        return;
    }

    for(int i = 0; i < raiz->n; i++){
        printf("%d ", raiz->chave[i]);
    }

    if(raiz->folha != 1){
        for(int i = 0; i <= raiz->n; i++){
            imprimir2(raiz->filho[i]);
        }
    }
}

void imprimirArvore(NoArvB *raiz, int nivel) {
    if (raiz == NULL) return;

    printf("Nivel %d -> ", nivel);
    for (int i = 0; i < raiz->n; i++) {
        printf("%d ", raiz->chave[i]);
    }
    printf("\n");

    if (!raiz->folha) {
        for (int i = 0; i <= raiz->n; i++) {
            imprimirArvore(raiz->filho[i], nivel + 1);
        }
    }
}

void limpar(ArvoreB raiz){
    if(raiz == NULL){
        return;
    }

    if(raiz->folha != 1){
        for(int i = 0; i <= raiz->n; i++){
            limpar(raiz->filho[i]);
        }
    }

    free(raiz);
}

/* A partir de agora, será a função de remoção que faremos. */

void merge(NoArvB *atual, int pos){
    NoArvB *filho  = atual->filho[pos];
    NoArvB *posterior = atual->filho[pos + 1];

    filho->chave[T - 1] = atual->chave[pos]; /* Chave do pai desce ao filho (1) */

    for(int j = 0; j < posterior->n; j++){ /* Copiar para o filho os valores do posterior (2) */
        filho->chave[j + T] = posterior->chave[j];
    }

    if(!filho->folha){ /* Se não for folha, copia os filhos. Obs: todos os filhos estão no mesmo nível. Se um for folha, outros também são. */
        for(int i = 0; i <= posterior->n; i++){
            filho->filho[i + T] = posterior->filho[i];
        }
    }

    for (int i = pos + 1; i < atual->n; i++) {/* Move para esquerda as chaves do pai para tapar o buraco que ficou (3) */
        atual->chave[i - 1] = atual->chave[i];
    }
    for (int i = pos + 2; i <= atual->n; i++) { /* Move os filhos. */
        atual->filho[i - 1] = atual->filho[i];
    }

    atual->n--;

    filho->n += posterior->n + 1;

    free(posterior);
}

void corrigirFilho(NoArvB *atual, int pos){
    
    /* 
    
                                                    NÓ PAI
                                ... | chave[pos-1] |       | chave[pos] | ...
                                    /              \       /            \
                                    /                \     /              \
                            filho[pos-1]          filho[pos]         filho[pos+1]
                            (Irmão Anterior)      (O FILHO)        (Irmão Posterior)

    */

    NoArvB *anterior = NULL;
    NoArvB *posterior = NULL;

    if(pos > 0 ){
        anterior  = atual->filho[pos - 1];
    }
    
    if(pos < atual->n){
        posterior = atual->filho[pos + 1];
    }
    

    NoArvB *filho = atual->filho[pos];
    if (filho == NULL) return; /* segurança */

    if ( anterior != NULL && anterior->n >= T) { /* irmão esquerdo pode emprestar */
        
        for (int j = filho->n - 1; j >= 0; j--) { /* Abrindo espaço na primeira posição. */
            filho->chave[j + 1] = filho->chave[j];
        }
        if (!filho->folha) { /* Vamos deslocar os filhos também. */
            for (int j = filho->n; j >= 0; j--) {
                filho->filho[j + 1] = filho->filho[j];
            }
        }

        filho->chave[0] = atual->chave[pos - 1]; /* Chave do pai desce ao filho. */
        if(!filho->folha){
            filho->filho[0] = anterior->filho[anterior->n]; /* Ver no Gemini a ilustração se tiver dúvida. */
        }

        atual->chave[pos - 1] = anterior->chave[anterior->n - 1]; /* Maior filho da esquerda sobe ao pai. */
    
        filho->n++;
        anterior->n--;
    }
    else if (posterior != NULL && posterior->n >= T) { /* irmão direito pode emprestar */
        
        filho->chave[filho->n] = atual->chave[pos]; /* Filho pega emprestado o valor do pai na última posição. */

        if(!filho->folha){
            filho->filho[filho->n + 1] = posterior->filho[0]; // O primeiro filho do irmão torna-se o último filho do filho
        }

        atual->chave[pos] = posterior->chave[0]; /* A primeira chave do posterior se torna a chave do pai. */

        // Move as chaves e filhos do irmão para a esquerda
        for (int i = 1; i < posterior->n; ++i)
            posterior->chave[i - 1] = posterior->chave[i];
        if (!posterior->folha) {
            for (int i = 1; i <= posterior->n; ++i)
                posterior->filho[i - 1] = posterior->filho[i];
        }

        filho->n++;
        posterior->n--;
    }
    else { /* nenhum pode emprestar -> merge */
        if (pos < atual->n) {
            merge(atual, pos);
        } else {
            merge(atual, pos - 1);
        }
    }
}

ArvoreB *remover(NoArvB *no, int k){
    if(no == NULL){
        printf("Arvore vazia.\n");
        return;
    }

    printf("Removendo %d.\n", k);

    /* Não dá pra usar a função de busca já implementada. Ela retorna apenas o nó desejado, mas não o pai dele que usaremos para remoção. */

    NoArvB *atual = no;
    NoArvB *pai = NULL;

    int i = 1;
    int pos = 0;

    while(atual != NULL){   /* Achamos o pai, o nó e a posição no nó. Pos = posição do nó. */

        i = 1;
        pos = 0;

        while (i <= atual->n && k > atual->chave[i - 1]) {
            i++;
        }

        if (i <= atual->n && k == atual->chave[i - 1]) {
            pos = i - 1;
            break;
        }

        pos = i - 1;

        if (atual->filho[i - 1] == NULL) return; /* Seguranca */

        if(atual->folha == 1){ 
            break;
        }

        if(atual->filho[i - 1]->n < T){
            corrigirFilho(atual, i - 1);
        }

        pai = atual;
        atual = atual->filho[i - 1];

    }

    if(atual == NULL) return; /* segurança */

    /* Vamos começar a tratar os casos agora. Verificar o predecessor e sucessor. */

    if(atual->folha == 1){
        // Caso 1: chave em folha -> só remove deslocando
        for (int j = pos; j < atual->n - 1; j++) {
            atual->chave[j] = atual->chave[j + 1];
        }
        atual->n--;

    }else{
        NoArvB *anterior = NULL;
        NoArvB *posterior = NULL;

        if(pos >= 0){
            anterior = atual->filho[pos];
        }

        if(pos + 1 <= atual->n){
            posterior = atual->filho[pos + 1];
        }

        if(anterior != NULL && anterior->n >= T){ /* Pegar o maior do anterior. */
            int valor = anterior->chave[anterior->n - 1];
            atual->chave[pos] = valor;
            remover(anterior, valor);
        }else{
            if(posterior != NULL && posterior->n >= T){ /* Pegar o menor do posterior. */
                int valor = posterior->chave[0];
                atual->chave[pos] = valor;
                remover(posterior, valor);
                    }else{ /* Fazer o Merge pq não dá pra tirar nada de nenhum dos dois. */
                        if (pos < atual->n) {
                        merge(atual, pos);
                        if (atual->filho[pos] != NULL){
                            atual->filho[pos] = remover(atual->filho[pos], k);
                        }
                        return;
                    } else {
                        merge(atual, pos - 1);
                        if (atual->filho[pos - 1] != NULL){
                            atual->filho[pos] = remover(atual->filho[pos], k);
                        }
                        return; 
                        }
                    }
        }
    }

    if (no->n == 0 && !no->folha) { /* Limpa a raiz caso fique vazia depois dos merge. */
        NoArvB *old = no;
        no = no->filho[0];
        free(old);
    }

    return no;
}


int main(){
    setlocale(LC_ALL,"");

    NoArvB *raiz = NULL;

    int valores[] = {10,20,5,6,12,30,7,17};
    int n = sizeof(valores)/sizeof(valores[0]);

    // Inserindo
    for(int i=0;i<n;i++){
        raiz = insereArvoreB(raiz, valores[i]);
    }

    printf("Arvore apos insercoes:\n");
    imprimirArvore(raiz,0);

    // Removendo folha
    printf("\nRemovendo 6 (folha)\n");
    remover(raiz,6);
    imprimirArvore(raiz,0);

    // Removendo chave em nó interno
    printf("\nRemovendo 12 (no interno)\n");
    remover(raiz,12);
    imprimirArvore(raiz,0);

    // Removendo que gera merge
    printf("\nRemovendo 7 (gera merge)\n");
    remover(raiz,7);
    imprimirArvore(raiz,0);

    // Removendo raiz
    printf("\nRemovendo 10 (raiz)\n");
    raiz = remover(raiz,10);
    imprimirArvore(raiz,0);


    limpar(raiz);
return 0;
}