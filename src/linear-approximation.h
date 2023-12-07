// Calculate an average of a given set
double average(double *set, int count);

// Calculate parameters of a linear approximation
// @param x: x values
// @param y: y values
// @param count: number of points
double *calc_linear_approximation_parameters(double *x, double *y, int count);

// Calculate determination coefficient R2 of a linear approximation
// @param x: x values
// @param y: y values
// @param point_count: number of points
// @param parameters: parameters to be passed to the predictor
double calc_determination_coefficient(double *x, double *y, int point_count,
                                      double *parameters);

// Show parameters of a linear approximation
// @param parameters: parameters of the linear approximation
void show_linear_parameters(double *parameters);