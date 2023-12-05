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

// Polynomial approximation
double *calc_approximation_coefficient(double *x, double *y, int count,
                                       int degree) {
  // allocate memory for matrix
  MAT *X = newMat(count, degree + 1);
  MAT *A = newMat(degree + 1, 1);
  MAT *Y = newMat(count, 1);
  MAT *XT = newMat(degree + 1, count);
  MAT *XTX = newMat(degree + 1, degree + 1);
  MAT *XTX_inv = newMat(degree + 1, degree + 1);
  MAT *XTX_inv_XT = newMat(degree + 1, count);

  // set values to matrix
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < degree + 1; j++) {
      setMat(X, i, j, pow(x[i], j));
    }
  }

  for (int i = 0; i < count; i++) {
    setMat(Y, i, 0, y[i]);
  }

  // calculate coefficients
  // printf("\nY:\n");
  // printMat(Y);
  // printf("\nX:\n");
  // printMat(X);

  transposeMatrix(X, XT);
  // printf("\nXT:\n");
  // printMat(XT);

  multiplyMatrix(XT, X, XTX);
  // printf("\nXTX:\n");
  // printMat(XTX);

  invertMatrix(XTX, XTX_inv);
  // printf("\nXTX_inv:\n");
  // printMat(XTX_inv);

  multiplyMatrix(XTX_inv, XT, XTX_inv_XT);
  // printf("\nXTX_inv_XT:\n");
  // printMat(XTX_inv_XT);

  multiplyMatrix(XTX_inv_XT, Y, A);
  // printf("\nA:\n");
  // printMat(A);

  // free memory
  freeMat(X);
  freeMat(Y);
  freeMat(XT);
  freeMat(XTX);
  freeMat(XTX_inv);
  freeMat(XTX_inv_XT);

  // extract coefficients to an array
  double *coefficients = malloc((degree + 1) * sizeof(double));
  if (coefficients == NULL) {
    return NULL;  // メモリ割り当て失敗
  }

  for (int i = 0; i < degree + 1; i++) {
    coefficients[i] = getMat(A, i, 0);
  }

  freeMat(A);

  return coefficients;
}

double calc_polynomial_approximation(double x, double *a, int degree) {
  double sum = 0.0;
  for (int i = 0; i < degree + 1; i++) {
    sum += a[i] * pow(x, i);
  }
  return sum;
}

// R2
double calc_determination_coefficient(double *x, double *y, int count,
                                      double *a, int degree) {
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
    double approx = calc_polynomial_approximation(x[i], a, degree);
    double diff = y[i] - approx;
    varianceSumToApproximationY += diff * diff;
  }

  return 1.0 - varianceSumToApproximationY / varianceSumToAverageY;
}

void show_coefficients(double *coefficients, int degree) {
  printf("多項式近似:\t y = ");
  for (int i = degree; i >= 0; i--) {
    // Use ANSI escape code to change color
    printf("\033[36m");
    printf("%lf", coefficients[i]);  // cyan
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
    fputs("degree must be positive\n", stderr);
    exit(EXIT_FAILURE);
  }

  printf("データ数:\t %d\n", dp->num);
  printf("次数:\t %d\n", degree);

  double *x = extractX(dp);
  double *y = extractY(dp);

  double *coefficients = calc_approximation_coefficient(x, y, dp->num, degree);
  show_coefficients(coefficients, degree);

  double r2 =
      calc_determination_coefficient(x, y, dp->num, coefficients, degree);
  printf("決定係数 R^2:\t %lf\n", r2);

  free(x);
  free(y);
}