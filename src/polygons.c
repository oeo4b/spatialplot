#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "polygons.h"
#include "stats.h"

void polygonTree(
  Polygon* p, Block* block, unsigned char hole,
  unsigned int x1, unsigned int x2,
  unsigned int y1, unsigned int y2,
  unsigned int b1, unsigned int b2,
  unsigned int t1, unsigned int t2
) {
  unsigned int i, j;
  unsigned int a = x2-x1+1;
  unsigned int b = y2-y1+1;
  unsigned int boxed = 0;
  if(!b1)
    if(pip(p->n, p->x, p->y,
           block->xlim[0]+block->depth*(double)x1/(double)CELL,
           block->ylim[0]+block->depth*(double)y1/(double)CELL)) b1++;
  if(!t1)
    if(pip(p->n, p->x, p->y,
           block->xlim[0]+block->depth*(double)x1/(double)CELL,
           block->ylim[0]+block->depth*(double)y2/(double)CELL)) b1++;
  if(!b2)
    if(pip(p->n, p->x, p->y,
           block->xlim[0]+block->depth*(double)x2/(double)CELL,
           block->ylim[0]+block->depth*(double)y1/(double)CELL)) b1++;
  if(!t2)
    if(pip(p->n, p->x, p->y,
           block->xlim[0]+block->depth*(double)x2/(double)CELL,
           block->ylim[0]+block->depth*(double)y2/(double)CELL)) b1++;
  boxed = bip(p->n, p->x, p->y,
              block->xlim[0]+block->depth*(double)x1/(double)CELL,
              block->xlim[0]+block->depth*(double)x2/(double)CELL,
              block->ylim[0]+block->depth*(double)y1/(double)CELL,
              block->ylim[0]+block->depth*(double)y2/(double)CELL);
  if(boxed && !(b1|b2|t1|t2)) {
    return;
  }
  if(boxed && (b1&b2&t1&t2)) {
    for(i=x1;i<=x2;i++)
      for(j=y1;j<=y2;j++)
        if(hole) block->block[i*CELL+j] = 0;
        else     block->block[i*CELL+j] = 1;
  }
  else {
    if(a>=4 && b>=4) {
      for(i=0;i<2;i++)
        for(j=0;j<2;j++)
          polygonTree(p, block, hole,
                    x1+(unsigned int)floor((double)(i*a)/2.0),
                    x1+(unsigned int)floor((double)((i+1)*a)/2.0)-1,
                    y1+(unsigned int)floor((double)(j*b)/2.0),
                    y1+(unsigned int)floor((double)((j+1)*b)/2.0)-1,
                    (i==0&&j==0) ? b1 : 0,
                    (i==1&&j==0) ? b2 : 0,
                    (i==0&&j==1) ? t1 : 0,
                    (i==1&&j==1) ? t2 : 0);
    }
    else {
      for(i=x1;i<=x2;i++)
        for(j=y1;j<=y2;j++) {
          if(pip(p->n, p->x, p->y,
                 block->xlim[0]+block->depth*(double)i/(double)CELL,
                 block->ylim[0]+block->depth*(double)j/(double)CELL))
            if(hole) block->block[i*CELL+j] = 0;
            else     block->block[i*CELL+j] = 1;
        }
    }
  }
}

void polygonReduce(Polygon* polygon, Block* block) {
  unsigned int i, j, k;
  for(i=0;i<polygon->n;i++) {
    polygon->x[i] = (-180.0)+block->depth*floor((double)CELL*(polygon->x[i]-(-180.0))/block->depth)/(double)CELL;
    polygon->y[i] = (-90.0)+block->depth*floor((double)CELL*(polygon->y[i]-(-90.0))/block->depth)/(double)CELL;
  }
  for(i=1,j=0;i<polygon->n;i++) {
    if(polygon->x[j]==polygon->x[i] && polygon->y[j]==polygon->y[i]) continue;
    j++;
    polygon->x[j] = polygon->x[i];
    polygon->y[j] = polygon->y[i];
  }
  polygon->n = j+1;
  unsigned char* x = (unsigned char*)calloc(polygon->n, sizeof(unsigned char));
  unsigned char* y = (unsigned char*)calloc(polygon->n, sizeof(unsigned char));
  for(i=0;i<polygon->n;i++) {
    if(polygon->x[i]>=block->xlim[0]) x[i]++;
    if(polygon->x[i]>block->xlim[1]) x[i]++;
    if(polygon->y[i]>=block->ylim[0]) y[i]++;
    if(polygon->y[i]>block->ylim[1]) y[i]++;
  }
  i = 0;
  j = 0;
  while(i<polygon->n) {
    polygon->x[j] = polygon->x[i];
    polygon->y[j] = polygon->y[i];
    i++; j++;
    if(x[i]==x[i-1] && y[i]==y[i-1] && i<polygon->n) {
      if(x[i]==1 && y[i]==1) {
        while(x[i]==x[i-1] && y[i]==y[i-1] && i<polygon->n) {
          polygon->x[j] = polygon->x[i];
          polygon->y[j] = polygon->y[i];
          i++; j++;
        }
      }
      else {
        while(x[i]==x[i-1] && y[i]==y[i-1] && i<polygon->n) {
          polygon->x[j] = polygon->x[i];
          polygon->y[j] = polygon->y[i];
          i++;
        }
        j++;
      }
    }
  }
  polygon->n = j;
  polygon->bbox[0] = min(polygon->x, polygon->n);
  polygon->bbox[1] = min(polygon->y, polygon->n);
  polygon->bbox[2] = max(polygon->x, polygon->n);
  polygon->bbox[3] = max(polygon->y, polygon->n);
  free(x);
  free(y);
}

int pip(int n, double* X, double* Y, double x, double y) {
  int i, j, c = 0;
  for(i=0,j=n-1;i<n;j=i++) {
    if( ((Y[i]>y)!=(Y[j]>y)) && (x<(X[j]-X[i])*(y-Y[i])/(Y[j]-Y[i])+X[i]) )
      c = !c;
  }
  return c;
}

int bip(int n, double* X, double* Y, double x1, double x2, double y1, double y2) {
  int i;
  double mid1 = (x1+x2)/2.0;
  double mid2 = (y1+y2)/2.0;
  for(i=0;i<n;i++) {
    if( abs(mid1-X[i])<((x2-x1)/2.0) && abs(mid2-Y[i])<((y2-y1)/2.0) )
      return 0;
  }
  return 1;
}
