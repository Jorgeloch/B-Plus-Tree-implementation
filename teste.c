#include "BPTree.h"

int main(void)
{
    BPlusTree *tree = createTree();
    for (int i = 1; i < 25; i++)
    {
        insert(tree, NULL, i);
    }
    int value;
    printf("escolha o proximo numero a ser inserido: ");
    scanf("%d", &value);
    insert(tree, NULL, value);
    search(tree, value);
}