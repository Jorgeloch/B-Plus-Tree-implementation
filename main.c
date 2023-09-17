#include "BPTree.h"

int main (void)
{
    BPlusTree *root = createNode();
    printNode(root);
    for (int i = 0; i < 6; i++)
    {
        int value;
        printf("Escolha um numero: ");
        scanf("%d", &value);
        insert(root, value);
        printNode(root);
    }
}