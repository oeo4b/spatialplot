#ifndef NODES_H
#define NODES_H
#include <stdio.h>
#include "polygons.h"

typedef struct Node {
  unsigned int n;
  unsigned int npoly;
  Polygon* polygons;
  struct Node* child;
} Node;

void readNodes(Node*);
void treeNodes(Node*, FILE***, unsigned int);

#endif
