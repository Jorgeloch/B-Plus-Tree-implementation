#include "BPTree.h"
void testDelete(BPlusTree *tree);
void testInsert(BPlusTree *tree);
void testSearch(BPlusTree *tree);

int main (void)
{
  BPlusTree *tree = createTree();
  system("clear\n");
  while (true)
  {
    int option;
    printf("que operacao voce deseja realizar? (-1 para sair)\n");
    printf("    1. inserir\n");
    printf("    2. buscar\n");
    printf("    3. deletar\n");
    scanf("%d", &option);
    system("clear");
    if (option == -1)
    {
      break;
    }
    switch (option)
    {
    case 1:
      testInsert(tree);
      break;
    case 2:
      testSearch(tree);
      break;
    case 3:
      testDelete(tree);
      break;
    default:
      printf("escolha uma opção válida!\n");
      break;
    }
  }
}
void testDelete(BPlusTree *tree)
{
  int value;
  printf("qual valor voce deseja remover?\n");
  scanf("%d", &value);
  if (!delete(tree, value))
  {
    printf("not removed\n");
    printTree(tree);
  }
  else 
  {
    printf("removed!\n");
    printTree(tree);
  }
}
void testInsert(BPlusTree *tree)
{
  int value;
  printf ("Escolha um numero: ");
  scanf ("%d", &value);
  if (!insert(tree, NULL, value))
  {
    printf("not inserted!\n");
  }
  else
  {
    printf("inserted!\n");
  }
  printTree(tree);
}
void testSearch(BPlusTree *tree)
{
  int value;
  printf("qual valor voce deseja buscar? ");
  scanf("%d", &value);
  if (!search(tree, value))
  {
    printf("not found!\n");
  }
  else
  {
    printf("found!\n");
  }
  printTree(tree);
}