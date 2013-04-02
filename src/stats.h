#ifndef STATS_H
#define STATS_H

double min(double*, int);
double max(double*, int);

/* Discrete distributions */
int rbinom(int, double);
int rpois(double);

/* Continous distributions */
double runif(double, double);
double rexp(double);
double rgamma(double, double);
double rchisq(double);
double rbeta(double, double);
double rnorm(double, double);
double rcauch(double);

/* Regression */
void lm(double*, double**, double*, unsigned int, unsigned int);

#endif
