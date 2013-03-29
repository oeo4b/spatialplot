#ifndef NODES_H
#define NODES_H
#include <stdio.h>
#include "polygons.h"

typedef struct Node {
  unsigned char flag;
  unsigned int n;
  unsigned int npoly;
  unsigned int nchar;
  unsigned char* name;
  double xy[2];
  double bbox[4];
  Polygon* polygons;
  struct Node* child;
} Node;

void readNodes(Node*, char**, unsigned int);
void treeNodes(Node*, FILE***, unsigned int);
void ytrans(double*);
void ytransNodes(Node*);
void printNodes(Node*);

#endif
