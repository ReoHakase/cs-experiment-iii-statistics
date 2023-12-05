typedef struct {
  unsigned x;  // 行数 rows
  unsigned y;  // 列数 columns
  double *v;
} MAT;

MAT *newMat(unsigned sizeX, unsigned sizeY);
void freeMat(MAT *mat);

double getMat(MAT *mat, unsigned x, unsigned y);
void setMat(MAT *mat, unsigned x, unsigned y, double val);

// This function copies a submatrix from one matrix into another matrix. It
// copies the submatrix of src starting at row srcTop and column srcLeft into
// the submatrix of dst starting at row dstTop and column dstLeft. It copies
// rows rows and columns columns. If the source matrix is too small, then just
// pad the destination matrix with zeros If the destionation matrix is too
// small, then truncate the source matrix
void copyMatrix(MAT *src, unsigned srcTop, unsigned srcLeft, MAT *dst,
                unsigned dstTop, unsigned dstLeft, unsigned rows,
                unsigned columns);

void printMat(MAT *mat);

void negateMatrix(MAT *src, MAT *dst);
void addMatrix(MAT *matA, MAT *matB, MAT *matC);
void multiplyMatrix(MAT *matA, MAT *matB, MAT *matC);
void invertMatrix(MAT *src, MAT *dst);
void transposeMatrix(MAT *src, MAT *dst);
double getMatrixDeterminant(MAT *mat);