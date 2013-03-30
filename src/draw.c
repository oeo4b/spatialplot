#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "draw.h"
#include "blocks.h"
#include "nodes.h"
#include "polygons.h"

static unsigned int dashed = 0;

/* Freetype globals */
FT_Library library;
FT_Face face;

void initText(unsigned int size) {
  int error;
  error = FT_Init_FreeType(&library);
  if(error) return;
  error = FT_New_Face(library, "/Library/Fonts/Arial.ttf", 0, &face);
  if(error==FT_Err_Unknown_File_Format) return;
  else if(error) return;
}

void drawLine(
  double* X, double* Y, Block* block, 
  Linetype lty, unsigned char lwd, Color color
) {
  int x0 = floor((double)CELL*(X[0]-block->xlim[0]) / block->depth);
  int x1 = floor((double)CELL*(X[1]-block->xlim[0]) / block->depth);
  int y0 = floor((double)CELL*(Y[0]-block->ylim[0]) / block->depth);
  int y1 = floor((double)CELL*(Y[1]-block->ylim[0]) / block->depth);
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;

  for(;;){
    if(x0>=0 && x0<CELL && x1>=0 && x1<CELL &&
       y0>=0 && y0<CELL && y1>=0 && y1<CELL) {
      if(lty==DASHED && (dashed%15)<7 )
        block->block[x0*CELL+y0] = color;
      else if(lty==DOTTED && dashed%2 )
        block->block[x0*CELL+y0] = color;
      else if(lty==SOLID)
        block->block[x0*CELL+y0] = color;
      dashed++;
    }
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

void drawPolygon(
  Polygon* polygon, Block* block, 
  Linetype lty, unsigned char lwd, Color color
) { 
  unsigned int i, j;
  double x[2];
  double y[2];
  for(i=0;i<(polygon->n-1);i++)
    drawLine(&polygon->x[i], &polygon->y[i], block, lty, lwd, color);
  x[0] = polygon->x[polygon->n-1];
  x[1] = polygon->x[0];
  y[0] = polygon->y[polygon->n-1];
  y[1] = polygon->y[0];
  drawLine(x, y, block, lty, lwd, color);
}

void drawPolygons(Node* node, Block* block, unsigned int level) {
  unsigned int i, j = 0;
  if(level>0 && node->n>0)
    j = thresholdChildNodes(node, block, 0.01);
  if(!j) { /* Conditionally include descendants */
    for(i=0;i<node->n;i++)
      drawPolygons(&node->child[i], block, level+1);
  }
  for(i=0;i<node->npoly;i++)
    if(block->xlim[0]<node->polygons[i].bbox[2] && 
       node->polygons[i].bbox[0]<block->xlim[1] &&
       block->ylim[0]<node->polygons[i].bbox[3] && 
       node->polygons[i].bbox[1]<block->ylim[1]) {
      if(level==1)
        drawPolygon(&node->polygons[i], block, SOLID, 1, 50);
      if(level==2)
        drawPolygon(&node->polygons[i], block, DASHED, 1, 200);
    }
}


void drawText(double x, double y, unsigned char* text, unsigned int n, Block* block) {

}

void drawLabels(Node* node, Block* block, unsigned int level) {
  unsigned int i;
  if(node->flag!=1) {
    if(block->xlim[0]<node->bbox[2] && 
       node->bbox[0]<block->xlim[1] &&
       block->ylim[0]<node->bbox[3] && 
       node->bbox[1]<block->ylim[1])
      drawText(node->xy[0], node->xy[1], node->name, node->nchar, block);
  }
  for(i=0;i<node->n;i++)
    drawLabels(&node->child[i], block, level+1);
}

int strwidth(unsigned char* text, unsigned int n) {
  return 1;
}

int strheight(unsigned char* text, unsigned int n) {
  return 1;
}
