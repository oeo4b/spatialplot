#include <stdlib.h>
#include <stdio.h>
#include "blocks.h"

void printBlock(Block* block) {
  int i, j;
  printf("P6\n%d %d\n255\n", CELL, CELL);
  for(j=(CELL-1);j>=0;j--)
    for(i=0;i<CELL;i++)
      printf("%c%c%c", block->block[i*CELL+j], block->block[i*CELL+j], block->block[i*CELL+j]);
}
