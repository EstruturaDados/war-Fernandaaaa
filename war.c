#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//  Struct Territorio
//  Representa um território do jogo, com:
//   - nome   : nome do território
//   - cor    : cor do exército que controla o território
//   - tropas : quantidade de tropas presentes

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

//funções
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapaMundo(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    Territorio* mapa = NULL;   // ponteiro para o vetor dinâmico de territórios
    int quantidadeTerritorios;
    char buffer[32];
    int opcaoAtacante, opcaoDefensor;

    // srand(time(NULL)):
    // Inicializa o gerador de números aleatórios com o tempo atual,
    // garantindo valores diferentes a cada execução do programa.

    srand(time(NULL));

    printf("=== WAR ===\n\n");

    // Pergunta quantos territórios existirão no mapa
    printf("Informe o numero de territorios: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &quantidadeTerritorios);

    if (quantidadeTerritorios <= 0) {
        printf("Quantidade invalida de territorios.\n");
        return 1;
    }

    // Alocação dinâmica do vetor de Territorio
    // Usamos calloc para iniciar tudo com zero.

    mapa = (Territorio*)calloc(quantidadeTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        return 1;
    }

    // Cadastro inicial dos territórios (nome, cor e tropas)
    cadastrarTerritorios(mapa, quantidadeTerritorios);

    // Loop principal: mostra mapa, executa fase de ataque, repete até o jogador sair.
    while (1) {

        printf("\n=====================================\n");
        printf(" MAPA DO MUNDO - ESTADO ATUAL\n");
        printf("=====================================\n\n");
        exibirMapaMundo(mapa, quantidadeTerritorios);

        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ",
               quantidadeTerritorios);
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &opcaoAtacante);

        // 0 encerra o jogo
        if (opcaoAtacante == 0) {
            break;
        }

        // Validação do índice do atacante
        if (opcaoAtacante < 1 || opcaoAtacante > quantidadeTerritorios) {
            printf("Opcao de atacante invalida!\n");
            continue;
        }

        printf("Escolha o territorio defensor (1 a %d): ",
               quantidadeTerritorios);
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &opcaoDefensor);

        // Validação do índice do defensor
        if (opcaoDefensor < 1 || opcaoDefensor > quantidadeTerritorios) {
            printf("Opcao de defensor invalida!\n");
            continue;
        }

        // Converte de 1..N para 0..N-1
        int idxAtacante = opcaoAtacante - 1;
        int idxDefensor = opcaoDefensor - 1;

        // Validação de ataque:
        // impede que o jogador ataque um território do mesmo exército
        // (mesma cor).

        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("\nNao e permitido atacar um territorio do mesmo exercito!\n");
            continue;
        }

        // Verifica se o atacante tem tropas suficientes
        if (mapa[idxAtacante].tropas <= 0) {
            printf("\nO territorio atacante precisa ter pelo menos 1 tropa.\n");
            continue;
        }

        // Chama a função de ataque usando ponteiros
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

        // Pausa para o jogador ler o resultado
        printf("\nPressione Enter para continuar para o proximo turno...");
        fgets(buffer, sizeof(buffer), stdin);
    }
    //Liberação da memória dinâmica com free()
    liberarMemoria(mapa);
    printf("\nJogo encerrado.\n");
    return 0;
}

// cadastrarTerritorios
//   Preenche o vetor de Territorio com dados informados pelo usuário:
//   - nome do território
//   - cor do exército
//   - número de tropas
//   Usa fgets para permitir nomes e cores com espaços.

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    int i;
    char bufferTropas[20];

    printf("=== Cadastro de Territorios ===\n\n");

    for (i = 0; i < quantidade; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de Tropas: ");
        fgets(bufferTropas, sizeof(bufferTropas), stdin);
        sscanf(bufferTropas, "%d", &mapa[i].tropas);

        printf("\n");
    }
}

// exibirMapaMundo
//   Mostra o estado atual do mapa, um território por linha, no formato:
//   1. Nome (Exercito Cor, Tropas: X)

void exibirMapaMundo(Territorio* mapa, int quantidade) {
    int i;
    for (i = 0; i < quantidade; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
}

// atacar
//   Simula uma batalha entre dois territórios:
//   - Gera um valor de 1 a 6 para atacante e defensor (como dados)
//   - Se o atacante vencer, o defensor muda de cor e recebe metade
//     das tropas do atacante.
//   - Se o atacante perder, ele perde 1 tropa.

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;  // valor de 1 a 6
    int dadoDefensor = (rand() % 6) + 1;  // valor de 1 a 6

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n",
           atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n",
           defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATAQUE! O territorio defensor foi conquistado.\n");

        // Transfere a cor do exército vencedor para o território defensor
        strcpy(defensor->cor, atacante->cor);

        // Metade das tropas do atacante ocupam o território conquistado
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas == 0 && atacante->tropas > 0) {
            tropasTransferidas = 1;  // garante pelo menos 1 tropa
        }

        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
    } else {
        printf("VITORIA NA DEFESA! O atacante perdeu 1 tropa.\n");
        if (atacante->tropas > 0) {
            atacante->tropas--;
        }
    }
}

// liberarMemoria
//   Libera o vetor dinâmico de Territorio alocado com calloc/malloc.
//   Usa free() corretamente para evitar vazamento de memória.

void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}