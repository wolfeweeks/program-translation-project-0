#ifndef NODE_H
#define NODE_H

typedef struct node_t
{
  char value;
  char *string;
  int depth;

  struct node_t *left;
  struct node_t *middle;
  struct node_t *right;
} node_t;

#endif // NODE_H