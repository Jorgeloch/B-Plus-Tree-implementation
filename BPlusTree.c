#include "BPTree.h"
#include <stdlib.h>
#include <math.h>
#define ORDER 7

bool insert(BPlusTree *root, int value)
{
  // encontrar o nó onde o novo valor deve ser inserido
  BPlusTree *node = findCorrectLeafNode(root, value);
  // CASO 1 - o nó onde desejamos inserir o valor possui espaço para o novo valor a ser inserido
  if (isNotFull(node))
  {
    // chamada da função responsável por inserir o valor na posição correta do nó
    insertKey(node, value);
    return true;
  }
  // CASO 2 - o nó onde o valor deveria ser inserido está cheio
  else 
  {
    printf("NO CHEIO\n");
    // adicionar ao nó pai
    // if (node->parent)
    // {
    //   int valueToParent;
    //   BPlusTree newNode = splitNode(node, &valueToParent);
    //   insert(node->parent, valueToParent);
    // }
  }
  return false;
}
bool search(BPlusTree *root, int value)
{
  // search the correct leaf node 
  BPlusTree *curr = findCorrectLeafNode(root, value);
  // check if the requested value is within the node
  for (int i = 0, n = numOfKeys(curr); i < n; i++)
  {
    if (curr->keys[i] == value)
    {
      return true;
    }
  }
  // return false if not found
  return false;
}
bool removeValue(BPlusTree *root, int value)
{
  return false;
}
bool isLeaf(BPlusTree *tree)
{
  return tree->leaf;
}
int numOfKeys(BPlusTree *tree)
{
  return tree->numOfKeys;
}
bool isFull(BPlusTree *tree)
{
  return (numOfKeys(tree) == (ORDER - 1));
}
bool isNotFull(BPlusTree *tree)
{
  return !isFull(tree);
}
bool insertKey(BPlusTree *node, int value)
{
  int positionToInsert = 0;
  for (int i = 0, n = numOfKeys(node); i < n; i++)
  {
    if (value > node->keys[i])
    {
      positionToInsert++;
    }
  }
  for (int i = numOfKeys(node); i > positionToInsert; i--)
  {
    node->keys[i] = node->keys[i - 1];  
    // node->childrens[i + 1] = node->childrens[i];
  }
  node->keys[positionToInsert] = value;
  node->numOfKeys++;
  return true;
}
BPlusTree *findCorrectLeafNode(BPlusTree *root, int value)
{
  // utilizando variavel curr para percorrer a arvore
  BPlusTree *curr = root;
  // iterando encontrar um nó folha
  while (!isLeaf(curr))
  {
    // Se o valor a ser inserido for maior que o maior valor armazenado no nó
    if (value >= curr->keys[ORDER - 2])
    {
      // ireamos para o ponteiro mais a direita
      curr = curr->childrens[ORDER - 1];
    }
    // caso contrário
    else
    {
      // iteramos pelo array de chaves
      for (int i = 0; i < ORDER - 1; i++)
      {
        // caso o valor a ser inserido seja menor que o valor armazenado na posicao i do nó
        if (value < curr->keys[i])
        {
          curr = curr->childrens[i];
          break;
        }
      }
    }
  }
  return curr;
}
void initialize(BPlusTree *node)
{
  node->keys = (int *) calloc((ORDER - 1), sizeof(int));
  node->numOfKeys = 0; node->leaf = false;
  node->childrens = NULL; 
  node->leaf = true;
  node->next = NULL;
  node->registers = NULL;
  node->parent = NULL;
}
void printNode(BPlusTree *node)
{
  if (node->childrens)
  {
    printf("childrens: [ ");
    for (int i = 0, n = numOfKeys(node); i < n + 1; i++)
    {
      printf("%p ", node->childrens[i]);
    }
    printf("]\n");
  }
  else
  {
    printf("childrens = NULL\n");
  }
  printf("keys: [ ");
  for (int i = 0, n = numOfKeys(node); i < n; i++)
  {
    printf("%d ", node->keys[i]);
  }
  printf("]\n");
  printf("parent: %p\n", node->parent);
  printf("leaf: %d\n", node->leaf);
}
BPlusTree *createNode()
{
  BPlusTree *node = (BPlusTree *) malloc(sizeof(BPlusTree));
  if (!node)
  {
    printf("Impossivel criar no!\n");
    exit(EXIT_FAILURE);
  }
  initialize(node);
  return node;
}
