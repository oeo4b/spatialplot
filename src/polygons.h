#ifndef POLYGONS_H
#define POLYGONS_H
#include "blocks.h"

typedef struct {
  unsigned char flag;
  unsigned int n;
  double* x;
  double* y;
  double bbox[4];
} Polygon;


void polygonTree(
  Polygon*, Block*, unsigned char,
  unsigned int, unsigned int,
  unsigned int, unsigned int,
  unsigned int, unsigned int,
  unsigned int, unsigned int
);
void polygonReduce(Polygon*, Block*);
int pip(int, double*, double*, double, double);
int bip(int, double*, double*, double, double, double, double);

#endif
