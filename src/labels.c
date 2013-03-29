#include <stdlib.h>
#include <stdio.h>
#include "labels.h"
#include "draw.h"
#include "nodes.h"
#include "blocks.h"

void bindLabels(Node* node, Block* block, unsigned int pad) {
  unsigned int i;
  double pixel = block->depth / (double)CELL;
  node->bbox[0] = node->xy[0] - pixel*(strwidth(node->name, node->nchar)/2+pad);
  node->bbox[1] = node->xy[1] - pixel*(pad);
  node->bbox[2] = node->xy[0] + pixel*(strwidth(node->name, node->nchar)/2+pad);
  node->bbox[3] = node->xy[1] + pixel*(strheight(node->name, node->nchar)+pad);
  node->flag = 0;
  for(i=0;i<node->n;i++)
    bindLabels(&node->child[i], block, pad);
}

void inclLabels(Node* node, double* bbox) {
  unsigned int i;
  if(!node->flag)
    if(!(bbox[0]<node->bbox[2] && node->bbox[0]<bbox[2] &&
         bbox[1]<node->bbox[3] && node->bbox[1]<bbox[3]))
      node->flag = 1;
  for(i=0;i<node->n;i++)
    inclLabels(&node->child[i], bbox);
}

void exclLabels(Node* node, double* bbox) {
  unsigned int i;
  if(!node->flag)
    if(bbox[0]<node->bbox[2] && node->bbox[0]<bbox[2] &&
       bbox[1]<node->bbox[3] && node->bbox[1]<bbox[3])
      node->flag = 1;
  for(i=0;i<node->n;i++)
    exclLabels(&node->child[i], bbox);
}

void treeLabels(Node* nodes, Node* node) {
  unsigned int i;
  if(!node->flag) {
    node->flag = 2;
    exclLabels(nodes, node->bbox);
  }
  for(i=0;i<node->n;i++)
    treeLabels(nodes, &node->child[i]);
}
