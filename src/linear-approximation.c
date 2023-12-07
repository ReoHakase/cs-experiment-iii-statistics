#include "linear-approximation.h"

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

double *calc_linear_approximation_parameters(double *x, double *y, int count) {
  double *parameters = malloc(2 * sizeof(double));
  if (parameters == NULL) {
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

  parameters[1] = (count * sumXY - sumX * sumY) / (count * sumXX - sumX * sumX);
  parameters[0] = (sumXX * sumY - sumX * sumXY) / (count * sumXX - sumX * sumX);
  return parameters;
}

double calc_determination_coefficient(double *x, double *y, int point_count,
                                      double *parameters) {
  // Σ (y_i - y_bar)^2
  double varianceSumToAverageY = 0.0;
  double avgY = average(y, point_count);
  for (int i = 0; i < point_count; i++) {
    double diff = y[i] - avgY;
    varianceSumToAverageY += diff * diff;
  }

  // Σ (y_i - f_i)^2
  double varianceSumToApproximationY = 0.0;
  for (int i = 0; i < point_count; i++) {
    double prediction = parameters[1] * x[i] + parameters[0];
    double diff = y[i] - prediction;
    varianceSumToApproximationY += diff * diff;
  }

  return 1.0 - varianceSumToApproximationY / varianceSumToAverageY;
}

void show_linear_parameters(double *parameters) {
  printf("y = \033[36m%lf\033[37mx + \033[36m%lf\033[0m\n", parameters[1],
         parameters[0]);
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

  double *parameters = calc_linear_approximation_parameters(x, y, dp->num);

  double r2 = calc_determination_coefficient(x, y, dp->num, parameters);

  printf("線形回帰: \t");
  show_linear_parameters(parameters);
  printf("決定係数: \t R^2 = %lf\n", r2);

  free(x);
  free(y);
}