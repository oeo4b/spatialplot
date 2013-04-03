#ifndef BLOCKS_H
#define BLOCKS_H
#define CELL 256
#include "color.h"

typedef struct {
  unsigned int level;
  double depth;
  unsigned int x;
  unsigned int y;
  double xlim[2];
  double ylim[2];
  unsigned char block[CELL*CELL]; /* 256 * 256 */  
} Block;

int readBlock(Block*);
void createBlock(Block*);
void writeBlock(Block*);
void printBlock(Block*, Color*);

#endif
