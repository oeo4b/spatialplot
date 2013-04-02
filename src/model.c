#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "model.h"
#include "stats.h"

void fit(Features* training, Model* model, Variogram type) {
  /* Set properties */
  model->type = type;
  model->training = training;

  /* Calculate the kernel function => K(X) ~ Cov(X) */
  unsigned int i, j, k, n = (pow(training->n, 2) - training->n) / 2;
  double* cov[2];
  for(i=0;i<2;i++) 
    cov[i] = (double*)malloc(sizeof(double)*n);
  for(i=0,k=0;i<(training->n-1);i++)
    for(j=i+1;j<training->n;j++,k++) {
      cov[0][k] = pow(
        pow(training->X[0][i]-training->X[0][j], 2)+
	pow(training->X[1][i]-training->X[1][j], 2), 0.5);
      cov[1][k] = pow(training->y[i]-training->y[j], 2);
    }

  /* Transformation and regression */
  double** X = (double**)malloc(sizeof(double*)*n);
  double B[2];
  model->range = max(cov[0], n);
  for(i=0;i<n;i++) {
    X[i] = (double*)malloc(sizeof(double)*2);
    X[i][0] = 1; /* Intercept */
    switch(type) { /* Transform */
    case LINEAR:
      X[i][1] = cov[0][i];;
      break;
    case SPHERICAL:
      X[i][1] = 1.5*cov[0][i] - 0.5*pow(cov[0][i], 3) / pow(model->range, 2);
      break;
    case GAUSSIAN:
      X[i][1] = model->range - model->range*exp(-(1.0/A) * pow(cov[0][i]/model->range, 2));
      break;
    case EXPONENTIAL:
      X[i][1] = model->range - model->range*exp(-(1.0/A) * (cov[0][i]/model->range));
      break;
    }
  }
  lm(cov[1], X, B, n, 2);

  /* Assign parameters to model */
  model->nugget = B[0];
  model->sill = B[0]+model->range*B[1];

  /* Dealloc */
  free(X);
  for(i=0;i<2;i++)
    free(cov[i]);
}

void predict(Block* block, Model* model) {

}
