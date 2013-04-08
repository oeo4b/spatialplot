/*
 * spatialplot 
 * Copyright 2013
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nodes.h"
#include "blocks.h"
#include "model.h"
#include "color.h"

void readFeatures(Features* features, FILE* stream) {
  unsigned int i;
  char point[100];
  for(i=0;i<features->n;i++) {
    fgets(point, 100, stream);
    features->X[0][i] = atof(point);
  }
  for(i=0;i<features->n;i++) {
    fgets(point, 100, stream);
    features->X[1][i] = atof(point);
  }
  for(i=0;i<features->n;i++) {
    fgets(point, 100, stream);
    features->y[i] = atof(point);
  }
}

void help(void) {
  printf("Usage: spatialplot [--version] [--help] <command> [<args>]\n\n"
         "Commands:\n"
         "  layer        Create map block layer\n"
	 "  interpolate  Interpolate spatial data and project to block\n");
}

void layer(int argc, char** argv) {
  if(argc<5) {
    printf("You must specify the block properties.\n\n"
           "Usage: spatialplot layer <level> <x> <y>\n");
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
  initText(4, "Arial");
  bindLabels(&root, &block, 10);

  /* Draw polygons to block */
  drawPolygons(&root, &block, 0);

  /* Omit label overlaps and draw to block */
  treeLabels(&root, &root);
  drawLabels(&root, &block, 0);

  /* Print to stdout */
  Color* spectrum = (Color*)malloc(sizeof(Color)*256);
  greycolors(spectrum, 256);
  printBlock(&block, spectrum);

  /* Dealloc */
  free(spectrum);
}

void interpolate(int argc, char** argv) {
  if(argc<6) {
    printf("You must specify the block properties.\n\n"
           "Usage: spatialplot interpolate <level> <x> <y> <n> [<options>]\n\n"
           "Options:\n"
           "  --model=TYPE     Specify the variogram model TYPE; possible values\n"
           "                   are 'linear', 'spherical', 'gaussian' or\n"
           "                   'exponential'(default)\n"
           "  --color=PALETTE  Choose from a color PALETTE; possible values are\n"
           "                   'grey'(default), 'heat', 'terrain' or 'cm'\n"
           "  --categ=VALUES   For categorical variables, specify the total number\n"
           "                   of possible VALUES\n");
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

  /* Read/write block */
  if(readBlock(&block)) {
    createBlock(&block);
    writeBlock(&block);
  }

  /* Read in features */
  Features features;
  features.n = atoi(argv[5]);
  features.y = (double*)malloc(sizeof(double)*features.n);
  for(i=0;i<2;i++)
    features.X[i] = (double*)malloc(sizeof(double)*features.n);
  readFeatures(&features, stdin);
  
  /* Model fitting */
  Model model;
  Variogram variogram = EXPONENTIAL;
  for(i=6;i<argc;i++)
    if(!strcmp(argv[i], "--model")&&(i+1)<argc) {
      if(!strcmp(argv[i+1], "linear")) variogram = LINEAR;
      if(!strcmp(argv[i+1], "spherical")) variogram = SPHERICAL;
      if(!strcmp(argv[i+1], "gaussian")) variogram = GAUSSIAN;
      if(!strcmp(argv[i+1], "exponential")) variogram = EXPONENTIAL;
    }
  fit(&features, &model, variogram);

  /* Model prediction */
  unsigned int categories = 255;
  for(i=6;i<argc;i++)
    if(!strcmp(argv[i], "--categ")&&(i+1)<argc)
      categories = atoi(argv[i+1]);
  predict(&block, &model, categories);

  /* Print to stdout */
  Color* spectrum = (Color*)malloc(sizeof(Color)*(categories+1));
  void (*palette)(Color*, unsigned int) = &greycolors;
  for(i=6;i<argc;i++)
    if(!strcmp(argv[i], "--color")&&(i+1)<argc) {
      if(!strcmp(argv[i+1], "grey")) palette = &greycolors;
      if(!strcmp(argv[i+1], "heat")) palette = &heatcolors;
      if(!strcmp(argv[i+1], "terrain")) palette = &terraincolors;
      if(!strcmp(argv[i+1], "cm")) palette = &cmcolors;
    }
  palette(spectrum, categories);
  printBlock(&block, spectrum);

  /* Dealloc */
  free(spectrum);
}

int main(int argc, char** argv) {
  /* Version */
  int major = 1;
  int minor = 1;
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
    printf("spatialplot: '%s' is not a spatialplot command. See 'spatialplot --help'.\n", argv[1]);

  return 0;
}
