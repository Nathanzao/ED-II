#include "AB-ArquivoBinario.c"

int main() {
    setlocale(LC_ALL,"Portuguese");

    NoArvB *raiz = NULL;
    int t = 0;

    printf("Defina o tamanho de T: ");
    scanf("%d", &t);
    fflush(stdin);

    ArvoreB *arv = criaArvoreB(t);

    raiz = insereArvoreB(raiz, 1, arv);
    raiz = insereArvoreB(raiz, 1, arv);
    raiz = insereArvoreB(raiz, 1, arv);

    printf("\n\nArvore apos tentativa de insercoes dos arquivos/diretorios:\n");

    printf("\n\n");
    imprimirArvoreComValores(raiz, 0, NULL);

    printf("\n\n");
    raiz = remover(raiz, "ArquivoBinario.bin", arv);

    printf("\n\n");
    imprimirArvoreComValores(raiz, 0, NULL);

    printf("Fazendo a soma de todos os valores dos arquivos binarios: ");
    printf("\n\n...\n\nResultado: %f", somarTudo(raiz));

    limpar(raiz, arv->max);

    printf("\n\nPressione para sair do programa.");
    getchar();
    return 0;
}