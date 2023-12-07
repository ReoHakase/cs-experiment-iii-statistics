// Calculate an average of a given set
double average(double *set, int count);

// Calculate parameters of a polynomial approximation
// @param x: x values
// @param y: y values
// @param count: number of points
// @param degree: degree of the polynomial
// @return: parameters of the polynomial
double *calc_polynomial_approximation_parameters(double *x, double *y,
                                                 int count, int degree);

// Predict a value by a polynomial approximation
// @param x: x value
// @param parameters: parameters of the polynomial approximation
// @param parameter_count: number of parameters e.g. `3` for a quadratic
// polynomial
// @return: predicted y value
double predict_by_polynomial_approximation(double x, double *parameters,
                                           int parameter_count);

// Calculate determination coefficient R2 using a given predictor
// @param x: x values
// @param y: y values
// @param point_count: number of points
// @param predictor: a function that predicts y by x and parameters
// @param parameters: parameters to be passed to the predictor
// @param parameter_count: number of parameters
double calc_determination_coefficient(double *x, double *y, int point_count,
                                      double (*predictor)(double x,
                                                          double *parameters,
                                                          int parameter_count),
                                      double *parameters, int parameter_count);

// Show parameters of a polynomial approximation
// @param parameters: parameters of the polynomial approximation
// @param degree: degree of the polynomial e.g. `2` for a quadratic polynomial
void show_polynomial_parameters(double *parameters, int degree);