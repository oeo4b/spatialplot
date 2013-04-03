#ifndef COLOR_H
#define COLOR_H

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Color;

void greycolors(Color*, unsigned int);
void heatcolors(Color*, unsigned int);
void cmcolors(Color*, unsigned int);
void terraincolors(Color*, unsigned int);
void rainbowcolors(Color*, unsigned int);

#endif
