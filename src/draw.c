#include "draw.h"
#include "blocks.h"
#include "nodes.h"
#include "polygons.h"

void drawLine(
  double** line, Block* block, 
  Linetype lty, unsigned char lwd, Color color
) {

}

void drawPolygon(
  Polygon* polygon, Block* block, 
  Linetype lty, unsigned char lwd, Color color
) { 

}

void drawNodes(Node* root, Block* block) {
  unsigned int i;
  for(i=0;i<node->n;i++)
    drawNodes(&node->child[i], block);
}
