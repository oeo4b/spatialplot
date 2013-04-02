#ifndef MODEL_H
#define MODEL_H
#include "blocks.h"

#define A 0.33333333333

typedef struct {
  unsigned int n;
  double* X[2];
  double* y;
} Features;

typedef enum {
  LINEAR,
  SPHERICAL,
  GAUSSIAN,
  EXPONENTIAL
} Variogram;

typedef struct {
  Features* training;
  Variogram type;
  double nugget;
  double range;
  double sill;
} Model;

double linear(double, double, double, double);
double spherical(double, double, double, double);
double gaussian(double, double, double, double);
double exponential(double, double, double, double);
void fit(Features*, Model*, Variogram);
void predict(Block*, Model*);

#endif
