#include <stdio.h>
#include <string.h>

#define QTD_TERRITORIOS 5  // Quantidade fixa de territórios que serão cadastrados

// Definição da struct Territorio:
// Representa um território com três informações básicas:
// - nome: nome do território
// - cor: cor do exército que ocupa o território
// - tropas: quantidade de tropas naquele território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // Vetor de structs: armazena os 5 territórios cadastrados pelo usuário
    Territorio territorios[QTD_TERRITORIOS];
    int i;

    // Buffer auxiliar para leitura da quantidade de tropas como string
    char bufferTropas[20];

    printf("=== Cadastro de Territorios ===\n\n");

    /*
     * Laço de entrada de dados:
     * - Percorre de 0 até QTD_TERRITORIOS - 1
     * - Em cada iteração, lê os dados de um território
     *   (nome, cor do exército e quantidade de tropas)
     * - Os dados são armazenados na posição correspondente do vetor 'territorios'
     */
    for (i = 0; i < QTD_TERRITORIOS; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // Leitura do nome do território com fgets
        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        // Leitura da cor do exército com fgets
        printf("Digite a cor do exercito: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        fgets(bufferTropas, sizeof(bufferTropas), stdin);
        sscanf(bufferTropas, "%d", &territorios[i].tropas);

        printf("\n");
    }

    printf("\n=== Territorios cadastrados ===\n\n");

    /*
     * Laço de saída de dados:
     * - Percorre novamente o vetor 'territorios'
     * - Em cada posição, exibe os valores que foram armazenados
     *   durante o laço de entrada, mostrando todos os territórios cadastrados
     */
    for (i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome    : %s\n", territorios[i].nome);
        printf("Exercito: %s\n", territorios[i].cor);
        printf("Tropas  : %d\n", territorios[i].tropas);
        printf("-----------------------------\n");
    }

    return 0;
}