#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Definições de constantes
#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    POR_NOME = 1,
    POR_TIPO = 2,
    POR_PRIORIDADE = 3
} CriterioOrdenacao;

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade;
} Item;

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
Item mochila[MAX_ITENS];
int numItens = 0;
long comparacoes = 0;
bool ordenadaPorNome = false;

// Protótipos das funções
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();
void limparBuffer(); // Função auxiliar para evitar erros de leitura

int main() {
    int opcao;

    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("Saindo da ilha... Boa sorte, sobrevivente!\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}

// Função auxiliar para limpar buffer do teclado (evita pular fgets)
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    limparTela();
    printf("=== MOCHILA DE SOBREVIVENCIA (FREE FIRE) ===\n");
    printf("Itens na mochila: %d/%d\n", numItens, MAX_ITENS);
    printf("Status da Mochila: %s\n", ordenadaPorNome ? "ORDENADA POR NOME" : "NAO ORDENADA POR NOME");
    printf("--------------------------------------------\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar os itens\n");
    printf("5. Realizar busca binaria por nome\n");
    printf("0. Sair\n");
    printf("--------------------------------------------\n");
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("Erro: A mochila esta cheia! Remova itens antes de adicionar novos.\n");
        return;
    }

    Item novoItem;

    printf("Nome do item: ");
    fgets(novoItem.nome, MAX_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // Remove o \n do final

    printf("Tipo do item (ex: arma, municao, cura): ");
    fgets(novoItem.tipo, MAX_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);

    do {
        printf("Prioridade (1-Baixa a 5-Alta): ");
        scanf("%d", &novoItem.prioridade);
        if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
            printf("Prioridade invalida! Digite entre 1 e 5.\n");
        }
    } while (novoItem.prioridade < 1 || novoItem.prioridade > 5);
    limparBuffer(); // Limpar buffer após leitura de inteiros

    mochila[numItens] = novoItem;
    numItens++;
    ordenadaPorNome = false; // A inserção quebra qualquer ordenação existente

    printf("Item adicionado com sucesso!\n");
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("Digite o nome do item para remover: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int i, j;
    bool encontrado = false;

    for (i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = true;
            // Remover: deslocar todos os itens à frente uma posição para trás
            for (j = i; j < numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            numItens--;
            printf("Item '%s' removido com sucesso.\n", nomeBusca);
            break; // Item removido, sai do loop
        }
    }

    if (!encontrado) {
        printf("Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

// listarItens():
void listarItens() {
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }
    
    // Exibe uma tabela formatada com todos os componentes presentes na mochila.
    printf("\n%-30s | %-20s | %-5s | %-5s\n", "NOME", "TIPO", "QTD", "PRIOR");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-30s | %-20s | %-5d | %-5d\n", 
            mochila[i].nome, 
            mochila[i].tipo, 
            mochila[i].quantidade, 
            mochila[i].prioridade);
    }
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(CriterioOrdenacao criterio) {
    int i, j;
    Item key;
    comparacoes = 0;

    for (i = 1; i < numItens; i++) {
        key = mochila[i];
        j = i - 1;
        
        bool trocar = false;

        // O loop while verifica a condição baseada no critério
        while (j >= 0) {
            comparacoes++; // Contabiliza a comparação feita dentro do loop
            
            if (criterio == POR_NOME) {
                if (strcmp(mochila[j].nome, key.nome) > 0) trocar = true;
                else trocar = false;
            } 
            else if (criterio == POR_TIPO) {
                if (strcmp(mochila[j].tipo, key.tipo) > 0) trocar = true;
                else trocar = false;
            } 
            else if (criterio == POR_PRIORIDADE) {
                // Prioridade: da mais alta (5) para a mais baixa (1)
                // Se a prioridade do anterior for MENOR que a do atual, deve trocar
                if (mochila[j].prioridade < key.prioridade) trocar = true;
                else trocar = false;
            }

            if (trocar) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break; // Se não precisa trocar, encerra o while para este item
            }
        }
        mochila[j + 1] = key;
    }
}

// menuDeOrdenacao():
void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("Itens insuficientes para ordenar.\n");
        return;
    }
    
    // Permite ao jogador escolher como deseja ordenar os itens.
    int opcao;
    printf("\n--- CRITERIOS DE ORDENACAO ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (Alta-Baixa)\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limparBuffer();
    
    // Utiliza a função insertionSort() com o critério selecionado.
    switch (opcao) {
        case 1:
        insertionSort(POR_NOME);
        ordenadaPorNome = true;
        printf("Mochila ordenada por NOME.\n");
        break;
        case 2:
        insertionSort(POR_TIPO);
        ordenadaPorNome = false;
        printf("Mochila ordenada por TIPO.\n");
        break;
        case 3:
        insertionSort(POR_PRIORIDADE);
        ordenadaPorNome = false;
        printf("Mochila ordenada por PRIORIDADE.\n");
        break;
        default:
        printf("Opcao invalida. Nenhuma ordenacao realizada.\n");
        return;
    }
    
    // Exibe a quantidade de comparações feitas (análise de desempenho).
    printf("Numero de comparacoes realizadas: %ld\n", comparacoes);
    listarItens();
}

// buscaBinariaPorNome():
void buscaBinariaPorNome() {
    // Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
    if (!ordenadaPorNome) {
        printf("ERRO: A mochila precisa estar ordenada por NOME para realizar a busca binaria.\n");
        printf("Va ao menu de ordenacao e selecione a opcao 1.\n");
        return;
    }
    
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    
    char nomeBusca[MAX_NOME];
    printf("Digite o nome do item para buscar: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
    
    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    bool encontrado = false;
    int comparacoesBusca = 0;
    
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoesBusca++;
        
        int resultado = strcmp(mochila[meio].nome, nomeBusca);
        
        // Se encontrar, exibe os dados do item buscado.
        if (resultado == 0) {
            printf("\n--- ITEM ENCONTRADO ---\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("-----------------------\n");
            printf("Comparacoes na busca: %d\n", comparacoesBusca);
            encontrado = true;
            break;
        } else if (resultado < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    // Caso contrário, informa que não encontrou o item.
    if (!encontrado) {
        printf("Item '%s' nao encontrado.\n", nomeBusca);
        printf("Comparacoes na busca: %d\n", comparacoesBusca);
    }
}