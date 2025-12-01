#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropa;
} Territorio;

// Funcao para exibir o mapa
void exibirMapaMundo(Territorio *territorios, int quantidade) {
    printf("\nMAPA DO MUNDO - ESTADO ATUAL\n");
    printf("================================\n");

    for (int i = 0; i < quantidade; i++) {
        printf("\nTERRITORIO %d:\n", i + 1);
        printf("    - Nome: %s\n", territorios[i].nome);
        printf("    - Dominado por: Exercito %s\n", territorios[i].cor);
        printf("    - Tropas: %d\n", territorios[i].tropa);
    }
}

// Funcao de ataque entre territorios
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nRolagem de dados:\n");
    printf("    - Atacante (%s): %d\n", atacante->nome, dadoAtacante);
    printf("    - Defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nO atacante venceu!\n");
        defensor->tropa--;

        if (defensor->tropa <= 0) {
            printf("O territorio %s foi conquistado pelo exercito %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropa = atacante->tropa / 2;
            atacante->tropa -= defensor->tropa;
        }
    } else {
        printf("\nO defensor resistiu ao ataque!\n");
        atacante->tropa--;
    }
}

// Funcao para simular ataque com escolha de territorios
void simularAtaque(Territorio *territorios, int quantidade) {
    int atacante, defensor;

    printf("\n--- ATAQUE ENTRE TERRITORIOS ---\n");
    exibirMapaMundo(territorios, quantidade);

    printf("\nEscolha o numero do territorio atacante: ");
    scanf("%d", &atacante);
    getchar();

    printf("Escolha o numero do territorio defensor: ");
    scanf("%d", &defensor);
    getchar();

    atacante--;
    defensor--;

    if (atacante < 0 || atacante >= quantidade || defensor < 0 || defensor >= quantidade || atacante == defensor) {
        printf("Escolhas invalidas. Tente novamente.\n");
        return;
    }

    if (territorios[atacante].tropa < 1) {
        printf("O territorio atacante nao possui tropas suficientes!\n");
        return;
    }

    atacar(&territorios[atacante], &territorios[defensor]);
}

// Funcao para atribuir missao ao jogador
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Funcao para verificar se a missao foi cumprida (logica simples)
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    if (strstr(missao, "vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropa > 0) {
                return 0; // Missao nao cumprida
            }
        }
        return 1; // Missao cumprida
    }

    if (strstr(missao, "3 territorios seguidos") != NULL) {
        for (int i = 0; i < tamanho - 2; i++) {
            if (strcmp(mapa[i].cor, mapa[i + 1].cor) == 0 &&
                strcmp(mapa[i].cor, mapa[i + 2].cor) == 0) {
                return 1;
            }
        }
        return 0;
    }

    return 0; // Missao generica nao cumprida
}

// Funcao para liberar memoria
void liberarMemoria(Territorio *territorios, char *missaoJogador) {
    free(territorios);
    free(missaoJogador);
    printf("Memoria liberada com sucesso.\n");
}

int main() {
    srand(time(NULL)); // Inicializa gerador de numeros aleatorios

    int quantidade = 5;
    Territorio *territorios = (Territorio *)malloc(quantidade * sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        return 1;
    }

    // Vetor de missoes pre-definidas
    char *missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Dominar o territorio central",
        "Ter pelo menos 10 tropas em um territorio",
        "Controlar todos os territorios com nome iniciando em A"
    };
    int totalMissoes = 5;

    // Aloca missao do jogador
    char *missaoJogador = (char *)malloc(100 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para a missao.\n");
        free(territorios);
        return 1;
    }

    atribuirMissao(missaoJogador, missoes, totalMissoes);
    printf("\nMISSAO DO JOGADOR: %s\n", missaoJogador);

    printf("\nVamos cadastrar os 5 territorios iniciais do nosso mundo:\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d:\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor (nome do exercito): ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropa);
        getchar();
    }

    int opcao;
    do {
        printf("\nMENU DE ACOES\n");
        printf("1. Exibir mapa\n");
        printf("2. Realizar ataque\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                exibirMapaMundo(territorios, quantidade);
                break;
            case 2:
                simularAtaque(territorios, quantidade);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

        // Verificacao silenciosa da missao
        if (verificarMissao(missaoJogador, territorios, quantidade)) {
            printf("\nPARABENS! Voce cumpriu sua missao: %s\n", missaoJogador);
            break;
        }

    } while (opcao != 0);

    liberarMemoria(territorios, missaoJogador);
    return 0;
}