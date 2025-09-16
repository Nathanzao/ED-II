#include "AB-ArquivoBinario.c"

int main() {
    setlocale(LC_ALL,"");

    NoArvB *raiz = NULL;

    raiz = insereArvoreB(raiz, 1);
    raiz = insereArvoreB(raiz, 1);
    raiz = insereArvoreB(raiz, 1);


    printf("\n\nArvore apos tentativa de insercoes dos arquivos/diretorios:\n");

    printf("\n\n");
    imprimirArvoreComValores(raiz, 0, NULL);

    printf("\n\n");
    raiz = remover(raiz, "ArquivoBinario.bin");

    printf("\n\n");
    imprimirArvoreComValores(raiz, 0, NULL);

    printf("Fazendo a soma de todos os valores dos arquivos binarios: ");
    printf("\n\n...\n\nResultado: %f", somarTudo(raiz));

    limpar(raiz);

    printf("\n\nPressione para sair do programa.");
    getchar();
    return 0;
}