#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "maths.h"

void printmatrix(double** X, unsigned int rows, unsigned int cols) {
  unsigned int i, j;
  for(i=0;i<rows;i++) {
    for(j=0;j<cols;j++) {
      printf("%f", X[i][j]);
      if(j<(cols-1)) printf(", ");
    }
    printf("\n");
  }
}

void printvector(double* X, unsigned int rows, unsigned int cols) {
  unsigned int i, j, k;
  for(i=0,k=0;i<rows;i++) {
    for(j=0;j<cols;j++,k++) {
      printf("%f", X[k]);
      if(j<(cols-1)) printf(", ");
    }
    printf("\n");
  }
}

void t(double** X, double** Xt, unsigned int rows, unsigned int cols) {
  unsigned int i, j;
  for(i=0;i<rows;i++)
    for(j=0;j<cols;j++)
      Xt[j][i] = X[i][j];
}

void mmult(double** X, double** Y, double** XY, unsigned int n, unsigned int m, unsigned int p) {
  unsigned int i, j, k, l;
  for(i=0;i<n;i++)
    for(j=0;j<p;j++) {
      XY[i][j] = 0;
      for(k=0;k<m;k++)
        XY[i][j] += X[i][k]*Y[k][j];
    }
}

inline void vmult(double* X, double* Y, double* z, int n) {
  int i = 0;
  *z = 0;
  if(n>25)
    while(i<=(n-25)) {
      (*z) += (X[i] * Y[i] +
               X[i+1]*Y[i+1]+
               X[i+2]*Y[i+2]+
               X[i+3]*Y[i+3]+
               X[i+4]*Y[i+4]+
               X[i+5]*Y[i+5]+
               X[i+6]*Y[i+6]+
               X[i+7]*Y[i+7]+
               X[i+8]*Y[i+8]+
               X[i+9]*Y[i+9]+
               X[i+10]*Y[i+10]+
               X[i+11]*Y[i+11]+
               X[i+12]*Y[i+12]+
               X[i+13]*Y[i+13]+
               X[i+14]*Y[i+14]+
               X[i+15]*Y[i+15]+
               X[i+16]*Y[i+16]+
               X[i+17]*Y[i+17]+
               X[i+18]*Y[i+18]+
               X[i+19]*Y[i+19]+
               X[i+20]*Y[i+20]+
               X[i+21]*Y[i+21]+
               X[i+22]*Y[i+22]+
               X[i+23]*Y[i+23]+
               X[i+24]*Y[i+24]);
      i += 25;
    }

  while(i<n) {
    (*z) += X[i]*Y[i];
    i++;
  }
}

inline void mvmult(double** X, double* Y, double* Z, unsigned int n, unsigned int p) {
  unsigned int i;
  for(i=0;i<n;i++)
    vmult(X[i], Y, &Z[i], p);
}

int solve(double** X, unsigned int n) {
  int m = n;
  double b[n][n];
  int indxc[n];
  int indxr[n];
  int ipiv [n];
  int i, icol, irow, j, k, l, ll;
  double big, dum, pivinv, temp;

  for(i=0;i<n;i++)
    for(j=0;j<n;j++) {
      if(i==j) b[i][j] = 1;
      else b[i][j] = 0;
    }
  for(j=0;j<n;j++) ipiv[j] = 0;
  for(i=0;i<n;i++) {
    big = 0;
    for(j=0;j<n;j++)
      if(ipiv[j]!=1)
        for(k=0;k<n;k++)
          if(ipiv[k]==0)
            if(fabs(X[j][k])>=big) {
              big = fabs(X[j][k]);
              irow = j;
              icol = k;
            }
    ++(ipiv[icol]);
    if(irow!=icol) {
      for(l=0;l<n;l++) {
        temp = X[irow][l];
        X[irow][l] = X[icol][l];
        X[icol][l] = temp;
      }
      for(l=0;l<m;l++) {
        temp = b[irow][l];
        b[irow][l] = b[icol][l];
        b[icol][l] = temp;
      }
    }
    indxr[i] = irow;
    indxc[i] = icol;

    if(X[icol][icol]==0) /* Singular matrix */
      return 0;

    pivinv = (double)1 / X[icol][icol];
    X[icol][icol] = 1;
    for(l=0;l<n;l++) X[icol][l] *= pivinv;
    for(l=0;l<m;l++) b[icol][l] *= pivinv;

    for(ll=0;ll<n;ll++)
      if(ll!=icol) {
        dum = X[ll][icol];
        X[ll][icol] = 0;
        for(l=0;l<n;l++) X[ll][l] -= X[icol][l]*dum;
        for(l=0;l<m;l++) b[ll][l] -= b[icol][l]*dum;
      }
  }

  for(l=(n-1);l>=0;l--)
    if(indxr[l]!=indxc[l])
      for(k=0;k<n;k++) {
        temp = X[k][indxr[l]];
        X[k][indxr[l]] = X[k][indxc[l]];
        X[k][indxc[l]] = temp;
      }
  return 1;
}
