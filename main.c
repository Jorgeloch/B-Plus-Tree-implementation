#include "BPTree.h"

int main (void)
{
  BPlusTree *tree = createTree();
  printNode(tree->root);
  for (int i = 0; i < 12; i++)
  {
    int value;
    printf("Escolha um numero: ");
    scanf("%d", &value);
    insert(tree, NULL, value);
    printNode(tree->root);
  }
  int find = 0;
  do
  {
    printf("Qual valor voce deseja procurar?");
    scanf("%d", &find);
    if (!search(tree, find))
    {
      printf("not found!\n");
      continue;
    }
    printf("found!\n");
  }
  while (find != -1);
}
