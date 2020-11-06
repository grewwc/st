#pragma once 

#include <vector>
#include <utility>
#include "helper_types.h"


// double calc_sum_In(const std::vector<Image> &images);

// double calc_sum_Pn(int Nf, int rows, int cols, int i0, int j0, double vi, double vj);

// double calc_sum_Pn_squared(int Nf, int rows, int cols, int i0, int j0, double vi, double vj);

// double calc_sum_In_Pn(const std::vector<Image> &images, int i0, int j0, double vi, double vj);

// std::pair<double, double> calc_alpha_I0(const std::vector<Image> &images, int i0, int j0, double vi, double vj);

// double Cost(const std::vector<Image> &images, int i0, int j0, double vi, double vj);

double Cost(const std::vector<Image> &images, int i0, int j0, double vi, double vj);



double Cost_alglib(const std::vector<Image> &images, int i0, int j0, double vi, double vj);
