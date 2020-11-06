#include "calc_bg.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

#ifdef debug
#include <iostream>
#endif

using std::accumulate;
using std::pow;
using std::vector;

static double calc_mean(const vector<Pixel> &pixels)
{
    // may not be numerical stable
    // just a quick & dirty implenmentation
    double res = 0.0;
    int count = 0;
    for (const auto &pixel : pixels)
    {
        if (pixel.is_background())
        {
            res += pixel;
            count++;
        }
    }
    return res / count;
}

static double calc_std(const vector<Pixel> &pixels)
{
    auto mean_val = calc_mean(pixels);
    const auto N = static_cast<int>(pixels.size());

    double res = 0.0;
    for (int i = 0; i < N; i++)
    {
        res += pow(pixels[i] - mean_val, 2.0);
    }
    return std::sqrt(res / (N - 1));
}

int mark_not_background(vector<Pixel> &pixels, double mean_val, double std_val, int sigma)
{
    int count = 0;
    double thresh = sigma * std_val;

    for (auto &pixel : pixels)
    {
        if (pixel.is_background() && abs(pixel - mean_val) > thresh)
        {
            pixel.mark_not_background();
            count++;
        }
    }
    return count;
}

double calc_bg(const vector<Image> &images, int sigma)
{
    // first copy all images to a single vector as data points
    vector<Pixel> pixels;
    pixels.reserve(images.size() * images[0].size());

    // const auto IMAGE_SIZE = static_cast<int>(images[0].size());
    const short NROWS = images[0].rows();
    const short NCOLS = images[0].cols();

    for (int i = 0; i < (int)images.size(); i++)
    {
        for (int j = 0; j < NROWS; j++)
        {
            for(int k=0; k<NCOLS; k++){
                pixels.push_back(Pixel{images[i][j][k]});
            }
        }
    }
    /// finished copying

    while (true)
    {
        auto mean_val = calc_mean(pixels);
        auto std_val = calc_std(pixels);
        auto modified = mark_not_background(pixels, mean_val, std_val, sigma);
        if (modified == 0)
        {
            return mean_val;
        }

#ifdef debug
        std::cout << "modified " << modified << " mean " << mean_val << " std_val " << std_val << std::endl;
#endif
    }

    throw std::runtime_error("don't converge");
}