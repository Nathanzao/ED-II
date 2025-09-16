#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define T 3
#define MAX (2*T - 1)

#define PATH_MAX 70

/*
Qualquer dúvida só olhar o resumo. Mas faremos algumas anotações aqui para lembrar:

    - b = 2t - 1;
    - t = minimo.

A única diferença de B para CLRS é os parâmetros.
*/

typedef struct No {
    int n; /* Quantidade já preenchida. */
    int folha; /* Define se é folha. Se 1, é. Se 0, não. */
    char chave[MAX][PATH_MAX];  
    struct No *filho[MAX+1]; /* Filho: precisa ter o +1 para englobar todos as possibilidades (menor e maior). */
} NoArvB, *ArvoreB;

// Função de uso real.
float somarTudo(NoArvB *r);

// Busca.
NoArvB* BuscaArvoreB(NoArvB *r, char *k, int *pos);

// Inserir.
void insereRecursivo(NoArvB *x, char *k, NoArvB **novaRaiz);
NoArvB* insereArvoreB(NoArvB *raiz, int diretorio);

// Remover.
NoArvB *remover(NoArvB *no, char *k);

// Funções auxiliares gerais.
void exibirValorDaChaveArquivo(char *dir, char *arquivo);
NoArvB* criaNo(int folha);
void imprimirArvoreComValores(NoArvB *raiz, int nivel, char *dir);
void limpar(ArvoreB raiz);
void criarArquivoBinario(char *nomeArquivo);

// Funções para ajudar na remoção/inserção.
void split(NoArvB *pai, int pos, NoArvB *y);
void merge(NoArvB *atual, int pos);
void corrigirFilho(NoArvB *atual, int pos);