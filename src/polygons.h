#ifndef POLYGONS_H
#define POLYGONS_H

typedef struct {
  unsigned char flag;
  unsigned int n;
  double* x;
  double* y;
  double bbox[4];
} Polygon;

#endif
