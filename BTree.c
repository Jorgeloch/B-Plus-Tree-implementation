#include "BPTree.h"

bool insert(BPlusTree *tree, void *reg, int value)
{
  // encontrar nó onde o valor deve ser inserido
  Node *leaf = findCorrectLeafNode(tree->root, value);
  // inserir valor no nó em ordem
  insertKey(tree, leaf, reg, value);
  return true;
}
bool delete(BPlusTree *tree, int value)
{
  if (search(tree, value))
  {
    return deleteFromNode(findCorrectLeafNode(tree->root, value), value);
  }
  return false;
}
bool deleteFromNode(Node *node, int value)
{
  int position = 0;
  while (node->keys[position] != value)
  {
    position++;
  }
  for (int n = numOfKeys(node); position < n; position++)
  {
    node->keys[position] = node->keys[position + 1];
  }
  node->numOfKeys--;
  if (numOfKeys(node) >= ORDER)
  {
    return true;
  }
  return lookForSiblings(node);
}
bool lookForSiblings(Node *node)
{
  Node *parent = node->parent;
  Node *leftSibling = NULL, *rightSibling = NULL;

  for (int i = 0, n = numOfKeys(parent); i <= n; i++)
  {
    if (parent->childrens[i + 1] == node)
    {
      leftSibling = parent->childrens[i];
    }
    if (i > 0 && parent->childrens[i - 1] == node)
    {
      rightSibling = parent->childrens[i];
      break;
    }
  }

  if (rightSibling != NULL && numOfKeys(rightSibling) > ORDER)
  {
    return borrowRight(node, parent, rightSibling);
  }
  if (leftSibling != NULL && numOfKeys(leftSibling) > ORDER)
  {
    return borrowLeft(node, parent, leftSibling);
  }
  return false;
}
bool borrowLeft(Node *node, Node *parent, Node *leftSibling)
{
  // inserir maior chave do irmao esquerdo
  int biggerKey = leftSibling->keys[numOfKeys(leftSibling) - 1];
  leftSibling->numOfKeys--;
  // reordenar array de chaves do irmao esquerdo
  int position = numOfKeys(node) - 1;
  while(position >= 0)
  {
    node->keys[position + 1] = node->keys[position];
    position--;
  }
  node->keys[0] = biggerKey;
  // incrementar numero de chaves no no alvo
  node->numOfKeys++;

  position = 0;
  while (parent->keys[position] != node->keys[1])
  {
    position++;
  }
  parent->keys[position] = biggerKey;
  return true;
}
bool borrowRight(Node *node, Node *parent, Node *rightSibling)
{
  int smallerKey = rightSibling->keys[0];
  node->keys[numOfKeys(node)] = smallerKey;
  node->numOfKeys++;

  for (int i = 0, n = numOfKeys(rightSibling) - 1; i < n; i++)
  {
    rightSibling->keys[i] = rightSibling->keys[i + 1];
  }
  rightSibling->numOfKeys--;

  int position = 0;
  while (parent->keys[position] != node->keys[numOfKeys(node) - 1])
  {
    position++;
  }
  parent->keys[position] = rightSibling->keys[0];
  return true;
}
bool mergeLeafLeft (Node *node, Node *parent, Node *leftSibling)
{
  int smallerKey  = node->keys[0];
  leftSibling->next = node->next;
  for (int i = 0, n = numOfKeys(node); i < n; i++)
  {
    leftSibling->keys[ORDER + i] = node->keys[i];
    leftSibling->numOfKeys++;
  }
  free(node);
  return deleteFromNode(parent, smallerKey);
}
bool mergeNodeLeft (Node *node, Node *parent, Node *leftSibling)
{
  for (int i = 0, n = numOfKeys(node); i < n; i++)
  {
    leftSibling->keys[ORDER + i] = node->keys[i];
    leftSibling->numOfKeys++;
  }
  for (int i = 1, n = numOfKeys(node); i <= n; i++)
  {
    leftSibling->childrens[ORDER + 1] = node->childrens[i];
  }
  free(node);
  return deleteFromNode(parent, leftSibling->keys[0]);
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
  while (i >= 0 && value < node->keys[i])
  {
    node->keys[i + 1] = node->keys[i];
    i--;
  }
  node->keys[i + 1] = value;
  node->numOfKeys++;
  int j = numOfKeys(node) - 1;
  for (; j > i + 1; j--)
  {
    node->childrens[j + 1] = node->childrens[j];
  }
  node->childrens[j + 1] = child;
  if (isFull(node))
  {
    splitNode(tree, node);
  }
}
void printNode(Node *node)
{
  int n = numOfKeys(node);
  printf("Keys: [ ");
  for (int i = 0; i < n; i++)
  {
    printf("%d ", node->keys[i]);
  }
  printf("]\n");
  printf("Childrens: [\n");
  for (int i = 0; i < n + 1; i++)
  {
    if (node->childrens[i])
    {
      printf("\n");
      printNode(node->childrens[i]);
      printf("\n");
    }
  }
  printf("]\n");
}
void printLeaf(Node *leaf)
{
  while(leaf)
  {
    printf("[ ");
    for (int i = 0, n = numOfKeys(leaf); i < n; i ++)
    {
      printf("%d ", leaf->keys[i]);
    }
    printf("] => ");
    leaf = leaf->next;
  }
  printf("NULL\n");
}
void printTree(BPlusTree *tree)
{
  Node *curr = tree->root;
  while (!isLeaf(curr))
  {
    curr = curr->childrens[0];
  }
  printLeaf(curr);
}
bool splitNode(BPlusTree *tree, Node *node)
{
  Node *newNode = createNode();
  newNode->leaf = node->leaf;
  newNode->numOfKeys = ORDER + 1;
  for (int i = 0; i < ORDER; i++)
  {
    newNode->keys[i] = node->keys[ORDER + i];
    node->keys[ORDER + i] = 0;
  }
  newNode->keys[ORDER] = node->keys[2 * ORDER];
  for (int i = 0; i < ORDER + 2; i++)
  {
    newNode->childrens[i] = node->childrens[ORDER + i];
    if(newNode->childrens[i])
    {
      Node *child = newNode->childrens[i];
      child->parent = newNode;
    }
    node->childrens[ORDER + 1] = NULL;
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
    return true;
  }
  // CASO 2 - estamos em um nó interno
  // MOVER o valor da posicao 0 do nó direito no nó pai  
  int removed = newNode->keys[0];
  for (int i = 0, n = numOfKeys(newNode); i < n - 1; i++)
  {
    newNode->keys[i] = newNode->keys[i + 1];
  }
  newNode->numOfKeys--;
  newNode->keys[numOfKeys(newNode)] = 0;
  for (int j = 0, n = numOfKeys(newNode) + 2; j < n - 1; j++)
  {
    newNode->childrens[j] = newNode->childrens[j + 1];
  }
  newNode->childrens[numOfKeys(newNode) + 1] = NULL;
  insertKey(tree, parent, newNode, removed);
  return true;
}
int numOfKeys(Node *node)
{
  return node->numOfKeys;
}
int getPosition(int *keys, int sizeOfKeys, int value)
{
  int position = 0;
  while (value >= keys[position] && position < sizeOfKeys)
  {
    position++;
  }
  return position;
}
int height(BPlusTree *tree)
{
  return tree->height;
}
Node *findCorrectLeafNode(Node *node, int value)
{
  Node *curr = node;
  while (!isLeaf(curr))
  {
    int position = 0;
    while (value >= curr->keys[position] && position < numOfKeys(curr))
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
