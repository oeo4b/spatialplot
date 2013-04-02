#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nodes.h"
#include "blocks.h"

void treeNodes(Node* node, FILE*** filestreams, unsigned int level) {
  unsigned int i;
  if(level>0) {
    fread(&node->n, 4, 1, filestreams[level-1][0]);
    fread(&node->nchar, 4, 1, filestreams[level-1][1]);
    node->name = (unsigned char*)malloc(sizeof(unsigned char)*(node->nchar+1));
    fread(node->name, 1, node->nchar+1, filestreams[level-1][2]);
    fread(node->xy, 8, 2, filestreams[level-1][3]);
    fread(&node->npoly, 4, 1, filestreams[level-1][4]);
    if(node->npoly>0) {
      node->polygons = (Polygon*)malloc(sizeof(Polygon)*node->npoly);
      for(i=0;i<node->npoly;i++) {
        fread(&node->polygons[i].n, 4, 1, filestreams[level-1][5]);
        fread(node->polygons[i].bbox, 8, 4, filestreams[level-1][6]);
        node->polygons[i].x = (double*)malloc(sizeof(double)*node->polygons[i].n);
        node->polygons[i].y = (double*)malloc(sizeof(double)*node->polygons[i].n);
        fread(node->polygons[i].x, 8, node->polygons[i].n, filestreams[level-1][7]);
        fread(node->polygons[i].y, 8, node->polygons[i].n, filestreams[level-1][7]);
      }
    }
  }
  if(node->n>0) {
    node->child = (Node*)malloc(sizeof(Node)*node->n);
    for(i=0;i<node->n;i++)
      treeNodes(&node->child[i], filestreams, level+1);
  }
}

void readNodes(Node* root, char** file, unsigned int n) {
  unsigned int i, j, k;
  char filename[100];

  /* File stream pointers */
  char* type[] = { "n", "nchar", "name", "xy", "npoly", "poly.n", "poly.bbox", "poly.xy" };
  FILE*** nodef = (FILE***)malloc(sizeof(FILE**)*n);
  for(i=0;i<n;i++) {
    nodef[i] = (FILE**)malloc(sizeof(FILE*)*8);
    for(j=0;j<8;j++) {
      sprintf(filename, "features/%s.%s", file[i], type[j]);
      nodef[i][j] = fopen(filename, "rb");
    }
  }

  /* Get n and alloc */
  fseek(nodef[0][0], 0, SEEK_END);
  root->n = ftell(nodef[0][0])/4;
  root->child = (Node*)malloc(sizeof(Node)*root->n);
  fseek(nodef[0][0], 0, SEEK_SET);

  /* Iterate down tree */
  root->nchar = 0;
  root->npoly = 0;
  root->xy[0] = 0;
  root->xy[1] = 0;
  treeNodes(root, nodef, 0);

  /* Close stream and dealloc */
  for(i=0;i<n;i++)
    for(j=0;j<4;j++)
      fclose(nodef[i][j]);
  free(nodef);
}

void ytrans(double* y) {
  *y = (180.0/3.14159265359) * log(tan(3.14159265359*(0.25 + *y/360.0)));
}

void ytransNodes(Node* node) { /* Latitude transformation - Mercator projection */
  unsigned int i, j;
  ytrans(&node->xy[1]);
  ytrans(&node->bbox[1]);
  ytrans(&node->bbox[3]);
  for(i=0;i<node->npoly;i++) {
    ytrans(&node->polygons[i].bbox[1]);
    ytrans(&node->polygons[i].bbox[3]);
    for(j=0;j<node->polygons[i].n;j++)
      ytrans(&node->polygons[i].y[j]);
  }
  for(i=0;i<node->n;i++)
    ytransNodes(&node->child[i]);
}

void printNodes(Node* node) {
  unsigned int i;
  printf("%s (%f, %f)\n", node->name, node->xy[0], node->xy[1]);
  for(i=0;i<node->n;i++)
    printNodes(&node->child[i]);
}

int thresholdChildNodes(Node* parent, Block* block, double threshold) {
  unsigned int i, j, n;
  double area = pow(block->depth, 2)*threshold;
  double sum = 0;
  for(i=0,n=0;i<parent->n;i++)
    for(j=0;j<parent->child[i].npoly;j++,n++)
      sum += ((parent->child[i].polygons[j].bbox[2]-
               parent->child[i].polygons[j].bbox[0])*
              (parent->child[i].polygons[j].bbox[3]-
               parent->child[i].polygons[j].bbox[1]));
  if(n==0) return 1;
  if((sum/(double)n)<area) return 1;
  return 0;
}

void freeNodes(Node* node) {

}
