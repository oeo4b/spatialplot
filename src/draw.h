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
void drawPolygons(Node*, Block*, unsigned int);
void drawText(double, double, unsigned char*, unsigned int, Block*);
void drawLabels(Node*, Block*, unsigned int);

int strwidth(unsigned char*, unsigned int);
int strheight(unsigned char*, unsigned int);

#endif
