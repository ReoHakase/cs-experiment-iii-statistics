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

double *deviation(double *set, int count) {
  double *deviations = malloc(count * sizeof(double));
  if (deviations == NULL) {
    return NULL;  // メモリ割り当て失敗
  }

  double avg = average(set, count);
  for (int i = 0; i < count; i++) {
    deviations[i] = set[i] - avg;
  }
  return deviations;
}

double variance(double *set, int count) {
  double sumVariance = 0.0;
  double avg = average(set, count);
  for (int i = 0; i < count; i++) {
    double diff = set[i] - avg;
    sumVariance += diff * diff;
  }
  return sumVariance / count;
}

double standard_deviation(double *deviation_set, int count) {
  double sumSquared = 0.0;
  for (int i = 0; i < count; i++) {
    sumSquared += deviation_set[i] * deviation_set[i];
  }
  return sqrt(sumSquared / count);
}

double covariance(double *deviation_set_x, double *deviation_set_y, int count) {
  double sumProduct = 0.0;
  for (int i = 0; i < count; i++) {
    sumProduct += deviation_set_x[i] * deviation_set_y[i];
  }
  return sumProduct / count;
}

double correlation_coefficient(double *deviation_set_x, double *deviation_set_y,
                               int count) {
  double cov = covariance(deviation_set_x, deviation_set_y, count);
  double stdDevX = standard_deviation(deviation_set_x, count);
  double stdDevY = standard_deviation(deviation_set_y, count);
  return cov / (stdDevX * stdDevY);
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

  double avgX = average(x, dp->num);
  double avgY = average(y, dp->num);

  double *deviation_set_x = deviation(x, dp->num);
  double *deviation_set_y = deviation(y, dp->num);

  double cov = covariance(deviation_set_x, deviation_set_y, dp->num);
  double stdDevX = standard_deviation(deviation_set_x, dp->num);
  double stdDevY = standard_deviation(deviation_set_y, dp->num);
  double corr =
      correlation_coefficient(deviation_set_x, deviation_set_y, dp->num);

  free(x);
  free(y);
  free(deviation_set_x);
  free(deviation_set_y);

  // ・平均(xx̅, yy�)、標準偏差(Sx,
  // Sy)、共分散(Cxy)、相関係数(R)を標準出力に出力する。
  printf("Average of x = \t%lf\n", avgX);
  printf("Average of y = \t%lf\n", avgY);
  printf("Standard deviation of x (S_x) = \t%lf\n", stdDevX);
  printf("Standard deviation of y (S_y) = \t%lf\n", stdDevY);
  printf("Covariance of x, y (C_{x,y}) = \t%lf\n", cov);
  printf("Correlation coefficient R = \t%lf\n", corr);
}