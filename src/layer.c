/**
 * layer
 * Usage: level x y [options]
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "nodes.h"
#include "blocks.h"

const char path[] = "/";

int main(int argc, char** argv) {
  if(argc<4) return 1;
  /* Block properties */
  Block block;
  block.level = atoi(argv[1]);
  block.depth = 90.0 / pow(2, block.level);
  block.x = atoi(argv[2]);
  block.y = atoi(argv[3]);
  block.xlim[0] = (-180.0)+(double)block.x*block.depth; 
  block.xlim[1] = (-180.0)+(double)(block.x+1)*block.depth;
  block.ylim[0] = (-180.0)+(double)block.y*block.depth;
  block.ylim[1] = (-180.0)+(double)(block.y+1)*block.depth;
  
  /* Read nodes */
  Node root;
  readNodes(&root);

  /* Draw nodes to block */
  drawNodes(&root, &block);

  /* Print to stdout */
  printBlock(&block);

  return 0;
}
