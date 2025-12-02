#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 20

// Estrutura da sala (nó da árvore binária da mansão)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da pista (nó da BST)
typedef struct Pista {
    char descricao[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

// Estrutura da entrada da tabela hash
typedef struct EntradaHash {
    char pista[100];
    char suspeito[50];
    struct EntradaHash *prox;
} EntradaHash;

EntradaHash *tabela_hash[TAMANHO_HASH];
Pista *raiz_pistas = NULL;// Raiz da BST de pistas


// Função hash simples para strings
int hash(const char *str) {
    int h = 0;
    while (*str) h = (h * 31 + *str++) % TAMANHO_HASH;// 31 é um número primo comum em funções hash
    return h;
}


// Cria dinamicamente uma sala da mansão
Sala* criarSala(const char *nome) {
    Sala *nova = malloc(sizeof(Sala));// Aloca memória para a nova sala
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}


// Insere uma pista na BST de forma ordenada
Pista* inserirPista(Pista *raiz, const char *descricao) {
    if (!raiz) {
        Pista *nova = malloc(sizeof(Pista));
        strcpy(nova->descricao, descricao);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(descricao, raiz->descricao) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    else if (strcmp(descricao, raiz->descricao) > 0)
        raiz->direita = inserirPista(raiz->direita, descricao);
    return raiz;
}

// Insere associação pista/suspeito na tabela hash
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = hash(pista);
    EntradaHash *nova = malloc(sizeof(EntradaHash));
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->prox = tabela_hash[indice];
    tabela_hash[indice] = nova;
}

// Consulta o suspeito correspondente a uma pista
char* encontrarSuspeito(const char *pista) {
    int indice = hash(pista);
    EntradaHash *atual = tabela_hash[indice];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Navega pela árvore e ativa o sistema de pistas
void explorarSalas(Sala *sala) {
    if (!sala) return;

    printf("\nVoce entrou na sala: %s\n", sala->nome);

    // Pistas associadas a salas
    if (strcmp(sala->nome, "Biblioteca") == 0) {
        printf("Pista encontrada: Luvas com sangue\n");
        raiz_pistas = inserirPista(raiz_pistas, "Luvas com sangue");
    } else if (strcmp(sala->nome, "Cozinha") == 0) {
        printf("Pista encontrada: Faca suja\n");
        raiz_pistas = inserirPista(raiz_pistas, "Faca suja");
    } else if (strcmp(sala->nome, "Sala de Jantar") == 0) {
        printf("Pista encontrada: Calice quebrado\n");
        raiz_pistas = inserirPista(raiz_pistas, "Calice quebrado");
    }

    char escolha;
    printf("Escolha: esquerda (e), direita (d), sair (s): ");
    scanf(" %c", &escolha);

    if (escolha == 'e') explorarSalas(sala->esquerda);
    else if (escolha == 'd') explorarSalas(sala->direita);
    else printf("Exploracao encerrada.\n");
}


void listarPistas(Pista *raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->descricao);
    listarPistas(raiz->direita);
}


// Conduz à fase de julgamento final
int contarPistasSuspeito(Pista *raiz, const char *suspeito) {
    if (!raiz) return 0;
    int cont = 0;
    char *s = encontrarSuspeito(raiz->descricao);
    if (s && strcmp(s, suspeito) == 0) cont++;
    cont += contarPistasSuspeito(raiz->esquerda, suspeito);
    cont += contarPistasSuspeito(raiz->direita, suspeito);
    return cont;
}

void verificarSuspeitoFinal() {
    char acusacao[50];
    printf("\nPistas coletadas:\n");
    listarPistas(raiz_pistas);

    printf("\nQuem voce acusa? ");
    scanf(" %[^\n]", acusacao);

    int total = contarPistasSuspeito(raiz_pistas, acusacao);
    if (total >= 2)
        printf("\nAcusacao valida! %s é o culpado com %d pistas.\n", acusacao, total);
    else
        printf("\nAcusacao fraca. Apenas %d pista(s) apontam para %s.\n", total, acusacao);
}


int main() {
    // Montagem manual da mansão
    Sala *entrada = criarSala("Entrada");
    entrada->esquerda = criarSala("Biblioteca");
    entrada->direita = criarSala("Cozinha");
    entrada->esquerda->esquerda = criarSala("Sala de Jantar");

    // Associação de pistas a suspeitos
    inserirNaHash("Luvas com sangue", "Sr. Black");
    inserirNaHash("Faca suja", "Sra. White");
    inserirNaHash("Calice quebrado", "Srta. Scarlet");

    // Início da exploração
    printf("Bem-vindo a mansao Enigma!\n");
    explorarSalas(entrada);

    // Fase final
    verificarSuspeitoFinal();

    return 0;
}
