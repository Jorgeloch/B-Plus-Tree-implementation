#include "BPTree.h"

bool insert(BPlusTree *tree, void *reg, int value)
{
  // encontrar nó onde o valor deve ser inserido
  Node *leaf = findCorrectLeafNode(tree->root, value);
  // inserir valor no nó em ordem
  insertKey(tree, leaf, reg, value);
  return true;
}
bool search(BPlusTree *tree, int value)
{
  Node *node = findCorrectLeafNode(tree->root, value);
  return searchOnNode(node, value);
}
bool isLeaf(Node *node)
{
  return node->leaf;
}
bool isFull(Node *node)
{
  int n = numOfKeys(node);
  return n == ((2 * ORDER) + 1);
}
bool searchOnNode(Node *node, int value)
{
  for (int i = 0, n = numOfKeys(node); i < n; i++)
  {
    if (node->keys[i] == value)
    {
      return true;
    }
  }
  return false;
}
void initializeTree(BPlusTree *tree)
{
  tree->root = createNode();
  tree->root->leaf = true;
  tree->height = 0;
}
void insertKey(BPlusTree *tree, Node *node, void *child, int value)
{
  int i = numOfKeys(node) - 1;
  int j = numOfKeys(node);
  while (i >= 0 && value < node->keys[i])
  {
    node->keys[i + 1] = node->keys[i];
    i--;
  }
  node->keys[i + 1] = value;
  node->numOfKeys++;
  if (isFull(node))
  {
    printf("Node has suffered an overflow\n");
    splitNode(tree, node);
  }
}
void printNode(Node *node)
{
  int n = numOfKeys(node);
  printf("address: %p\n", node);
  printf("Parent: %p\n", node->parent);
  printf("Keys: [ ");
  for (int i = 0; i < n; i++)
  {
    printf("%d ", node->keys[i]);
  }
  printf("]\n");
  printf("Number Of Keys: %d\n", numOfKeys(node));
  printf("Childrens: [ ");
  for (int i = 0; i < n + 1; i++)
  {
    if (node->childrens[i])
    {
      printf("\n\n");
      printNode(node->childrens[i]);
      printf("\n\n");
    }
  }
  printf("]\n");
  printf("Leaf: %s\n", node->leaf ? "True" : "False");
  printf("Next: %p\n", node->leaf ? node->next : NULL);
}
bool splitNode(BPlusTree *tree, Node *node)
{
  Node *newNode = createNode();
  newNode->leaf = node->leaf;
  newNode->numOfKeys = ORDER + 1;
  for (int i = 0; i < ORDER; i++)
  {
    newNode->keys[i] = node->keys[ORDER + i];
  }
  newNode->keys[ORDER] = node->keys[2 * ORDER];
  for (int i = 0; i < ORDER + 1; i++)
  {
    newNode->childrens[i] = node->childrens[ORDER + i];
  }
  node->numOfKeys = ORDER;
  if (!node->parent)
  {
    node->parent = createNode();
    tree->root = node->parent;
    node->parent->childrens[0] = node;
    node->parent->childrens[1] = newNode;
  }
  Node *parent = node->parent;
  newNode->parent = parent;
  // CASO 1 - estamos em um nó folha
  if (isLeaf(node))
  {
    // COPIAR valor da posicao 0 do nó direito no nó pai
    newNode->next = node->next;
    node->next = newNode;
    // como o valor não foi removido da árvore, apenas inserimos no no pai o valor novo
    insertKey(tree, parent, newNode, newNode->keys[0]);
    int position = getPosition(parent->keys, numOfKeys(parent), newNode->keys[0]);
    
    return true;
  }
  // CASO 2 - estamos em um nó interno
  // MOVER o valor da posicao 0 do nó direito no nó pai  
  int removed = removeFromNode(newNode, newNode->keys[0]);
  insertKey(tree, parent, newNode, removed);
  return true;
}
void orderChildrens(Node *node, void *child, int position)
{
  //for (int i = numOfKeys(node); i > position; i++)
  //{
  // node->childrens[i + 1] = node->childrens[i];
  //}
  //node->childrens[position] = child;
}
int numOfKeys(Node *node)
{
  return node->numOfKeys;
}
int getPosition(int *keys, int sizeOfKeys, int value)
{
  int position = 0;
  while (value > keys[position] && position < sizeOfKeys)
  {
    position++;
  }
  return position;
}
int height(BPlusTree *tree)
{
  return tree->height;
}
int removeFromNode(Node *node, int value)
{
  int n = numOfKeys(node);
  int position = getPosition(node->keys, n, value);
  int removed = node->keys[position];
  for (int i = position; i < n; i++)
  {
    node->keys[i] = node->keys[i - 1];
  }
  node->numOfKeys--;
  return removed;
}
Node *findCorrectLeafNode(Node *node, int value)
{
  Node *curr = node;
  while (!isLeaf(curr))
  {
    int position = 0;
    while (value > curr->keys[position] && position < numOfKeys(curr))
    {
      position++;
    }
    curr = curr->childrens[position];
  }
  return curr;
}
Node *createNode()
{
  Node *node = calloc(1, sizeof(Node));
  return node;
}
BPlusTree *createTree()
{
  BPlusTree *tree = calloc(1, sizeof(BPlusTree));
  initializeTree(tree);
  return tree;
}
