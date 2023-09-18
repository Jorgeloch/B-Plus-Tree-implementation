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
  int find = 0;
  do
  {
    printf("Qual valor voce deseja procurar?");
    scanf("%d", &find);
    if (!search(root, find))
    {
      printf("not found!\n");
      continue;
    }
    printf("found!\n");
  }
  while (find != -1);
}
