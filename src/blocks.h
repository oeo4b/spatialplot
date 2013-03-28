#ifndef BLOCKS_H
#define BLOCKS_H
#define CELL 256

typedef struct {
  unsigned int level;
  double depth;
  unsigned int x;
  unsigned int y;
  double xlim[2];
  double ylim[2];
  unsigned char block[65536]; /* 256 * 256 */  
} Block;

void printBlock(Block*);

#endif
