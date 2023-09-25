#include "BPTree.h"

bool search(BPlusTree *tree, int value)
{
  // encontra o nó onde a chave deveria estar
  Node *node = findCorrectLeafNode(tree->root, value);
  // realiza a busca no array de chaves do no
  return searchOnNode(node, value);
}
bool insert(BPlusTree *tree, void *reg, int value)
{
  // se o valor ja existir na arvore
  if (search(tree, value))
  {
    return false;
  }
  // encontrar nó onde o valor deve ser inserido
  Node *leaf = findCorrectLeafNode(tree->root, value);
  // inserir valor no nó em ordem
  insertKey(tree, leaf, reg, value);
  return true;
}
bool delete(BPlusTree *tree, int value)
{
  // se o valor existir 
  if (search(tree, value))
  {
    // deletar valor do no
    return deleteFromNode(findCorrectLeafNode(tree->root, value), value);
  }
  return false;
}
bool deleteFromNode(Node *node, int value)
{
  // encontrar posicao do valor a ser deletado
  int position = 0;
  while (node->keys[position] != value)
  {
    position++;
  }
  // remover o valor do array
  for (int n = numOfKeys(node); position < n; position++)
  {
    node->keys[position] = node->keys[position + 1];
  }
  // decrementar em um o numero de elementos no nó
  node->numOfKeys--;
  if (numOfKeys(node) < ORDER && node->parent)
  {
    // caso o nó possua um pai e o numero de elementos seja menor que o minimo
    return lookForSiblings(node);
  }
  return true;
}
bool lookForSiblings(Node *node)
{
  // inicializando variavies
  Node *parent = node->parent;
  Node *leftSibling = NULL, *rightSibling = NULL;
  // buscando o nó irmão direito e irmão esquerdo
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
  // se exisitir irmao direito e o mesmo puder emprestar uma chave
  if (rightSibling != NULL && numOfKeys(rightSibling) > ORDER)
  {
    // chama funcao para pegar emprestado do direito
    return borrowRight(node, parent, rightSibling);
  }
  // se existir irmao esquerdo e o mesmo puder emprestar uma chave
  if (leftSibling != NULL && numOfKeys(leftSibling) > ORDER)
  {
    // chama funcao para pegar emprestado do esquero
    return borrowLeft(node, parent, leftSibling);
  }
  return true;
}
bool borrowLeft(Node *node, Node *parent, Node *leftSibling)
{
  int biggerKey = leftSibling->keys[numOfKeys(leftSibling) - 1];
  // decrementar em um o numero de chaves no irmao esquerdo 
  leftSibling->numOfKeys--;
  // reordenar chaves do nó alvo para abrir espaço para o novo elemetno 
  int position = numOfKeys(node) - 1;
  while(position >= 0)
  {
    node->keys[position + 1] = node->keys[position];
    position--;
  }
  // inserindo nova chave no nó alvo
  node->keys[0] = biggerKey;
  // incrementar numero de chaves no no alvo
  node->numOfKeys++;
  // reajustando chaves no nó pai
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
  // inserindo menor chave do irmao direito no nó alvo
  node->keys[numOfKeys(node)] = smallerKey;
  // incrementando em um o numero de chaves no nó alvo
  node->numOfKeys++;
  // remover menor chave do irmão esquerda
  for (int i = 0, n = numOfKeys(rightSibling) - 1; i < n; i++)
  {
    rightSibling->keys[i] = rightSibling->keys[i + 1];
  }
  // decrementar em um o numero de chaves do irmao direito
  rightSibling->numOfKeys--;
  int position = 0;
  // reajustando chaves do nó pai
  while (parent->keys[position] != node->keys[numOfKeys(node) - 1])
  {
    position++;
  }
  parent->keys[position] = rightSibling->keys[0];
  return true;
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
  // itera sobre array verificando se o valor da chave é igual ao valor buscado
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
  // itera sobre o array movendo cada chave maior que o valor a ser inserido para a direita
  while (i >= 0 && value < node->keys[i])
  {
    node->keys[i + 1] = node->keys[i];
    i--;
  }
  // insere o novo valor na posicao correta
  node->keys[i + 1] = value;
  // incrementa em um o numero de chaves da prova
  node->numOfKeys++;
  int j = numOfKeys(node) - 1;
  // itera sobre o array de filhos do nó movendo cada um para a direita ate a posicao correta
  for (; j > i + 1; j--)
  {
    node->childrens[j + 1] = node->childrens[j];
  }
  // insere ponteiro para o filho na posicao correta
  node->childrens[j + 1] = child;
  // verifica se atingiu o limite maximo de chaves
  if (isFull(node))
  {
    // divide o nó em dois novos nós
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
  // inicializa novo nó
  Node *newNode = createNode();
  newNode->leaf = node->leaf;
  newNode->numOfKeys = ORDER + 1;
  // insere metade dos maiores valores no novo nó
  for (int i = 0; i < ORDER; i++)
  {
    newNode->keys[i] = node->keys[ORDER + i];
    node->keys[ORDER + i] = 0;
  }
  newNode->keys[ORDER] = node->keys[2 * ORDER];
  // insere os filhos no novo nó na ordem certa
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
  // caso nao possua um no pai
  if (!node->parent)
  {
    // inicializa um novo no para ser o pai
    node->parent = createNode();
    // reajusta raiz da arvore
    tree->root = node->parent;
    // incrementa a altura da arvore
    tree->height++;
    // insere os nos no pai 
    node->parent->childrens[0] = node;
    node->parent->childrens[1] = newNode;
  }
  Node *parent = node->parent;
  // ajusta o pai do novo nó
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
  // inicializa variavel para percorrer a arvore
  Node *curr = node;
  while (!isLeaf(curr))
  {
    // procura o proximo ponteiro correto
    int position = 0;
    while (value >= curr->keys[position] && position < numOfKeys(curr))
    {
      position++;
    }
    // atualiza a variavel atual
    curr = curr->childrens[position];
  }
  // quando encontra o nó correto, retorna o nó
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
