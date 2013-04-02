/*
 * spatialplot 
 * Copyright 2013
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nodes.h"
#include "blocks.h"

void readFeatures(unsigned int n, double** features, FILE* stream) {
  unsigned int i, j;
  char point[100];
  for(i=0;i<3;i++)
    for(j=0;j<n;j++) {
      fgets(point, 100, stream); 
      features[i][j] = atof(point);
    }
}

void help(void) {
  printf("usage: spatialplot [--version] [--help] <command> [<args>]\n\n"
         "Commands:\n"
         "  layer        Create map block layer\n"
	 "  interpolate  Interpolate spatial data and project to block\n");
}

void layer(int argc, char** argv) {
  if(argc<5) {
    printf("You must specify the block properties.\n\n"
           "usage: spatialplot layer <level> <x> <y>\n");
    return;
  }

  /* Block properties */
  Block block;
  block.level = atoi(argv[2]);
  block.depth = 90.0 / pow(2, block.level);
  block.x = atoi(argv[3]);
  block.y = atoi(argv[4]);
  block.xlim[0] = (-180.0)+(double)block.x*block.depth; 
  block.xlim[1] = (-180.0)+(double)(block.x+1)*block.depth;
  block.ylim[0] = (-180.0)+(double)block.y*block.depth;
  block.ylim[1] = (-180.0)+(double)(block.y+1)*block.depth;

  /* Zero block */
  unsigned int i, j;
  for(i=0;i<CELL;i++)
    for(j=0;j<CELL;j++)
      block.block[i*CELL+j] = 255;
  
  /* Read nodes */
  char* file[] = { "countries", "states", "cities" };
  Node root;
  readNodes(&root, file, 3);
  ytransNodes(&root);
  initText(4);
  bindLabels(&root, &block, 10);

  /* Draw polygons to block */
  drawPolygons(&root, &block, 0);

  /* Omit label overlaps and draw to block */
  treeLabels(&root, &root);
  drawLabels(&root, &block, 0);

  /* Print to stdout */
  printBlock(&block);
}

void interpolate(int argc, char** argv) {
  if(argc<6) {
    printf("You must specify the block properties.\n\n"
           "usage: spatialplot interpolate <level> <x> <y> <n> [<model>]\n");
    return;
  }

  /* Block properties */
  Block block;
  block.level = atoi(argv[2]);
  block.depth = 90.0 / pow(2, block.level);
  block.x = atoi(argv[3]);
  block.y = atoi(argv[4]);
  block.xlim[0] = (-180.0)+(double)block.x*block.depth; 
  block.xlim[1] = (-180.0)+(double)(block.x+1)*block.depth;
  block.ylim[0] = (-180.0)+(double)block.y*block.depth;
  block.ylim[1] = (-180.0)+(double)(block.y+1)*block.depth;

  /* Zero block */
  int i, j;
  for(i=0;i<CELL;i++)
    for(j=0;j<CELL;j++)
      block.block[i*CELL+j] = 0;

  /* Read in features */
  unsigned int n = atoi(argv[5]);
  double* features[3];
  for(i=0;i<3;i++)
    features[i] = (double*)malloc(sizeof(double)*n);
  readFeatures(n, features, stdin);
  
  /* Model fitting */

  /* Read/write block */
  if(readBlock(&block)) {
    createBlock(&block);
    writeBlock(&block);
  }

  /* Prediction */

  /* Print to stdout */
  printBlock(&block);

  /* Dealloc */
  for(i=0;i<3;i++)
    free(features[i]);
}

int main(int argc, char** argv) {
  /* Version */
  int major = 1;
  int minor = 0;
  int build = 1;

  if(argc<2)
    help();
  else if(!strcmp(argv[1], "--version"))
    printf("spatialplot version %d.%d.%d\n", major, minor, build);
  else if(!strcmp(argv[1], "--help"))
    help();
  else if(!strcmp(argv[1], "layer"))
    layer(argc, argv);
  else if(!strcmp(argv[1], "interpolate"))
    interpolate(argc, argv);
  else
    printf("git: '%s' is not a spatialplot command. See 'spatialplot --help'.\n", argv[1]);

  return 0;
}
