#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HuffmanNode {
    char data;
    struct HuffmanNode *left, *right;
} HuffmanNode;

HuffmanNode* createNode(char data) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

void insertCode(HuffmanNode* root, char* code, char data) {
    HuffmanNode* current = root;
    for (int i = 0; i < strlen(code); i++) {
        if (code[i] == '0') {
            if (!current->left)
                current->left = createNode('\0');
            current = current->left;
        } else {
            if (!current->right)
                current->right = createNode('\0');
            current = current->right;
        }
    }
    current->data = data;
}

void decompressFile(const char* inputFile, const char* outputFile) {
    FILE* input = fopen(inputFile, "rb");
    if (!input) {
        printf("Error opening input file!\n");
        return;
    }

    HuffmanNode* root = createNode('\0');

    char line[512];
    while (fgets(line, sizeof(line), input)) {
        if (line[1] == ':') {
            char data = line[0];
            char code[256];
            sscanf(line + 2, "%s", code);
            insertCode(root, code, data);
        } else {
            break;
        }
    }

    FILE* output = fopen(outputFile, "wb");
    if (!output) {
        printf("Error opening output file!\n");
        fclose(input);
        return;
    }

    HuffmanNode* current = root;
    char ch;
    while (fread(&ch, 1, 1, input)) {
        if (ch == '0')
            current = current->left;
        else if (ch == '1')
            current = current->right;

        if (!current->left && !current->right) {
            fwrite(&current->data, 1, 1, output);
            current = root;
        }
    }

    fclose(input);
    fclose(output);
    printf("Decompression successful! Output saved in %s\n", outputFile);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <compressed_file>\n", argv[0]);
        return 1;
    }

    char outputFile[256];
    snprintf(outputFile, sizeof(outputFile), "decompressed_%s", argv[1]);

    decompressFile(argv[1], outputFile);
    printf("Success!\n");

    return 0;
}
