#ifndef TREE_H
#define TREE_H

#include "node.h"
#include "stdio.h"

void printPreorder(node_t *node, FILE *file);

void printInorder(node_t *node, FILE *file);

void printPostorder(node_t *node, FILE *file);

node_t *buildTree(char file[]);

#endif // TREE_H