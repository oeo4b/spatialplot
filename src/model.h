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

typedef struct Model {
  Features* training;
  Variogram type;
  double nugget;
  double range;
  double sill;
  double (*kernel)(double, struct Model*);
} Model;

double linear(double, Model*);
double spherical(double, Model*);
double gaussian(double, Model*);
double exponential(double, Model*);
void fit(Features*, Model*, Variogram);
void predict(Block*, Model*, unsigned int);

#endif
