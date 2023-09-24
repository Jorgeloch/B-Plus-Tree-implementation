#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef B_PLUS_TREE
#define B_PLUS_TREE
#define ORDER 3

typedef struct Node {
  struct Node *next;
  struct Node *parent;
  // reservando um espaço extra para as chaves, para auxiliar no momento da inserção
  int keys[(ORDER * 2) + 1];
  void *childrens[(ORDER * 2) + 2];
  int numOfKeys;
  bool leaf;
} Node;

typedef struct {
  Node *root;
  int height;
} BPlusTree;

bool search(BPlusTree *tree, int value); // OK
bool insert(BPlusTree *tree, void *reg, int value); // OK
bool removeValue(BPlusTree *tree, int value);
bool isLeaf(Node *Node); // OK
bool isFull(Node *Node); // OK
bool searchOnNode(Node *Node, int value); // OK
bool splitNode(BPlusTree *tree, Node *node); // OK
void insertKey(BPlusTree *tree, Node *node, void *child, int value); // OK
void initializeTree(BPlusTree *tree); // OK
void printNode(Node *Node); // OK
void printTree(BPlusTree *tree);
int numOfKeys(Node *Node); // OK
int getPosition(int *keys, int sizeOfKeys, int value);// OK
int height(BPlusTree *tree);// OK
Node *findCorrectLeafNode(Node *root, int value);// OK
Node *createNode();// OK
BPlusTree *createTree();// OK

#endif //B_PLUS_TREE
