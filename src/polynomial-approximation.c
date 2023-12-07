#include "polynomial-approximation.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "read-data.h"

double average(double *set, int count) {
  double sum = 0.0;
  for (int i = 0; i < count; i++) {
    sum += set[i];
  }
  return sum / count;
}

double *calc_polynomial_approximation_parameters(double *x, double *y,
                                                 int count, int degree) {
  // Allocate memory for matrix
  MAT *X = newMat(count, degree + 1);
  MAT *A = newMat(degree + 1, 1);
  MAT *Y = newMat(count, 1);
  MAT *XT = newMat(degree + 1, count);
  MAT *XTX = newMat(degree + 1, degree + 1);
  MAT *XTX_inv = newMat(degree + 1, degree + 1);
  MAT *XTX_inv_XT = newMat(degree + 1, count);

  // Set values to design matrix X
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < degree + 1; j++) {
      setMat(X, i, j, pow(x[i], j));
    }
  }

  // Set values to response vector Y
  for (int i = 0; i < count; i++) {
    setMat(Y, i, 0, y[i]);
  }

  // Calculate parameters
  transposeMatrix(X, XT);
  multiplyMatrix(XT, X, XTX);
  invertMatrix(XTX, XTX_inv);
  multiplyMatrix(XTX_inv, XT, XTX_inv_XT);
  multiplyMatrix(XTX_inv_XT, Y, A);

  // Free memory
  freeMat(X);
  freeMat(Y);
  freeMat(XT);
  freeMat(XTX);
  freeMat(XTX_inv);
  freeMat(XTX_inv_XT);

  // Extract parameters to an array
  double *parameters = malloc((degree + 1) * sizeof(double));
  if (parameters == NULL) {
    return NULL;
  }
  for (int i = 0; i < degree + 1; i++) {
    parameters[i] = getMat(A, i, 0);
  }
  freeMat(A);

  return parameters;
}

double predict_by_polynomial_approximation(double x, double *parameters,
                                           int parameter_count) {
  double sum = 0.0;
  for (int i = 0; i < parameter_count; i++) {
    sum += parameters[i] * pow(x, i);
  }
  return sum;
}

double calc_determination_coefficient(double *x, double *y, int point_count,
                                      double (*predictor)(double x,
                                                          double *parameters,
                                                          int parameter_count),
                                      double *parameters, int parameter_count) {
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
    // double approx = calc_polynomial_approximation(x[i], a, degree);
    double prediction = predictor(x[i], parameters, parameter_count);
    double diff = y[i] - prediction;
    varianceSumToApproximationY += diff * diff;
  }

  return 1.0 - varianceSumToApproximationY / varianceSumToAverageY;
}

void show_polynomial_parameters(double *parameters, int degree) {
  printf("y = ");
  for (int i = degree; i >= 0; i--) {
    // Use ANSI escape code to change color
    printf("\033[36m");
    printf("%lf", parameters[i]);  // cyan
    printf("\033[0m");
    printf("\033[37m");
    if (i > 1) {
      printf("x^%d", i);  // gray
    } else if (i == 1) {
      printf("x");  // gray
    }
    printf("\033[0m");
    if (i != 0) {
      printf(" + ");
    }
  }
  printf("\n");
}

int main(int ac, char *av[]) {
  FILE *fp;
  Data *dp;
  if ((fp = fopen(av[1], "r")) == NULL) {
    fputs("readData: error 1\n", stderr);
    exit(EXIT_FAILURE);
  }
  dp = readData(fp);

  char *dimensionAttribute = av[2];
  int degree = atoi(dimensionAttribute);
  if (degree < 1) {
    fputs("Degree must be positive\n", stderr);
    exit(EXIT_FAILURE);
  }

  printf("データ数:\t %d\n", dp->num);
  printf("次数:\t %d\n", degree);

  double *x = extractX(dp);
  double *y = extractY(dp);

  double *parameters =
      calc_polynomial_approximation_parameters(x, y, dp->num, degree);
  printf("多項式近似: \t");
  show_polynomial_parameters(parameters, degree);

  double r2 = calc_determination_coefficient(
      x, y, dp->num, predict_by_polynomial_approximation, parameters,
      degree + 1);
  printf("決定係数 R^2:\t %lf\n", r2);

  free(x);
  free(y);
}