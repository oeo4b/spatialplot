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

void initText(unsigned int, const char*);
void drawLine(double*, double*, Block*, Linetype, unsigned char, unsigned char);
void drawPolygon(Polygon*, Block*, Linetype, unsigned char, unsigned char);
void drawPolygons(Node*, Block*, unsigned int);
void drawText(double, double, unsigned long*, unsigned int, Block*);
void drawLabels(Node*, Block*, unsigned int);

int strwidth(unsigned char*, unsigned int);
int strheight(unsigned char*, unsigned int);

#endif
