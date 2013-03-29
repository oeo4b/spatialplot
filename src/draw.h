#ifndef DRAW_H
#define DRAW_H
#include "polygons.h"
#include "nodes.h"
#include "blocks.h"

typedef enum {
  SOLID,
  DASHED,
  DOTTED
} Linetype;

typedef unsigned char Color;

void drawLine(double*, double*, Block*, Linetype, unsigned char, Color);
void drawPolygon(Polygon*, Block*, Linetype, unsigned char, Color);
void drawNodes(Node*, Block*, unsigned int);

#endif
