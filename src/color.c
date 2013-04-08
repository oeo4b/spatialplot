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
    spectrum[i+1].r = 255;
    spectrum[i+1].g = mg*(double)i> 255.0 ? 255 : 
      (unsigned char)floor(mg*(double)i);
    spectrum[i+1].b = mb*(double)i+bb < 0.0 ? 0 : 
      (unsigned char)floor(mb*(double)i+bb);
  }

  /* Background */
  spectrum[0].r = 255;
  spectrum[0].g = 255;
  spectrum[0].b = 255;  
}

void cmcolors(Color* spectrum, unsigned int n) {
  unsigned int i;
  double Br[2];
  Br[0] = 125.0;
  Br[1] = 240.0 / (double)n;
  double Bg[2];
  Bg[0] = 365.0;
  Bg[1] = -240.0 / (double)n;

  for(i=0;i<n;i++) {
    spectrum[i+1].r = (double)i/(double)n < 0.5 ? 
      (unsigned char)floor(Br[0]+Br[1]*(double)i) : 
      245;
    spectrum[i+1].g = (double)i/(double)n < 0.5 ? 
      245 :
      (unsigned char)floor(Bg[0]+Bg[1]*(double)i);
    spectrum[i+1].b = 245;
  }

  /* Background */
  spectrum[0].r = 255;
  spectrum[0].g = 255;
  spectrum[0].b = 255;  
}

void terraincolors(Color* spectrum, unsigned int n) {
  unsigned int i;
  double Br[2][2];
  Br[0][0] = 0.0;
  Br[0][1] = 450.0 / (double)n;
  Br[1][0] = 205.0;
  Br[1][1] = 40.0 / (double)n;
  double Bg[2][3];
  Bg[0][0] = 160.0;
  Bg[0][1] = 130.0 / (double)n;
  Bg[1][0] = 688.3;
  Bg[1][1] = -1410.0 / (double)n;
  Bg[1][2] = 966.7 / pow((double)n, 2);
  double Bb[2];
  Bb[0] = -245.0;
  Bb[1] = 490.0 / (double)n; 

  for(i=0;i<n;i++) {
    spectrum[i+1].r = (double)i/(double)n < 0.5 ? 
      (unsigned char)floor(Br[0][0]+Br[0][1]*(double)i) : 
      (unsigned char)floor(Br[1][0]+Br[1][1]*(double)i);
    spectrum[i+1].g = (double)i/(double)n < 0.5 ? 
      (unsigned char)floor(Bg[0][0]+Bg[0][1]*(double)i) : 
      (unsigned char)floor(Bg[1][0]+Bg[1][1]*(double)i+Bg[1][2]*pow((double)i, 2));
    spectrum[i+1].b = (double)i/(double)n < 0.5 ? 
      0 : 
      (unsigned char)floor(Bb[0]+Bb[1]*(double)i);
  }

  /* Background */
  spectrum[0].r = 255;
  spectrum[0].g = 255;
  spectrum[0].b = 255;  
}

