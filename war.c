#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5
#define TAM_MISSAO 100
#define QTD_MISSOES 5

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

Territorio* alocarMapa(int qtd);
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa, char* missao);
void exibirMapa(const Territorio* mapa, int qtd);
void exibirMenuPrincipal(void);
void exibirMissao(const char* missao);
void faseDeAtaque(Territorio* mapa, int qtd);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int sortearMissao(char* destino, char* missoes[], int total);
int verificarVitoria(const char* missao, const Territorio* mapa, int qtd);
void limparBufferEntrada(void);

// ----------------------------- Função Principal ----------------------------
int main() {
    srand(time(NULL)); // Garante aleatoriedade dos dados de ataque

    // 1. Setup inicial
    Territorio* mapa = alocarMapa(QTD_TERRITORIOS);
    if (!mapa) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }
    inicializarTerritorios(mapa);

    // Missoes disponíveis
    char* missoesPossiveis[QTD_MISSOES] = {
        "Destruir o exercito Vermelho.",
        "Destruir o exercito Azul.",
        "Conquistar 3 territorios com mais de 5 tropas.",
        "Controlar todos os territorios com o exercito Verde.",
        "Acumular 20 tropas no total."
    };

    // Sorteia a missão do jogador
    char* missaoJogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    sortearMissao(missaoJogador, missoesPossiveis, QTD_MISSOES);

    int opcao, venceu = 0;

    // 2. Loop principal do jogo
    do {
        printf("\n==========================================\n");
        printf("========== MAPA DO MUNDO ATUAL ==========\n");
        printf("==========================================\n");
        exibirMapa(mapa, QTD_TERRITORIOS);

        printf("\n--- SUA MISSAO ---\n");
        exibirMissao(missaoJogador);

        exibirMenuPrincipal();
        printf("Escolha sua acao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, QTD_TERRITORIOS);
                break;
            case 2:
                if (verificarVitoria(missaoJogador, mapa, QTD_TERRITORIOS)) {
                    printf("\nParabens! Voce cumpriu sua missao!\n");
                    venceu = 1;
                } else {
                    printf("\nA missao ainda nao foi cumprida.\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }

        if (verificarVitoria(missaoJogador, mapa, QTD_TERRITORIOS)) {
            venceu = 1;
            break;
        }

    } while (opcao != 0 && !venceu);

    if (venceu) {
        printf("\n==========================================\n");
        printf("MISSAO CUMPRIDA! Voce venceu o jogo!\n");
        printf("Missao: %s\n", missaoJogador);
        printf("==========================================\n");
    }

    // 3. Limpeza de memória
    liberarMemoria(mapa, missaoJogador);
    return 0;
}

// ------------------------ Implementação das Funções ------------------------

// Aloca dinamicamente o vetor de territórios
Territorio* alocarMapa(int qtd) {
    return (Territorio*)calloc(qtd, sizeof(Territorio));
}

// Inicializa os territórios com nomes, cores e tropas fixas
void inicializarTerritorios(Territorio* mapa) {
    strcpy(mapa[0].nome, "America");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Europa");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Asia");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Africa");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Oceania");
    strcpy(mapa[4].cor, "Branco");
    mapa[4].tropas = 1;
}

// Libera toda a memória alocada dinamicamente
void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa) free(mapa);
    if (missao) free(missao);
}

// Exibe o menu principal de ações
void exibirMenuPrincipal(void) {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
}

// Exibe o estado atual do mapa
void exibirMapa(const Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe a missão do jogador
void exibirMissao(const char* missao) {
    printf("%s\n", missao);
}

// Interface da fase de ataque
void faseDeAtaque(Territorio* mapa, int qtd) {
    int a, d;
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o territorio atacante (1 a %d): ", qtd);
    scanf("%d", &a);
    printf("Escolha o territorio defensor (1 a %d): ", qtd);
    scanf("%d", &d);
    limparBufferEntrada();

    a--; d--;

    if (a < 0 || a >= qtd || d < 0 || d >= qtd) {
        printf("Indices invalidos!\n");
        return;
    }

    if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
        printf("Nao e permitido atacar um territorio do mesmo exercito!\n");
        return;
    }

    if (mapa[a].tropas <= 0) {
        printf("O territorio atacante precisa ter pelo menos 1 tropa.\n");
        return;
    }

    simularAtaque(&mapa[a], &mapa[d]);
}

// Executa a lógica de ataque com rand() entre 1 e 6
void simularAtaque(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Ataque (%s): %d | Defesa (%s): %d\n",
           atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        printf("VITORIA DO ATAQUE! O territorio foi conquistado.\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= defensor->tropas;
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        if (atacante->tropas > 0) atacante->tropas--;
    }
}

// Sorteia e retorna uma missão
int sortearMissao(char* destino, char* missoes[], int total) {
    int idx = rand() % total;
    strcpy(destino, missoes[idx]);
    return idx;
}

// Verifica se a missão foi cumprida
int verificarVitoria(const char* missao, const Territorio* mapa, int qtd) {
    // Exemplo de lógica simples
    if (strstr(missao, "Vermelho")) {
        for (int i = 0; i < qtd; i++)
            if (strcmp(mapa[i].cor, "Vermelho") == 0)
                return 0;
        return 1;
    }

    if (strstr(missao, "Azul")) {
        for (int i = 0; i < qtd; i++)
            if (strcmp(mapa[i].cor, "Azul") == 0)
                return 0;
        return 1;
    }

    if (strstr(missao, "20 tropas")) {
        int total = 0;
        for (int i = 0; i < qtd; i++)
            total += mapa[i].tropas;
        if (total >= 20) return 1;
    }

    return 0;
}

// Limpa o buffer de entrada (stdin)
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
