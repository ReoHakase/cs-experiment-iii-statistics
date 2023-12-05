#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

static void errorMat(char *str) {
  perror(str);
  exit(EXIT_FAILURE);
}

MAT *newMat(unsigned sizeX, unsigned sizeY) {
  MAT *new;
  if ((new = (MAT *)malloc(sizeof(MAT))) == NULL)
    errorMat("newMat: no more memory");
  new->x = sizeX;
  new->y = sizeY;
  if ((new->v = (double *)calloc(sizeX * sizeY, sizeof(double))) == NULL)
    errorMat("newMat: too large");
  return new;
}

void freeMat(MAT *mat) {
  free(mat->v);
  free(mat);
}

double getMat(MAT *mat, unsigned x, unsigned y) {
  if (x >= mat->x) errorMat("getMat: x is out of range");
  if (y >= mat->y) errorMat("getMat: y is out of range");
  return (mat->v[x * mat->y + y]);
}

void setMat(MAT *mat, unsigned x, unsigned y, double val) {
  if (x >= mat->x) errorMat("setMat: x is out of range");
  if (y >= mat->y) errorMat("setMat: y is out of range");
  mat->v[x * mat->y + y] = val;
}

void printMat(MAT *mat) {
  int x, y;
  for (x = 0; x < mat->x; x++) {
    for (y = 0; y < mat->y; y++) printf(" %lf", getMat(mat, x, y));
    putchar('\n');
  }
}

// This function copies a submatrix from one matrix into another matrix. It
// copies the submatrix of src starting at row srcTop and column srcLeft into
// the submatrix of dst starting at row dstTop and column dstLeft. It copies
// rows rows and columns columns. If the source matrix is too small, then just
// pad the destination matrix with zeros If the destionation matrix is too
// small, then truncate the source matrix
void copyMatrix(MAT *src, unsigned srcTop, unsigned srcLeft, MAT *dst,
                unsigned dstTop, unsigned dstLeft, unsigned rows,
                unsigned columns) {
  // Copy elements from src to dst
  for (unsigned i = 0; i < rows; i++) {
    for (unsigned j = 0; j < columns; j++) {
      // If the source element is out of range, then just pad the destination
      // element with zero
      if (srcTop + i >= src->x || srcLeft + j >= src->y) {
        setMat(dst, dstTop + i, dstLeft + j, 0);
        continue;
      }

      // If the destionation matrix is too small, then truncate the source
      // matrix
      if (dstTop + i >= dst->x || dstLeft + j >= dst->y) {
        continue;
      }

      setMat(dst, dstTop + i, dstLeft + j,
             getMat(src, srcTop + i, srcLeft + j));
    }
  }
}

//* Negate all elements of a matrix
void negateMatrix(MAT *src, MAT *dst) {
  for (unsigned i = 0; i < src->x; i++) {
    for (unsigned j = 0; j < src->y; j++) {
      setMat(dst, i, j, -getMat(src, i, j));
    }
  }
}

//* Add two matrices (C = A + B)
void addMatrix(MAT *matA, MAT *matB, MAT *matC) {
  for (unsigned i = 0; i < matA->x; i++) {
    for (unsigned j = 0; j < matA->y; j++) {
      setMat(matC, i, j, getMat(matA, i, j) + getMat(matB, i, j));
    }
  }
}

void multiplyMatrix(MAT *matA, MAT *matB, MAT *matC) {
  // Let m to be the number of rows of matrix A
  unsigned m = matA->x;
  // Let p to be the number of columns of matrix A and the number of rows of
  // matrix B
  unsigned p = matA->y;
  if (p != matB->x) {
    printf(
        "Error: The number of columns of matrix A must be equal to the number "
        "of rows of matrix B.\n");
    exit(1);
  }
  // Let n to be the number of columns of matrix B
  unsigned n = matB->y;

  // Simply calculate the product of A and B
  for (unsigned i = 0; i < m; i++) {
    for (unsigned j = 0; j < n; j++) {
      // C_{i, j} = \sum_{k = 1}^{p} A_{i, k} * B_{k, j}
      double sum = 0;
      for (unsigned k = 0; k < p; k++) {
        sum += getMat(matA, i, k) * getMat(matB, k, j);
      }
      setMat(matC, i, j, sum);
    }
  }
}

