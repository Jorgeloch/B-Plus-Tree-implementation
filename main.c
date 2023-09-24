#include "BPTree.h"

int main (void)
{
  BPlusTree *tree = createTree();
  for (int value = 0; value != -1;)
  {
    printf("Escolha um numero: ");
    scanf("%d", &value);
    insert(tree, NULL, value);
    system("clear");
    printTree(tree);
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
