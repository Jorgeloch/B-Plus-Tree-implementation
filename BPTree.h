#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef B_PLUS_TREE
#define B_PLUS_TREE

typedef struct Register {
  int id;
  char *name;
  char *class_name;
} Register;

typedef struct BPlusTree {
  int *keys;
  int numOfKeys;
  struct BPlusTree *parent;
  struct BPlusTree **childrens;
  struct BPlusTree *next;
  Register **registers;
  bool leaf;
} BPlusTree; 
bool search(BPlusTree *root, int value); 
bool insert(BPlusTree *root, int value);
bool removeNode(BPlusTree *root, int value);
bool isLeaf(BPlusTree *node);
bool isFull(BPlusTree *node);
bool isNotFull(BPlusTree *node);
bool insertKey(BPlusTree *node, int value);
int numOfKeys(BPlusTree *node);
BPlusTree *findCorrectLeafNode(BPlusTree *root, int value);
BPlusTree *createNode();
BPlusTree *splitChildren(BPlusTree *children, int position);
void intialize(BPlusTree *node);
void printNode(BPlusTree *node);


#endif //B_PLUS_TREE
