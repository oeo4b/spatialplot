#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "color.h"

void greycolors(Color* spectrum, unsigned int n) {
  unsigned int i, j;
  for(i=0;i<n;i++) {
    spectrum[i].r = i;
    spectrum[i].g = i;
    spectrum[i].b = i;
  }
}

void heatcolors(Color* spectrum, unsigned int n) {
  unsigned int i;
  double mg = 250.0/((double)n*0.7);
  double mb = 250.0/((double)n*0.1);
  double bb = 255.0 - mb*n;
  for(i=0;i<n;i++) {
    spectrum[i].r = 255;
    spectrum[i].g = mg*(double)i> 255 ? 255 : 
      (unsigned char)floor(mg*(double)i);
    spectrum[i].b = mb*(double)i+bb < 0 ? 0 : 
      (unsigned char)floor(mb*(double)i+bb);
  }

  /* Background */
  spectrum[0].r = 255;
  spectrum[0].g = 255;
  spectrum[0].b = 255;  
}

void cmcolors(Color* spectrum, unsigned int n) {

}

void terraincolors(Color* spectrum, unsigned int n) {

}

void rainbowcolors(Color* spectrum, unsigned int n) {

}
