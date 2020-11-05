#include "lsfit.h"

#include <unordered_map>
#include <vector>
#include <cmath>

#include "psf.h"
#include "helper_types.h"
#include "constants.h"
#include "interpolation.h"

#include <atomic>
#include <mutex>
#include <iostream>
using namespace std;

using namespace alglib;

mutex mu;

/** use this function instead */
double Cost(const vector<Image> &images, int i0, int j0, double vi, double vj)
{
    const int Nf = static_cast<int>(images.size());
    const int rows = images[0].rows();
    const int cols = images[0].cols();

    double sum_In{0.0};
    double sum_In_squared{0.0};

    double sum_Pn_squared = 0.0;
    double sum_In_Pn = 0.0;
    double sum_Pn = 0.0;

    // do things all together
    // avoid using cache
    // possibly faster

    for (int n = 0; n < Nf; n++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int i0_prime = (int)std::round(i0 + n * vi);
                int j0_prime = (int)std::round(j0 + n * vj);
                double curr_psf = psf(i - i0_prime, j - j0_prime, R);

                sum_In += images[n][i][j];
                sum_In_squared += pow(images[n][i][j], 2);
                sum_In_Pn += images[n][i][j] * curr_psf;
                sum_Pn += curr_psf;
                sum_Pn_squared += curr_psf * curr_psf;
            }
        }
    }

    const int N = Nf * rows * cols;

    double numerator_I0 = sum_In * sum_Pn_squared - sum_In_Pn * sum_Pn;
    double numerator_alpha = N * sum_In_Pn - sum_Pn * sum_In;
    double dominator = N * sum_Pn_squared - sum_Pn * sum_Pn;

    double alpha = numerator_alpha / dominator;
    double I0 = numerator_I0 / dominator;

    // cout << "alpha " << alpha << "  I0  " << I0 << endl;
    return sum_In_squared + alpha * alpha * sum_Pn_squared + N * I0 * I0 - 2 * sum_In_Pn * alpha - 2 * I0 * sum_In + 2 * alpha * I0 * sum_Pn;
}

double Cost_alglib(const vector<Image> &images, int i0, int j0, double vi, double vj)
{
    const int Nf = static_cast<int>(images.size());
    const int rows = images[0].rows();
    const int cols = images[0].cols();
    const int N = Nf * rows * cols;
    vector<double> x_vec;
    x_vec.reserve(2 * N);
    vector<double> y_vec;
    y_vec.reserve(N);

    for (int n = 0; n < Nf; n++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int i0_prime = (int)std::round(i0 + n * vi);
                int j0_prime = (int)std::round(j0 + n * vj);
                double curr_psf = psf(i - i0_prime, j - j0_prime, R);
                y_vec.push_back(images[n][i][j]);
                x_vec.push_back(1);
                x_vec.push_back(curr_psf);
            }
        }
    }

    real_2d_array x;
    real_1d_array y;
    real_1d_array c = "[0.0, 1.0]";
    ae_int_t info;
    lsfitreport report;
    x.setcontent(x_vec.size() / 2, 2, x_vec.data());
    y.setcontent(y_vec.size(), y_vec.data());
    lsfitlinear(y, x, info, c, report);
    // cout << c[0] << "  " << c[1] << endl;

    return pow(report.rmserror, 2) * N;
}
