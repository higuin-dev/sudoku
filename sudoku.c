#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//salvar os melhores tempos
void salvarTempos(int melhoresTempos[3]){
    FILE *file = fopen("menores_tempos.txt", "w");
    if (file == NULL){
        printf("erro ao abrir o arquivo.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        fprintf(file, "%d\n", melhoresTempos[i]);
    }
    fclose(file);
    printf("Tempos salvos com sucesso!\n");
}

//carregar os melhores tempos de um arquivo
void carregarTempos(int melhoresTempos[3]){
    FILE *file = fopen("menores_tempos.txt", "r");
    if (file == NULL){
        printf("arquivo de melhores tempos não encontrado ou inexistente.\n");
        for (int i = 0; i < 3; i++){
            melhoresTempos[i] = 0;
        }
        return;
    }
    for (int i = 0; i < 3; i++){
        if (fscanf(file, "%d", &melhoresTempos[i]) != 1){
            melhoresTempos[i] = 0;
        }
    }
    fclose(file);
    printf("melhores tempos carregados xD!\n");
}

//registrar o tempo gasto e atualizar os melhores tempos
void registraTempo(int tempoGasto, int melhoresTempos[3]){
    for (int i = 0; i < 3; i++) {
        if (melhoresTempos[i] == 0 || tempoGasto < melhoresTempos[i]){
            for (int j = 2; j > i; j--){
                melhoresTempos[j] = melhoresTempos[j - 1];
            }
            melhoresTempos[i] = tempoGasto;
            break;
        }
    }
}

//carregar um jogo do arquivo
void geraJogo(const char *arquivo, int sudoku[9][9], int numeroJogo){
    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("erro ao abrir o arquivo de jogos...\n");
        exit(1);
    }

    //pular para o jogo escolhido
    for (int i = 0; i < (numeroJogo-1) * 9; i++){
        char buffer[100];
        fgets(buffer, sizeof(buffer), file);
    }

    //carregar o jogo
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }

    fclose(file);
}

//escolher a dificuldade do jogo
const char* Dificuldade(){
    int dificuldade;
    printf("selecione a dificuldade do game:\n1 - easy\n2 - médio\n3 - dificil dms uai\n");
    scanf("%d", &dificuldade);
    switch (dificuldade){
        case 1: return "mapas_faceis.txt";
        case 2: return "mapas_medios.txt";
        case 3: return "mapas_dificeis.txt";
        default:
            printf("opção invalida. nível fácil por padrão.\n");
            return "mapas_faceis.txt";
    }
}

//escolher um jogo
void escolheJogo(const char *arquivo, int sudoku[9][9]){
    char opcao;
    int numeroJogo;
    getchar();
    printf("deseja escolher um jogo específico, gerar um aleatório ou escolher com base no melhor tempo?\n");
    printf("a - escolher;\nb - aleatório;\nc - melhor tempo\n");
    scanf("%c", &opcao);
    getchar();
    if(opcao == 'a'){
        printf("digite o número do jogo:\n");
        scanf("%d", &numeroJogo);
    } else if(opcao == 'b'){
        numeroJogo = rand() % 5 + 1;
    } else if(opcao == 'c'){
        numeroJogo = 1; 
    } else {
        printf("opção inválida, gerando um jogo aleatório:\n");
        numeroJogo = rand() % 5 + 1;
    }
    geraJogo(arquivo, sudoku, numeroJogo);
}

//exibir o tabuleiro de sudoku
void exibirTabuleiro(int sudoku[9][9]){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (sudoku[i][j] == 0){
                printf(". ");
            } else {
                printf("%d ", sudoku[i][j]);
            }
        }
        printf("\n");
    }
}

//verifica se as linhas do sudoku estão corretas
int verificaLinhas(int sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        int numeros[10] = {0};  
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != 0) {
                if (numeros[sudoku[i][j]] == 1) {
                    return 0;  //jogo errado :(
                }
                numeros[sudoku[i][j]] = 1;
            }
        }
    }
    return 1;  //jogo certo :)
}

//verifica se as colunas do sudoku estão corretas
int verificaColunas(int sudoku[9][9]) {
    for (int j = 0; j < 9; j++) {
        int numeros[10] = {0}; 
        for (int i = 0; i < 9; i++) {
            if (sudoku[i][j] != 0) {
                if (numeros[sudoku[i][j]] == 1) {
                    return 0;  //jogo errado :(
                }
                numeros[sudoku[i][j]] = 1;
            }
        }
    }
    return 1;  //jogo certo :)
}

