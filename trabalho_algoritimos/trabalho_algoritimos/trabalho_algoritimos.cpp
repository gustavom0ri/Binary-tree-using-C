#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

// Estrutura do nó da árvore binária
typedef struct Node {
    char word[100];
    struct Node* left;
    struct Node* right;
} Node;

// Função para criar um novo nó
Node* newNode(char* word) {
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->word, word);
    node->left = node->right = NULL;
    return node;
}

// Função para inserir uma palavra na árvore binária
Node* insert(Node* root, char* word) {
    if (root == NULL) {
        return newNode(word);
    }

    if (strcmp(word, root->word) < 0) {
        root->left = insert(root->left, word);
    }
    else if (strcmp(word, root->word) > 0) {
        root->right = insert(root->right, word);
    }

    return root;
}

// Função para buscar uma palavra na árvore binária
int search(Node* root, char* word) {
    if (root == NULL) {
        return 0;
    }

    if (strcmp(word, root->word) == 0) {
        return 1;
    }
    else if (strcmp(word, root->word) < 0) {
        return search(root->left, word);
    }
    else {
        return search(root->right, word);
    }
}

// Função para exibir o passo a passo da busca
void searchStepByStep(Node* root, char* word) {
    if (root == NULL) {
        printf("Passo: Árvore vazia, palavra não encontrada.\n");
        return;
    }

    printf("Passo a Passo da Busca:\n");
    while (root != NULL) {
        printf("Compara '%s' com '%s'.\n", word, root->word);
        if (strcmp(word, root->word) == 0) {
            printf("Palavra '%s' encontrada!\n", word);
            return;
        }
        else if (strcmp(word, root->word) < 0) {
            printf("Vai para a subárvore esquerda.\n");
            root = root->left;
        }
        else {
            printf("Vai para a subárvore direita.\n");
            root = root->right;
        }
    }
    printf("Palavra '%s' não encontrada.\n", word);
}

// Função para carregar as palavras de um arquivo
Node* loadFromFile(char* filename, Node* root) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo '%s' não encontrado. Será criado um novo ao salvar as palavras.\n", filename);
        return root;
    }

    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]); // Converte para minúsculo
        }
        root = insert(root, word);
    }

    fclose(file);
    return root;
}

// Função para salvar a árvore no arquivo
void saveToFile(Node* root, FILE* file) {
    if (root == NULL) return;

    // Percorre a subárvore esquerda
    saveToFile(root->left, file);

    // Salva o nó atual no arquivo
    fprintf(file, "%s\n", root->word);

    // Percorre a subárvore direita
    saveToFile(root->right, file);
}

// Função para verificar se a entrada é um número inteiro válido
int isValidIntInput() {
    int choice;
    while (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Limpar o buffer
        printf("Entrada inválida! Digite um número inteiro: ");
    }
    return choice;
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");
    Node* root = NULL;
    char filename[] = "palavras.txt";
    char word[100];
    int choice;

    printf("Bem-vindo ao Indexador de Palavras!\n");

    root = loadFromFile(filename, root);

    do {
        printf("\n--- Menu ---\n");
        printf("1. Buscar uma palavra\n");
        printf("2. Buscar palavras por substring\n");
        printf("3. Exibir palavras em ordem\n");
        printf("4. Adicionar uma palavra\n");
        printf("5. Remover uma palavra\n");
        printf("6. Contar palavras armazenadas\n");
        printf("7. Buscar passo a passo\n");
        printf("8. Sair\n");
        printf("Escolha uma opção: ");
        choice = isValidIntInput();

        switch (choice) {
        case 1:
            printf("Digite a palavra que deseja buscar: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            if (search(root, word))
                printf("Palavra '%s' encontrada!\n", word);
            else
                printf("Palavra '%s' não encontrada.\n");
            break;

        case 7:
            printf("Digite a palavra para busca passo a passo: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            searchStepByStep(root, word);
            break;

        case 8:
        {
            FILE* file = fopen(filename, "w");
            if (file == NULL) {
                printf("Erro ao salvar o arquivo.\n");
            }
            else {
                saveToFile(root, file);
                fclose(file);
                printf("Palavras salvas em '%s'.\n", filename);
            }
            printf("Saindo...\n");
            break;
        }

        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (choice != 8);

    return 0;
}
