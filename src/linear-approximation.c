#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "read-data.h"

double average(double *set, int count) {
  double sum = 0.0;
  for (int i = 0; i < count; i++) {
    sum += set[i];
  }
  return sum / count;
}

// Linear approximation
double *calc_approximation_coefficient(double *x, double *y, int count) {
  double *coefficients = malloc(2 * sizeof(double));
  if (coefficients == NULL) {
    return NULL;  // メモリ割り当て失敗
  }

  double sumX = 0.0;
  double sumY = 0.0;
  double sumXY = 0.0;
  double sumXX = 0.0;
  for (int i = 0; i < count; i++) {
    sumX += x[i];
    sumY += y[i];
    sumXY += x[i] * y[i];
    sumXX += x[i] * x[i];
  }

  coefficients[0] =
      (count * sumXY - sumX * sumY) / (count * sumXX - sumX * sumX);
  coefficients[1] =
      (sumXX * sumY - sumX * sumXY) / (count * sumXX - sumX * sumX);
  return coefficients;
}

// R2
double calc_determination_coefficient(double *x, double *y, int count, double a,
                                      double b) {
  // Σ (y_i - y_bar)^2
  double varianceSumToAverageY = 0.0;
  double avgY = average(y, count);
  for (int i = 0; i < count; i++) {
    double diff = y[i] - avgY;
    varianceSumToAverageY += diff * diff;
  }

  // Σ (y_i - f_i)^2
  double varianceSumToApproximationY = 0.0;
  for (int i = 0; i < count; i++) {
    double approx = a * x[i] + b;
    double diff = y[i] - approx;
    varianceSumToApproximationY += diff * diff;
  }

  return 1.0 - varianceSumToApproximationY / varianceSumToAverageY;
}

int main(int ac, char *av[]) {
  FILE *fp;
  Data *dp;
  if ((fp = fopen(av[1], "r")) == NULL) {
    fputs("readData: error 1\n", stderr);
    exit(EXIT_FAILURE);
  }
  dp = readData(fp);

  double *x = extractX(dp);
  double *y = extractY(dp);

  double *coefficients = calc_approximation_coefficient(x, y, dp->num);
  double a = coefficients[0];
  double b = coefficients[1];

  double r2 = calc_determination_coefficient(x, y, dp->num, a, b);

  printf("線形回帰: y = %lf x + %lf\n", a, b);
  printf("決定係数: R^2 = %lf\n", r2);

  free(x);
  free(y);
}