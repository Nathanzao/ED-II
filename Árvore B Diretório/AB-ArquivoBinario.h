#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

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
    char **chave;  
    struct No **filho; /* Filho: precisa ter o +1 para englobar todos as possibilidades (menor e maior). */
} NoArvB;

typedef struct{
    int t;
    int max;
} ArvoreB;

// Função de uso real.
float somarTudo(NoArvB *r);

// Busca.
NoArvB* BuscaArvoreB(NoArvB *r, char *k, int *pos);

// Inserir.
void insereRecursivo(NoArvB *x, char *k, NoArvB **novaRaiz, ArvoreB *arv);
NoArvB* insereArvoreB(NoArvB *raiz, int diretorio, ArvoreB *arv);

// Remover.
NoArvB *remover(NoArvB *no, char *k, ArvoreB *arv);

// Funções auxiliares gerais.
void exibirValorDaChaveArquivo(char *dir, char *arquivo);
NoArvB* criaNo(int folha, int max);
void imprimirArvoreComValores(NoArvB *raiz, int nivel, char *dir);
void limpar(NoArvB *raiz, int max);
void criarArquivoBinario(char *nomeArquivo);

// Funções para ajudar na remoção/inserção.
void split(NoArvB *pai, int pos, NoArvB *y, ArvoreB *arv);
void merge(NoArvB *atual, int pos, ArvoreB *arv);
void corrigirFilho(NoArvB *atual, int pos, ArvoreB *arv);

// Funções exclusivas para T dinâmico.
ArvoreB* criaArvoreB(int t);