void invertMatrix(MAT *src, MAT *dst) {
  // Check if the source matrix is square
  if (src->x != src->y) {
    fprintf(stderr, "invertMatrix: Source matrix is not square.\n");
    return;
  }
  unsigned size = src->x;
  // Reduce calculation error
  double determinant = getMatrixDeterminant(src);
  // printf("Determinant: %lf\n", determinant);
  if (determinant == 0.0) {
    fprintf(stderr, "invertMatrix: Matrix is singular.\n");
    return;
  }

  // Calculate the adjugate matrix
  MAT *adjugate = newMat(size, size);
  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j < size; j++) {
      // Calculate the cofactor
      MAT *cofactor = newMat(size - 1, size - 1);
      for (unsigned k = 0; k < size; k++) {
        for (unsigned l = 0; l < size; l++) {
          if (k == i || l == j) {
            continue;
          }
          unsigned row = (k < i) ? k : k - 1;
          unsigned col = (l < j) ? l : l - 1;
          setMat(cofactor, row, col, getMat(src, k, l));
        }
      }
      double cofactorValue = getMatrixDeterminant(cofactor);
      if ((i + j) % 2 == 1) {
        cofactorValue *= -1;
      }
      freeMat(cofactor);
      setMat(adjugate, j, i, cofactorValue);
    }
  }

  // Calculate the inverse matrix
  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j < size; j++) {
      setMat(dst, i, j, getMat(adjugate, i, j) / determinant);
    }
  }

  freeMat(adjugate);
}

double getMatrixDeterminant(MAT *mat) {
  if (mat->x != mat->y) {
    fprintf(stderr, "getMatrixDeterminant: Matrix is not square.\n");
    return 0.0;
  }

  unsigned size = mat->x;
  MAT *temp = newMat(size, size);  // 拡大行列を作成

  // srcをtempにコピー
  copyMatrix(mat, 0, 0, temp, 0, 0, size, size);

  // 行列式を計算
  double determinant = 1.0;
  for (unsigned i = 0; i < size; i++) {
    // 対角成分が0の場合、対角成分以外の行を探して入れ替える
    if (getMat(temp, i, i) == 0.0) {
      unsigned row = i + 1;
      while (row < size && getMat(temp, row, i) == 0.0) {
        row++;
      }
      if (row == size) {
        // すべての行が0の場合、行列式は0
        freeMat(temp);
        return 0.0;
      }
      // 行を入れ替える
      for (unsigned col = 0; col < size; col++) {
        double tempValue = getMat(temp, i, col);
        setMat(temp, i, col, getMat(temp, row, col));
        setMat(temp, row, col, tempValue);
      }
      // 行を入れ替えたので行列式の符号を反転
      determinant *= -1;
    }

    // 対角成分を1にする
    double diagonalValue = getMat(temp, i, i);
    for (unsigned col = 0; col < size; col++) {
      setMat(temp, i, col, getMat(temp, i, col) / diagonalValue);
    }
    determinant *= diagonalValue;

    // 対角成分以外を0にする
    for (unsigned row = 0; row < size; row++) {
      if (row == i) {
        continue;
      }
      double tempValue = getMat(temp, row, i);
      for (unsigned col = 0; col < size; col++) {
        setMat(temp, row, col,
               getMat(temp, row, col) - getMat(temp, i, col) * tempValue);
      }
    }
  }

  freeMat(temp);
  return determinant;
}

void transposeMatrix(MAT *src, MAT *dst) {
  // Assert dimensions
  if (src->x != dst->y || src->y != dst->x) {
    fprintf(stderr, "transposeMatrix: Dimensions of matrices do not match.\n");
    return;
  }

  for (unsigned row = 0; row < src->x; row++) {
    for (unsigned col = 0; col < src->y; col++) {
      setMat(dst, col, row, getMat(src, row, col));
    }
  }
}