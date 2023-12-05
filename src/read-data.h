#include <stdio.h>

typedef struct {
  double x;
  double y;
} Datum;

typedef struct {
  unsigned num;
  Datum *data;
} Data;

Data *readData(FILE *fp);
double *extractX(Data *dp);
double *extractY(Data *dp);