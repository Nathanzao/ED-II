#include "AB-ArquivoBinario.h"

void exibirValorDaChaveArquivo(char *dir, char *arquivo) {
    char caminho[PATH_MAX];
    strcpy(caminho, arquivo);

    if(dir != NULL){
        snprintf(caminho, sizeof(caminho), "%s/%s", dir, arquivo);
    }

    
    FILE *f = fopen(caminho, "rb");
    if (f == NULL) {
        printf("Erro. Nao foi possivel abrir %s.\n", caminho);
        return;
    }

    int valor = 0;
    if (fread(&valor, sizeof(int), 1, f) == 1) {
        printf("Arquivo: %s e valor = %d\n", arquivo, valor);
    } else {
        printf("Erro. Falha ao ler valor de %s\n", caminho);
    }
    fclose(f);
}


NoArvB *criaNo(int folha, int max) {
    NoArvB *no = malloc(sizeof(NoArvB));
    no->n = 0;
    no->folha = folha;

    no->chave = malloc(max * sizeof(char*));
    for (int i = 0; i < max; i++) {
        no->chave[i] = malloc(PATH_MAX * sizeof(char));
    }

    // aloca vetor de filhos
    no->filho = malloc((max+1) * sizeof(NoArvB*));
    for (int i = 0; i <= max; i++) {
        no->filho[i] = NULL;
    }

    return no;
}

