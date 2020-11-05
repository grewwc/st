#include "psf.h"
#include <cmath>

double psf(double x, double y, double R, double beta)
{
    return std::pow(1 + (x * x + y * y) / (R * R), -beta);
}

