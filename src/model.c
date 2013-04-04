#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "model.h"
#include "stats.h"

double linear(double h, Model* self) {
  return h>self->range ? self->sill :
    self->nugget + (self->sill-self->nugget)*
    ( h/self->range );
}

double spherical(double h, Model* self) {
  return h>self->range ? self->sill :
    self->nugget + (self->sill-self->nugget)*
    ( 1.5*(h/self->range) - 0.5*pow(h/self->range, 3) );
}

double gaussian(double h, Model* self) {
  return 
    self->nugget + ((self->sill-self->nugget)/self->range)*
    ( 1.0 - exp(-(1.0/A) * pow(h/self->range, 2)) );
}

double exponential(double h, Model* self) {
  return 
    self->nugget + ((self->sill-self->nugget)/self->range)*
    ( 1.0 - exp(-(1.0/A) * (h/self->range)) );
}

void fit(Features* training, Model* model, Variogram type) {
  /* Set properties */
  switch(type) {
  case LINEAR:
    model->kernel = &linear;
    break;
  case SPHERICAL:
    model->kernel = &spherical;
    break;
  case GAUSSIAN:
    model->kernel = &gaussian;
    break;
  case EXPONENTIAL:
    model->kernel = &exponential;
    break;
  }
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
      X[i][1] = cov[0][i];
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

void predict(Block* block, Model* model, unsigned int groups) {
  unsigned int i, j, k;
  unsigned int n = model->training->n;
  double** var = (double**)malloc(sizeof(double*)*(n+1));
  for(i=0;i<=n;i++) {
    var[i] = (double*)malloc(sizeof(double)*(n+1));
    for(j=0;j<=n;j++) {
      if(i==n&&j==n) var[i][j] = 0;
      else if(i==n||j==n) var[i][j] = 1;
      else
        var[i][j] = model->kernel(pow(
	  pow(model->training->X[0][i]-model->training->X[0][j], 2)+
	  pow(model->training->X[1][i]-model->training->X[1][j], 2), 0.5), 
	  model);
    }
  }
  solve(var, n+1);
  double** invvar = var;

  /* W = Var^-1 * Cov */
  double x, y, z;
  double zmin = min(model->training->y, n);
  double zmax = max(model->training->y, n);
  double zrange = zmax-zmin;
  double* cov = (double*)malloc(sizeof(double)*(n+1));
  cov[n] = 1;
  double* w = (double*)malloc(sizeof(double)*(n+1));
  for(i=0;i<CELL;i++)
    for(j=0;j<CELL;j++) {
      if(block->block[i*CELL+j]==0) continue;
      x = block->xlim[0] + block->depth*(double)i/(double)CELL;
      y = block->ylim[0] + block->depth*(double)j/(double)CELL;
      for(k=0;k<n;k++)
        cov[k] = model->kernel(pow(
	  pow(model->training->X[0][k]-x, 2)+
	  pow(model->training->X[1][k]-y, 2), 0.5), 
	  model);
      mvmult(invvar, cov, w, n+1, n+1);
      vmult(w, model->training->y, &z, n);
      z = (z-zmin)/zrange; /* Standardize estimate */
      if(z<0) z = 0;
      if(z>1) z = 1;
      block->block[i*CELL+j] = 1+floor(z*(groups-1)+0.5);
    }

  /* Dealloc */
  free(invvar);
  free(cov);
  free(w);
}