//verifica se as subgrades 3x3 do sudoku estão corretas
int verificaSubgrades(int sudoku[9][9]) {
    for (int inicioLinha = 0; inicioLinha < 3; inicioLinha++) {
        for (int inicioColuna = 0; inicioColuna < 3; inicioColuna++) {
            int numeros[10] = {0};  
            for (int i = inicioLinha * 3; i < inicioLinha * 3 + 3; i++) {
                for (int j = inicioColuna * 3; j < inicioColuna * 3 + 3; j++) {
                    if (sudoku[i][j] != 0) {
                        if (numeros[sudoku[i][j]] == 1) {
                            return 0;  //jogo errado :(
                        }
                        numeros[sudoku[i][j]] = 1;
                    }
                }
            }
        }
    }
    return 1; //jogo certo :)
}

//verifica se o jogo está correto
int verificaJogo(int sudoku[9][9]) {
    return verificaLinhas(sudoku) && verificaColunas(sudoku) && verificaSubgrades(sudoku);
}

//contar quantos zeros ainda restam no tabuleiro
int cont_zeros(int sudoku[9][9]){
    int count = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            if (sudoku[i][j] == 0){
                count++;
            }
        }
    }
    return count;
}

//menu
int menu(){
    int escolha;
    printf("carregar novo jogo ou antigo?\n1 - novo jogo\n2 - continuar jogo anterior\n");
    scanf("%d", &escolha);
    return escolha;
}

//aumentar os espaços vazios
void aumentaVazio(int sudoku[9][9], int quantidade) {
    int count = 0;
    while (count < quantidade) {
        int linha = rand() % 9;
        int coluna = rand() % 9;
        if (sudoku[linha][coluna] != 0) {
            sudoku[linha][coluna] = 0;
            count++;
        }
    }
}

int main() {
    int sudoku[9][9];
    int melhoresTempos[3];
    char opcao;
    int linha, coluna, valor, tempoGasto;
    time_t inicio, fim;

    srand(time(NULL));

    carregarTempos(melhoresTempos);

    const char* arqDificuldade;

    // menu de carregamento
    
    if (menu() == 1) {
        arqDificuldade = Dificuldade();
        escolheJogo(arqDificuldade, sudoku);
    } else {
        arqDificuldade = Dificuldade();
        escolheJogo(arqDificuldade, sudoku);
        aumentaVazio(sudoku, 15);  // aumenta a dificuldade removendo mais 15 números
    }

    exibirTabuleiro(sudoku);

    inicio = time(NULL);

    do {
        printf("\n'a' p/ adicionar,\n'r' para remover,\n'v' para verificar o sudoku,\n's' para sair: ");
        scanf(" %c", &opcao);

        switch (opcao){
            case 'a':
                printf("digite a linha (1-9), coluna (1-9) e valor (1-9): ");
                scanf("%d %d %d", &linha, &coluna, &valor);
                linha--; 
                coluna--;
                if (sudoku[linha][coluna] == 0) {
                    sudoku[linha][coluna] = valor;
                } else {
                    printf("entrada invalida...\n");
                }
                exibirTabuleiro(sudoku);
                break;
            case 'r':
                printf("digite a linha (1-9) e coluna (1-9) para remover: ");
                scanf("%d %d", &linha, &coluna);
                linha--;  
                coluna--; 
                if (sudoku[linha][coluna] != 0) {
                    sudoku[linha][coluna] = 0;
                } else {
                    printf("não existe jogada para remover nessa posição.\n");
                }
                exibirTabuleiro(sudoku);
                break;
            case 'v':
                if (cont_zeros(sudoku) == 0) {
                    if (verificaJogo(sudoku)) {
                        fim = time(NULL);
                        tempoGasto = (int)(fim - inicio);
                        printf("parabéns, você conseguiu completar o sudoku corretamente!\n");
                        printf("tempo gasto: %d segundos\n", tempoGasto);
                        registraTempo(tempoGasto, melhoresTempos);
                        salvarTempos(melhoresTempos);
                        printf("melhores tempos:\n");
                        for (int i = 0; i < 3; i++) {
                            if (melhoresTempos[i] != 0) {
                                printf("%dº: %d segundos\n", i + 1, melhoresTempos[i]);
                            }
                        }
                    } else {
                        printf("o sudoku está incorreto. tente novamente.\n");
                    }
                } else {
                    printf("o tabuleiro ainda não está totalmente preenchido!\n");
                }
                break;
            case 's':
                printf("tchau...\n");
                break;
            default:
                printf("opção inválida!\n");
        }
    } while (opcao != 's');

    return 0;
}