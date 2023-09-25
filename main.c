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
    printf("Qual valor voce deseja procurar? ");
    scanf("%d", &find);
    system("clear");
    if (!search(tree, find))
    {
      printf("not found!\n");
      continue;
    }
    printf("found!\n");
  }
  while (find != -1);
  
  int remove = 0;
  do
  {
    printf("Qual valor voce deseja remover? ");
    scanf("%d", &remove);
    system("clear");
    if (!delete(tree, remove))
    {
      printf("not removed or unbalanced!\n");
      printTree(tree);
      continue;
    }
    printf("removed and balanced!\n");
    printTree(tree);
  }
  while (remove != -1);
}
