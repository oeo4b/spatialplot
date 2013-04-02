#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "stats.h"
#include "maths.h"

double min(double* x, int n) {
  int i;
  double y = x[0];
  for(i=1;i<n;i++)
    if(x[i]<y) y = x[i];
  return y;
}

double max(double* x, int n) {
  int i;
  double y = x[0];
  for(i=1;i<n;i++)
    if(x[i]>y) y = x[i];
  return y;
}

/* Discrete distributions */
int rbinom(int n, double p) {

}

int rpois(double np) {

}

/* Continous distributions */
double runif(double a, double b) {
  double u;
  if(a==b) return a;
  else {
    do {
      u = (double)rand() / (double)RAND_MAX;
    } while(u<0 || u>1);
    return a + (b-a)*u;
  }
}

double rexp(double b) {

}

double rgamma(double a, double b) {

}

double rchisq(double df) {

}

double rbeta(double a, double b) {

}

double rnorm(double mu, double sigma) { /* Box-muller method */
  double U = runif(0.0, 1.0);
  double V = runif(0.0, 1.0);
  double Z = sqrt(-2.0*log(U)) * cos(2.0*PI*V);
  return mu + sigma*Z;
}

double rcauch(double theta) {

}

/* Regression */
void lm(double* Y, double** X, double* B, unsigned int n, unsigned int p) {
  /* (p x n) * (p x n) */
  unsigned int i, j;
  double** Xt = (double**)malloc(sizeof(double*)*p);
  double** XtX = (double**)malloc(sizeof(double*)*p);
  double** XtXinvXt = (double**)malloc(sizeof(double*)*p);
  double** XtXinv;
  for(i=0;i<p;i++) {
    Xt[i] = (double*)malloc(sizeof(double)*n);
    XtX[i] = (double*)malloc(sizeof(double)*p);
    XtXinvXt[i] = (double*)malloc(sizeof(double)*n);
  }

  /*  (X'X)^-1*X'y */
  t(X, Xt, n, p);
  mmult(Xt, X, XtX, p, n, p);
  solve(XtX, p);
  XtXinv = XtX;
  mmult(XtXinv, Xt, XtXinvXt, p, p, n);
  mvmult(XtXinvXt, Y, B, p, n);

  /* Dealloc */
  free(Xt);
  free(XtX);
  free(XtXinvXt);
}
