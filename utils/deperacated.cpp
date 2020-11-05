/** this file contains some code which is useful for reading & debuging purpose
 *  don't use in production code
 */

#include "lsfit.h"

#include <unordered_map>
#include <vector>
#include <cmath>

#include "psf.h"
#include "helper_types.h"
#include "constants.h"
#include <iostream>

using namespace std;

struct key_type
{
    int i0, j0;
    double vi, vj;

    key_type(int i0 = -1, int j0 = -1, double vi = -1, double vj = -1)
        : i0{i0}, j0{j0}, vi{vi}, vj{vj} {}

    bool operator==(const key_type &other) const
    {
        return i0 == other.i0 && j0 == other.j0 && (abs(vi - other.vi) < EPS) && (abs(vj - other.vj) < EPS);
    }
};

struct key_type_hasher
{
    size_t operator()(const key_type &key) const
    {
        return hash<int>()(key.i0) ^ hash<int>()(key.j0) ^ hash<double>()(key.vi) ^ hash<double>()(key.vj);
    }
};


using cache_type = unordered_map<key_type, double, key_type_hasher>;

// in principal, we don't need to lock the cache
// because there is no overlap for key_type.
static cache_type cache;

static bool contains(const key_type &key)
{
    return cache.find(key) != cache.cend();
    // return false;
}

double calc_sum_In(const vector<Image> &images)
{
    key_type key{};
    if (contains(key))
    {
        return cache[key];
    }
    double res = 0.0;
    const int Nf = images.size();
    const int rows = images[0].rows();
    const int cols = images[0].cols();
    for (int n = 0; n < Nf; n++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                res += images[n][i][j];
            }
        }
    }
    cache[key] = res;
    return res;
}

double calc_sum_In_squared(const vector<Image> &images)
{
    key_type key{-2, -2, -2, -2};
    if (contains(key))
    {
        return cache[key];
    }
    double res = 0.0;
    const int Nf = images.size();
    const int rows = images[0].rows();
    const int cols = images[0].cols();

    for (int n = 0; n < Nf; n++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                res += pow(images[n][i][j], 2);
            }
        }
    }
    cache[key] = res;
    return res;
}

double calc_sum_Pn(int Nf, int rows, int cols, int i0, int j0, double vi, double vj)
{
    key_type key{i0, j0, vi, vj};
    if (contains(key))
    {
        cout << "here" << endl;
        return cache[key];
    }
    double res = 0.0;
    for (int n = 0; n < Nf; n++)
    {
        int i0_prime = (int)round(i0 + n * vi);
        int j0_prime = (int)round(j0 + n * vj);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                res += psf(i - i0_prime, j - j0_prime, R);
            }
        }
    }
    cache[key] = res;
    return res;
}

double calc_sum_Pn_squared(int Nf, int rows, int cols, int i0, int j0, double vi, double vj)
{
    key_type key{i0, j0, vi, vj};
    if (contains(key))
    {
        return cache[key];
    }
    double res = 0.0;
    for (int n = 0; n < Nf; n++)
    {
        int i0_prime = (int)round(i0 + n * vi);
        int j0_prime = (int)round(j0 + n * vj);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                double temp = psf(i - i0_prime, j - j0_prime, R);
                res += temp * temp;
            }
        }
    }
    cache[key] = res;
    return res;
}

double calc_sum_In_Pn(const vector<Image> &images, int i0, int j0, double vi, double vj)
{
    key_type key{i0, j0, vi, vj};
    if (contains(key))
    {
        return cache[key];
    }
    const int Nf = (int)images.size();
    const int rows = (int)images[0].rows();
    const int cols = (int)images[0].cols();

    double res = 0.0;

    for (int n = 0; n < Nf; n++)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int i0_prime = (int)round(i0 + n * vi);
                int j0_prime = (int)round(j0 + n * vj);
                res += images[n][i][j] * psf(i - i0_prime, j - j0_prime, R);
            }
        }
    }
    cache[key] = res;
    return res;
}

pair<double, double> calc_alpha_I0(const vector<Image> &images, int i0, int j0, double vi, double vj)
{
    const int Nf = static_cast<int>(images.size());
    const int rows = images[0].rows();
    const int cols = images[0].cols();

    double sum_Pn = calc_sum_Pn(Nf, rows, cols, i0, j0, vi, vj);
    double sum_In = calc_sum_In(images);
    double sum_Pn_squared = calc_sum_Pn_squared(Nf, rows, cols, i0, j0, vi, vj);
    double sum_In_Pn = calc_sum_In_Pn(images, i0, j0, vi, vj);

    double numerator_I0 = sum_In * sum_Pn_squared - sum_In_Pn * sum_Pn;
    double numerator_alpha = Nf * rows * cols * sum_In_Pn - sum_Pn * sum_In;
    double dominator = Nf * rows * cols * sum_Pn_squared - sum_Pn * sum_Pn;
    // cout << numerator_alpha << endl;
    // cout << numerator_I0 << endl;
    // cout << dominator << endl;

    return {numerator_alpha / dominator, numerator_I0 / dominator};
}

double Cost(const vector<Image> &images, int i0, int j0, double vi, double vj)
{
    const int Nf = static_cast<int>(images.size());
    const int rows = images[0].rows();
    const int cols = images[0].cols();

    double sum_Pn = calc_sum_Pn(Nf, rows, cols, i0, j0, vi, vj);
    double sum_In = calc_sum_In(images);
    double sum_Pn_squared = calc_sum_Pn_squared(Nf, rows, cols, i0, j0, vi, vj);
    double sum_In_Pn = calc_sum_In_Pn(images, i0, j0, vi, vj);
    double sum_In_squared = calc_sum_In_squared(images);

    auto p = calc_alpha_I0(images, i0, j0, vi, vj);
    double alpha = get<0>(p) ;
    double I0 = get<1>(p);

    // cout << "alpha " << alpha << endl;
    // cout << "I0 " << I0 << endl;

    return sum_In_squared + alpha * alpha * sum_Pn_squared + Nf * rows * cols * I0 * I0 - 2 * sum_In_Pn * alpha - 2 * I0 * sum_In + 2 * alpha * I0 * sum_Pn;
}
