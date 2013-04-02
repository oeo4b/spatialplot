#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "blocks.h"
#include "nodes.h"
#include "polygons.h"

int readBlock(Block* block) {
  /* Read block from path */
  FILE* f;
  char filename[100];
  sprintf(filename, "grid/l%u_d%u_%u_%u.grd", block->level, CELL, block->x, block->y);
  if(access(filename, F_OK)==(-1))
    return 1;
  f = fopen(filename, "rb");

  /* Copy to char */
  unsigned int i, j;
  unsigned char header;
  unsigned int bytes = (unsigned int)ceil((double)(CELL*CELL)/8.0);
  unsigned char* bitblock = (unsigned char*)calloc(bytes, sizeof(unsigned char));
  fread(&header, sizeof(unsigned char), 1, f);
  if(header==1||header==2) {
    fread(bitblock, sizeof(unsigned char), bytes, f);
    for(i=0;i<CELL;i++)
      for(j=0;j<CELL;j++)
        if(bitblock[(unsigned int)floor((double)(i*CELL+j)/8.0)] & (1<<(j%8)))
          block->block[i*CELL+j]++;
  }
  if(header==3)
    for(i=0;i<CELL;i++)
      for(j=0;j<CELL;j++)
        block->block[i*CELL+j]++;

  /* Close stream */
  fclose(f);

  /* Dealloc */
  free(bitblock);

  return 0;
}

void createBlock(Block* block) {
  /* Alloc and read */
  unsigned int i, j, k, l, b1, b2, t1, t2;
  double xstd[2], ystd[2];
  
  /* Read nodes */
  char* file[] = { "countries", "states", "cities" };
  Node root;
  readNodes(&root, file, 3);
  ytransNodes(&root);

  /* Iterate through first level */
  for(i=0;i<root.n;i++)
    for(j=0;j<root.child[i].npoly;j++) {
      if(block->xlim[0]<root.child[i].polygons[j].bbox[2] &&
         root.child[i].polygons[j].bbox[0]<block->xlim[1] &&
         block->ylim[0]<root.child[i].polygons[j].bbox[3] &&
	 root.child[i].polygons[j].bbox[1]<block->ylim[1]) {

        /* Reduce polygon resolution to grid level */
        polygonReduce(&root.child[i].polygons[j], block);
        if(root.child[i].polygons[j].n<3) continue;

        /* Recursive blocking */
        xstd[0] = (double)CELL*(root.child[i].polygons[j].bbox[0]-block->xlim[0])/block->depth;
        xstd[1] = (double)CELL*(root.child[i].polygons[j].bbox[2]-block->xlim[0])/block->depth;
        ystd[0] = (double)CELL*(root.child[i].polygons[j].bbox[1]-block->ylim[0])/block->depth;
        ystd[1] = (double)CELL*(root.child[i].polygons[j].bbox[3]-block->ylim[0])/block->depth;
        if(xstd[0]<0) xstd[0] = 0;
        if(xstd[1]>=(double)CELL) xstd[1] = (double)CELL-1;
        if(ystd[0]<0) ystd[0] = 0;
        if(ystd[1]>=(double)CELL) ystd[1] = (double)CELL-1;
        polygonTree(
	  &root.child[i].polygons[j], block, 0,
          floor(xstd[0]), ceil(xstd[1]), floor(ystd[0]), floor(ystd[1]),
       	  pip(root.child[i].polygons[j].n,
       	      root.child[i].polygons[j].x,
       	      root.child[i].polygons[j].y,
       	      block->xlim[0]+block->depth*(double)floor(xstd[0])/(double)CELL,
       	      block->ylim[0]+block->depth*(double)floor(ystd[0])/(double)CELL),
          pip(root.child[i].polygons[j].n,
       	      root.child[i].polygons[j].x,
       	      root.child[i].polygons[j].y,
       	      block->xlim[0]+block->depth*(double)ceil(xstd[1])/(double)CELL,
       	      block->ylim[0]+block->depth*(double)floor(ystd[0])/(double)CELL),
       	  pip(root.child[i].polygons[j].n,
       	      root.child[i].polygons[j].x,
       	      root.child[i].polygons[j].y,
              block->xlim[0]+block->depth*(double)floor(xstd[0])/(double)CELL,
       	      block->ylim[0]+block->depth*(double)ceil(ystd[1])/(double)CELL),
       	  pip(root.child[i].polygons[j].n,
       	      root.child[i].polygons[j].x,
       	      root.child[i].polygons[j].y,
       	      block->xlim[0]+block->depth*(double)ceil(xstd[1])/(double)CELL,
       	      block->ylim[0]+block->depth*(double)ceil(ystd[1])/(double)CELL));
      }
    }

  /* Dealloc */
  freeNodes(&root);
}

void writeBlock(Block* block) {
  /* Write block to path */
  unsigned int i, j;
  FILE* f;
  char filename[100];
  unsigned char header = 0;
  unsigned int bytes = (unsigned int)ceil((double)(CELL*CELL)/8.0);
  unsigned char* bitblock = (unsigned char*)calloc(bytes, sizeof(unsigned char));

  /* Set header */
  unsigned int count = 0;
  for(i=0;i<CELL;i++)
    for(j=0;j<CELL;j++)
      if(block->block[i*CELL+j])
        count++;
  if(count>0) header++;
  if(count==(CELL*CELL)) header = 3;

  /* Open stream */
  sprintf(filename, "grid/l%d_d%d_%d_%d.grd", block->level, CELL, block->x, block->y);
  f = fopen(filename, "wb");
  fwrite(&header, sizeof(unsigned char), 1, f);
  if(header==1||header==2) {
    for(i=0;i<CELL;i++)
      for(j=0;j<CELL;j++)
        if(block->block[i*CELL+j])
          bitblock[(unsigned int)floor((double)(i*CELL+j)/8.0)] |= (1<<(j%8));
    fwrite(bitblock, sizeof(unsigned char), bytes, f);
  }

  /* Close stream */
  fclose(f);

  /* Dealloc */
  free(bitblock);
}

void printBlock(Block* block) {
  int i, j;
  printf("P6\n%d %d\n255\n", CELL, CELL);
  for(j=(CELL-1);j>=0;j--)
    for(i=0;i<CELL;i++)
      printf("%c%c%c", block->block[i*CELL+j], block->block[i*CELL+j], block->block[i*CELL+j]);
}
