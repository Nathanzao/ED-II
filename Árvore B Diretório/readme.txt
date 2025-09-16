Ferramentas utilizadas:
    - IDE : VsCode.

Como o código está funcionando:
    - TAD;
    - Existe uma função de criar arquivo binário dentro da própria implementação;
    - Funções e struct adaptadas para suportar uma String ao invés de um inteiro. Afinal, queremos o caminho do diretório. Para isso, usei as bibliotecas
    básicas de manipulações de String em C e seus comandos (strcmp, strcpy, ...);
    - Considerando as limitações do VsCode e do Windows para ler arquivos e outros diretórios, adaptei as funções. Elas conseguem executar normalmente
    os dois casos de: buscar um valor em um diretório diferente ou no atual (por atual me refiro à pasta OUTPUT do VsCode, porque ele não consegue ler
    arquivos fora dela no Windows);
        - PegaValorDoArquivo: nos da um nome de arquivo e possível diretório para ir atrás do valor.
        - Inserção: analisa se irá buscar um arquivo binário em um diretório diferente ou no atual (pasta OUTPUT).
        - Remoção: remove normalmente.
        - Imprimir: imprime o nome do arquivo binário e o valor que contém ele. Note que essa função recebe uma variável chamada "dir", que indica que iremos usar
        um diretório diferente do atual caso essa variável esteja diferente de NULL. Caso seja NULL, a função assume que os arquivos binários estão na pasta atual
        e segue em frente.

Problema prático:
    - Somar todos os valores dos arquivos binários. Por padrão, deixei 3 arquivos já na pasta OUTPUT: "ArquivoBinario.bin", "ArquivoBinario2.bin",
    "ArquivoBinario3.bin". Na main está havendo uma remoção para exemplificar, então não irá somar os três.

Possíveis erros:
    - Ao compilar, fica aparecendo notificações de aviso (não de erros) sobre return's sem NULL. Deixei eles desse jeito pois acho mais fácil visualizar.