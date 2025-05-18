#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

typedef struct Node {
    char word[100];
    struct Node* left, * right;
} Node;

// Função para criar um novo nó
Node* createNode(char* word) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->word, word);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Inserir palavra na árvore
Node* insert(Node* root, char* word) {
    if (root == NULL)
        return createNode(word);

    if (strcmp(word, root->word) < 0)
        root->left = insert(root->left, word);
    else if (strcmp(word, root->word) > 0)
        root->right = insert(root->right, word);

    return root;
}

// Remover palavra da árvore
Node* removeNode(Node* root, char* word) {
    if (root == NULL)
        return root;

    if (strcmp(word, root->word) < 0)
        root->left = removeNode(root->left, word);
    else if (strcmp(word, root->word) > 0)
        root->right = removeNode(root->right, word);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = root->right;
        while (temp && temp->left != NULL)
            temp = temp->left;

        strcpy(root->word, temp->word);
        root->right = removeNode(root->right, temp->word);
    }

    return root;
}

// Buscar palavra na árvore
int search(Node* root, char* word) {
    if (root == NULL)
        return 0;
    if (strcmp(word, root->word) == 0)
        return 1;
    if (strcmp(word, root->word) < 0)
        return search(root->left, word);
    return search(root->right, word);
}

// Exibir palavras em ordem
void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%s\n", root->word);
        inOrder(root->right);
    }
}

// Função auxiliar para salvar palavras em ordem no arquivo
void saveInOrder(Node* node, FILE* file) {
    if (node != NULL) {
        saveInOrder(node->left, file);
        fprintf(file, "%s\n", node->word);
        saveInOrder(node->right, file);
    }
}

// Salvar palavras no arquivo
void saveToFile(const char* filename, Node* root) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    saveInOrder(root, file);
    fclose(file);
}

// Função para validar a entrada da palavra (somente letras e números)
int isValidWordInput(char* word) {
    int i = 0;
    while (word[i] != '\0') {
        if (!isalnum(word[i]))
            return 0;
        i++;
    }
    return 1;
}

// Buscar palavras que contenham uma substring
void searchSubstring(Node* root, char* substring) {
    if (root != NULL) {
        if (strstr(root->word, substring) != NULL)
            printf("%s\n", root->word);
        searchSubstring(root->left, substring);
        searchSubstring(root->right, substring);
    }
}

// Carregar palavras do arquivo para a árvore
Node* loadFromFile(const char* filename, Node* root) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo '%s'.\n", filename);
        return root;
    }

    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }

        if (isValidWordInput(word) && !search(root, word)) {
            root = insert(root, word);
        }
    }

    fclose(file);
    return root;
}

// Contar palavras armazenadas
int countWords(Node* root) {
    if (root == NULL)
        return 0;
    return 1 + countWords(root->left) + countWords(root->right);
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");
    Node* root = NULL;
    char filename[] = "palavras.txt";
    char word[100], substring[100];
    int choice;

    printf("Bem-vindo ao Indexador de Palavras!\n");

    root = loadFromFile(filename, root);

    do {
        printf("\n--- Menu ---\n");
        printf("1. Buscar uma palavra\n");
        printf("2. Buscar palavras por substring\n");
        printf("3. Exibir palavras em ordem\n");
        printf("4. Adicionar uma palavra ao banco de dados\n");
        printf("5. Remover uma palavra do banco de dados\n");
        printf("6. Contar palavras armazenadas\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        clock_t start, end;
        switch (choice) {
        case 1:
            printf("Digite a palavra que deseja buscar: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            start = clock();
            if (search(root, word))
                printf("Palavra '%s' encontrada!\n", word);
            else
                printf("Palavra '%s' não encontrada.\n");
            end = clock();
            printf("Tempo de busca: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;

        case 2:
            printf("Digite a substring: ");
            scanf("%s", substring);
            for (int i = 0; substring[i]; i++) {
                substring[i] = tolower(substring[i]);
            }
            start = clock();
            printf("Palavras que contêm a substring '%s':\n", substring);
            searchSubstring(root, substring);
            end = clock();
            printf("Tempo de busca: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;

        case 3:
            start = clock();
            printf("Palavras em ordem:\n");
            inOrder(root);
            end = clock();
            printf("Tempo para exibir palavras: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;

        case 4:
            printf("Digite a palavra que deseja adicionar: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            if (!isValidWordInput(word)) {
                printf("Palavra inválida! Somente letras e números são permitidos.\n");
            }
            else if (search(root, word)) {
                printf("A palavra '%s' já está no banco de dados.\n", word);
            }
            else {
                root = insert(root, word);
                saveToFile(filename, root);
                printf("Palavra '%s' adicionada ao banco de dados.\n", word);
            }
            break;

        case 5:
            printf("Digite a palavra que deseja remover: ");
            scanf("%s", word);
            for (int i = 0; word[i]; i++) {
                word[i] = tolower(word[i]);
            }
            start = clock();
            root = removeNode(root, word);
            saveToFile(filename, root);
            end = clock();
            printf("Palavra '%s' removida do banco de dados.\n", word);
            printf("Tempo para remover: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;

        case 6:
            start = clock();
            printf("Quantidade de palavras armazenadas: %d\n", countWords(root));
            end = clock();
            printf("Tempo para contar: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);
            break;

        case 7:
            printf("Saindo do programa...\n");
            break;

        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 7);

    return 0;
}
