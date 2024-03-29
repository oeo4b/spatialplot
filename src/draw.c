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

void initText(unsigned int size, const char* font) {
  int error;
  char text[100];
  error = FT_Init_FreeType(&library);
  if(error) return;
#ifdef __unix
  sprintf(text, "/usr/openwin/lib/X11/fonts/TrueType/%s.ttf", font);
#elif __linux__
  sprintf(text, "/usr/openwin/lib/X11/fonts/TrueType/%s.ttf", font);
#elif __APPLE__
  sprintf(text, "/Library/Fonts/%s.ttf", font);
#endif
  error = FT_New_Face(library, text, 0, &face);
  if(error) return;
  error = FT_Set_Char_Size(face, 0, size*64, CELL, CELL);   
  if(error) return;
  return;
}

void drawLine(
  double* X, double* Y, Block* block, 
  Linetype lty, unsigned char lwd, unsigned char color
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
  Linetype lty, unsigned char lwd, unsigned char color
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
  node->flag = 0;
}

void drawText(double x, double y, unsigned long* text, unsigned int n, Block* block) {
  int i, j, k, error, X, Y;
  int originx = floor((double)CELL*(x-block->xlim[0])/block->depth);
  int originy = floor((double)CELL*(y-block->ylim[0])/block->depth);
  FT_UInt glyph_index;

  for(i=0,j=0;i<n;i++) {
    glyph_index = FT_Get_Char_Index(face, text[i]);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if(error) return;
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if(error) return;
    j += face->glyph->metrics.horiAdvance/64;
  }
  originx -= j/2;

  for(i=0;i<n;i++) {
    glyph_index = FT_Get_Char_Index(face, text[i]);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if(error) return;
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if(error) return;
    for(j=0;j<face->glyph->bitmap.rows;j++)
      for(k=0;k<face->glyph->bitmap.width;k++) {
        X = originx+face->glyph->metrics.horiBearingX/64+k;
        Y = originy+face->glyph->metrics.horiBearingY/64-j;
        if(X>=0 && X<CELL && Y>=0 && Y<CELL) {
          block->block[X*CELL+Y] = 255 - face->glyph->bitmap.buffer[j*face->glyph->bitmap.width+k];
	}
      }
    originx += face->glyph->metrics.horiAdvance/64;
  }
}

void drawLabels(Node* node, Block* block, unsigned int level) {
  unsigned int i;
  static unsigned long chartolong[100];
  if(node->flag!=1) {
    if(block->xlim[0]<node->bbox[2] && 
       node->bbox[0]<block->xlim[1] &&
       block->ylim[0]<node->bbox[3] && 
       node->bbox[1]<block->ylim[1]) {
      for(i=0;i<node->nchar;i++)
        chartolong[i] = (unsigned long)node->name[i];
      drawText(node->xy[0], node->xy[1], chartolong, node->nchar, block);
    }
  }
  for(i=0;i<node->n;i++)
    drawLabels(&node->child[i], block, level+1);
}

int strwidth(unsigned char* text, unsigned int n) {
  int i, j, error;
  FT_UInt glyph_index;
  for(i=0,j=0;i<n;i++) {
    glyph_index = FT_Get_Char_Index(face, text[i]);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if(error) return 0;
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if(error) return 0;
    j += face->glyph->metrics.horiAdvance/64;
  }
  return j;
}

int strheight(unsigned char* text, unsigned int n) {
  int i, j, error;
  FT_UInt glyph_index;
  for(i=0,j=0;i<n;i++) {
    glyph_index = FT_Get_Char_Index(face, text[i]);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if(error) return 0;
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if(error) return 0;
    if((face->glyph->metrics.horiBearingY/64) > j) 
      j = face->glyph->metrics.horiBearingY/64;
  }
  return j;
}