NoArvB* BuscaArvoreB(NoArvB *r, char *k, int *pos) { /* Função de busca. */
    int i = 1;
    

    // Busca sequencial para encontrar a posição ou ultrapassar
    while (i <= r->n && strcmp(k, r->chave[i - 1]) > 0) {
        i++;
    }

    // Se encontrou a chave no nó atual
    if (i <= r->n && strcmp(k, r->chave[i - 1]) == 0) { /* Precisa do i - 1 para pegar a posição real do vetor. */
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


void split(NoArvB *pai, int pos, NoArvB *y, ArvoreB *arv){ /* Y = nó que será dividido. Pai = nó pai do que será dividido. Pos = indice onde y está como filho de pai [(pai->filho[pos] == y)]. */
    NoArvB *z = criaNo(y->folha, arv->max); /* z = novo nó. */

    int meio = arv->t - 1; /* Pegar o valor que está na posição mediana. */

    for (int j = meio + 1, k = 0; j < y->n; j++, k++) { /* Vamos copiar os valores da direita da mediana ára o novo nó. */
        strcpy(z->chave[k], y->chave[j]);
        z->n++;
    }

    if(y->folha != 1){ /* Caso y não seja folha, vamos copiar seus filhos a partir da direita da mediana para o novo nó. */
        for (int j = meio + 1, k = 0; j <= y->n; j++, k++) {
            z->filho[k] = y->filho[j];
        }
    }

    char mediana[PATH_MAX];
    strcpy(mediana, y->chave[meio]); /* Valor que será inserido no nó pai. */
    y->n = meio; /* Atualizando a quantidade de elementos do nó com os valores da esquerda. */

    for (int j = pai->n; j >= pos + 1; j--) { /* Vamos começar do fim dos ponteiros dos filhos e ir voltando até achar o ponteiro dianteiro da posição original de y. Isso para encaixar z. */
        pai->filho[j + 1] = pai->filho[j]; /* Enquanto fazemos a parte de cima, jogamos os ponteiros do pai para direita, abrindo o espaço para botar o novo Z. */
    }
    pai->filho[pos + 1] = z;

    for (int j = pai->n - 1; j >= pos; j--) { /* Faz a mesma coisa, só que para as chaves. Por isso o n - 1.*/
        strcpy(pai->chave[j + 1], pai->chave[j]);
    }
    strcpy(pai->chave[pos], mediana);
    pai->n++;


    /* Note um detalhe: o pai nunca está cheio, por isso os dois FOR acima nunca estarão pegando aquele caso de MAX inválido.
    Ele não está cheio porque se tivesse, já teria justamente sofrido esse split. */
}

void insereRecursivo(NoArvB *x, char *k, NoArvB **novaRaiz, ArvoreB *arv){
    int i = x->n - 1; /* Pegando último elemento. */

    if(x->folha == 1){
        while(i >= 0 && strcmp(k, x->chave[i]) < 0){ /* Caso seja o último, irá cair na posição MAX. Logo, temporariamente irá ocupar uma posição inválida, mas logo após vem o split. */
            strcpy(x->chave[i + 1], x->chave[i]);
            i--;
        }
        strcpy(x->chave[i + 1], k);
        x->n++;
    }else{ /* Caso tenha filhos. */
        while(i >= 0 && strcmp(k, x->chave[i]) < 0){ /* Encontra o filho que queremos descer. */
            i--;
        }

        insereRecursivo(x->filho[++i], k, novaRaiz, arv); /* ++i, pois sempre irá parar entre um intervalo. */

        if(x->filho[i]->n >= arv->max){
            split(x, i, x->filho[i], arv); /* Se o filho está cheio, divida-o. */
        }
    }
}

NoArvB* insereArvoreB(NoArvB *raiz, int diretorio, ArvoreB *arv) { /* Fazemos efetivamente a inserção nessa função. A outra é só uma parte. */

    char dir[PATH_MAX];
    char k[PATH_MAX];

    printf("Digite o nome do arquivo binario (que ja existe) que deseja inserir na Arvore B: ");
    fgets(dir, sizeof(dir), stdin);
    dir[strcspn(dir, "\n")] = '\0'; // remove \n do fgets

    if(diretorio != 1){
        printf("Digite o novo diretorio que deseja: ");
        fgets(k, sizeof(k), stdin);
        k[strcspn(k, "\n")] = '\0'; // remove \n do fgets
        strcat(k, "/");
        strcat(k, dir);

        strcpy(dir, k);
    }

    FILE *f = fopen(dir, "r");

    if (!f) {
        printf("Erro. Nao foi possivel abrir %s.\n", dir);
        return raiz;
    }

    fclose(f);

    // CASO 1: A árvore está completamente vazia.
    if (raiz == NULL) {
        raiz = criaNo(1, arv->max); // Cria o primeiro nó (que é raiz e folha)
        strcpy(raiz->chave[0], dir);
        raiz->n = 1;

        printf("\nInsercao funcionou.\n");
        return raiz;
    }

    if((BuscaArvoreB(raiz, dir, 0)) != NULL){
        printf("\nErro. Ja existe esse valor na arvore.\n");
        return raiz;
    }

    // CASO 2: A árvore já existe. Delega a inserção para a função recursiva.
    insereRecursivo(raiz, dir, &raiz, arv);

    // CASO 3: A raiz ficou cheia após a inserção? (Ocorrem múltiplas divisões, chegou na raiz e agora a árvore precisa crescer em altura)
    if (raiz->n == arv->max) {
        NoArvB *nova = criaNo(0, arv->max); // 1. Cria uma NOVA raiz (que não é folha)
        nova->filho[0] = raiz;     // 2. A raiz ANTIGA vira filha da nova
        
        // 3. Divide a raiz antiga, promovendo a mediana para a nova raiz
        split(nova, 0, raiz, arv);
        
        raiz = nova; // 4. O ponteiro principal da árvore agora aponta para a nova raiz
    }

    printf("\nInsercao funcionou.\n");
    return raiz;
}

void imprimirArvoreComValores(NoArvB *raiz, int nivel, char *dir) {
    if (raiz == NULL) return;

    printf("Nivel %d -> ", nivel);
    for (int i = 0; i < raiz->n; i++) {
        printf("%s ", raiz->chave[i]);
    }
    printf("\n");

    
    for (int i = 0; i < raiz->n; i++) {
        exibirValorDaChaveArquivo(dir, raiz->chave[i]); // Vamos assumir que está tudo dentro do OUTPUT.
    }

    if (!raiz->folha) {
        for (int i = 0; i <= raiz->n; i++) {
            imprimirArvoreComValores(raiz->filho[i], nivel + 1, dir);
        }
    }
}


void limpar(NoArvB *raiz, int max){
    if(raiz == NULL) return;

    if(!raiz->folha){
        for(int i = 0; i <= raiz->n; i++){
            limpar(raiz->filho[i], max);
        }
    }

    for (int i = 0; i < max; i++) {
        free(raiz->chave[i]);
    }
    free(raiz->chave);
    free(raiz->filho);
    free(raiz);
}

/* A partir de agora, será a função de remoção que faremos. */

void merge(NoArvB *atual, int pos, ArvoreB *arv){
    NoArvB *filho  = atual->filho[pos];
    NoArvB *posterior = atual->filho[pos + 1];

    strcpy(filho->chave[arv->t - 1], atual->chave[pos]); /* Chave do pai desce ao filho (1) */

    for(int j = 0; j < posterior->n; j++){ /* Copiar para o filho os valores do posterior (2) */
        strcpy(filho->chave[j + arv->t], posterior->chave[arv->t]);
    }

    if(!filho->folha){ /* Se não for folha, copia os filhos. Obs: todos os filhos estão no mesmo nível. Se um for folha, outros também são = logo o posterior também não é folha. */
        for(int i = 0; i <= posterior->n; i++){
            filho->filho[i + arv->t] = posterior->filho[i];
        }
    }

    for (int i = pos + 1; i < atual->n; i++) {/* Move para esquerda as chaves do pai para tapar o buraco que ficou (3) */
        strcpy(atual->chave[i - 1], atual->chave[i]);
    }
    for (int i = pos + 2; i <= atual->n; i++) { /* Move os filhos. É dois porque o filho[pos + 1] deixou de existir.s*/
        atual->filho[i - 1] = atual->filho[i];
    }

    atual->n--;

    filho->n += posterior->n + 1; /* Somamos a quantidade de valores do filho com o irmão posterior, mais o 1 do nó pai que desceu. */

    free(posterior);
}

void corrigirFilho(NoArvB *atual, int pos, ArvoreB *arv){
    
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

    if(pos > 0 ){ /* Verificando se POS ocupa a primeira posição. Se for, NULL.*/
        anterior  = atual->filho[pos - 1];
    }
    
    if(pos < atual->n){ /* Verificando se POS é o último. Se for, NULL. */
        posterior = atual->filho[pos + 1];
    }
    

    NoArvB *filho = atual->filho[pos];

    if (anterior != NULL && anterior->n >= arv->t) { /* irmão esquerdo pode emprestar */
        
        for (int j = filho->n - 1; j >= 0; j--) { /* Abrindo espaço na primeira posição do filho, para inserir o valor do pai em sua primeira posição. */
            strcpy(filho->chave[j + 1], filho->chave[j]); /* Veja que usando o corrigir filho justamente porque ele tem menos que o mínimo. Então N nunca está cheio. */
        }
        if (!filho->folha) { /* Vamos deslocar os filhos também. */
            for (int j = filho->n; j >= 0; j--) {
                filho->filho[j + 1] = filho->filho[j];
            }
        }

        strcpy(filho->chave[0], atual->chave[pos - 1]); /* Chave do pai desce ao filho. */
        if(!filho->folha){
            filho->filho[0] = anterior->filho[anterior->n]; /* O filho da posição 0 do filho irá receber os filhos do maior valor de seu irmão a esquerda.*/
        }

        strcpy(atual->chave[pos - 1], anterior->chave[anterior->n - 1]); /* Maior filho da esquerda sobe ao pai. */
    
        filho->n++;
        anterior->n--;
    }
    else if (posterior != NULL && posterior->n >= arv->t) { /* irmão direito pode emprestar */
        
        strcpy(filho->chave[filho->n], atual->chave[pos]); /* Filho pega emprestado o valor do pai na última posição. */

        if(!filho->folha){
            filho->filho[filho->n + 1] = posterior->filho[0]; // O primeiro filho do irmão torna-se o último filho do filho
        }

        strcpy(atual->chave[pos], posterior->chave[0]); /* A primeira chave do posterior se torna a chave do pai. */

        // Move as chaves e filhos do irmão para a esquerda
        for (int i = 1; i < posterior->n; ++i)
            strcpy(posterior->chave[i - 1], posterior->chave[i]);
        if (!posterior->folha) {
            for (int i = 1; i <= posterior->n; ++i)
                posterior->filho[i - 1] = posterior->filho[i];
        }

        filho->n++;
        posterior->n--;
    }
    else { /* nenhum pode emprestar -> merge */
        if (pos < atual->n) {
            merge(atual, pos, arv); /* Merge com o irmão a direita. */
        } else {
            merge(atual, pos - 1, arv); /* Merge com o irmão da esquerda. */
        }
    }
}

NoArvB *remover(NoArvB *no, char *k, ArvoreB *arv){
    if(no == NULL){
        printf("Arvore vazia.\n");
        return;
    }

    printf("Removendo %s.\n", k);

    /* Não dá pra usar a função de busca já implementada. Ela retorna apenas o nó desejado, mas não o pai dele que usaremos para remoção. */

    NoArvB *atual = no;
    NoArvB *pai = NULL;

    int i = 1;
    int pos = 0;

    while(atual != NULL){   /* Achamos o pai, o nó e a posição no nó. Pos = posição do nó. */

        i = 1;
        pos = 0;

        while (i <= atual->n && strcmp(k, atual->chave[i - 1]) > 0) {
            i++;
        }

        if (i <= atual->n && strcmp(k, atual->chave[i - 1]) == 0) { /* Encontramos o valor nesse nó. */
            pos = i - 1;
            break;
        }

        pos = i - 1;


        if(atual->folha == 1){ /* Não achamos e é folha. */
            break;
        }

        if(atual->filho[pos]->n < arv->t){
            corrigirFilho(atual, pos, arv);
        }

        pai = atual;
        atual = atual->filho[i - 1];

    }


    /* Vamos começar a tratar os casos agora. Verificar o predecessor e sucessor. */

    if(atual == NULL || (atual->folha && (pos >= atual->n || strcmp(atual->chave[pos], k) != 0))){ /* Caso não achamos na árvore. */
        printf("%s nao encontrada na arvore.\n", k);
        return no;
    }

    if(atual->folha == 1){
        // Caso 1: chave em folha -> só remove deslocando para esquerda, sobrescrevendo em cima da chave.
        for (int j = pos; j < atual->n - 1; j++) {
            strcpy(atual->chave[j], atual->chave[j + 1]);
        }
        atual->n--;

    }else{ /* Caso não seja folha. */
        NoArvB *anterior = NULL;
        NoArvB *posterior = NULL;

        if(pos >= 0){ /* Verificando se o irmão esquerdo existe. */
            anterior = atual->filho[pos];
        }

        if(pos + 1 <= atual->n){ /* Verificando se o irmão direito existe. */
            posterior = atual->filho[pos + 1];
        }

        if(anterior != NULL && anterior->n >= arv->t){ /* Pegar o maior do anterior. */
            
            char valor[PATH_MAX];
            strcpy(valor, anterior->chave[anterior->n - 1]);
            
            strcpy(atual->chave[pos], valor);
            remover(anterior, valor, arv);
        }else{
            if(posterior != NULL && posterior->n >= arv->t){ /* Pegar o menor do posterior. */
                                
                char valor[PATH_MAX];
                strcpy(valor, posterior->chave[0]);
                
                strcpy(atual->chave[pos], valor);
                remover(posterior, valor, arv);
                    }else{ /* Fazer o Merge pq não dá pra tirar nada de nenhum dos dois. */
                        if (pos < atual->n) { /* Caso o pos não seja o último, fundimos com o irmão da direita. */
                            merge(atual, pos, arv);

                            if (atual->filho[pos] != NULL){ /* Verificamos se a árvore ficou vazia e removemos a raiz original. */
                                atual->filho[pos] = remover(atual->filho[pos], k, arv);
                            }

                    } else { /* Se for o último, será com o da esquerda. */
                            merge(atual, pos - 1, arv);
                            
                            if (atual->filho[pos - 1] != NULL){
                                atual->filho[pos] = remover(atual->filho[pos], k, arv);
                            }

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

float somarTudo(NoArvB *r){
    if(r == NULL){
        return 0;
    }

    float soma_total_nivel = 0;

    for(int i = 0; i < r->n; i++){
        char *caminho = r->chave[i];

        FILE *f = fopen(caminho, "rb");
        
        if(f == NULL){
            printf("\nErro na abertura do arquivo na funcao de somar tudo!\n");
            return -1;
        }

        int conteudo = -666;
        fread(&conteudo, sizeof(int), 1, f);

        if(conteudo != -666){
            soma_total_nivel += conteudo;
        }

        fclose(f);
    }

    if(!r->folha){
        for(int i = 0; i <= r->n; i++){
            soma_total_nivel += somarTudo(r->filho[i]);
        }
    }

    return soma_total_nivel;
}

void criarArquivoBinario(char *nomeArquivo){
    FILE *arq = fopen(nomeArquivo, "wb");

    if (arq == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }else{
        printf("Abrimos o arquivo, quando criado.\n");
    }

    int valor = 0;
    printf("\nDigite um valor para inserir no arquivo binario: ");
    scanf("%d", &valor);

    fwrite(&valor, sizeof(int), 1, arq);


fclose(arq);
}

ArvoreB* criaArvoreB(int t) {
    ArvoreB *arv = malloc(sizeof(ArvoreB));
    arv->t = t;
    arv->max = 2 * t - 1;
    return arv;
}
