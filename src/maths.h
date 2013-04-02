#ifndef MATHS_H
#define MATHS_H

#define PI 3.14159265359

void t(double**, double**, unsigned int, unsigned int);
void mmult(double**, double**, double**, unsigned int, unsigned int, unsigned int);
void mvmult(double**, double*, double*, unsigned int, unsigned int);
void vmult(double*, double*, double*, int);
int solve(double**, unsigned int);
void printmatrix(double**, unsigned int, unsigned int);

#endif

