#include "mock_data.h"
#include "api.h"

std::vector<Image> get_images(){
    const char *fname = "../data/example.fits";
    Image img = read_image_2d(fname);
    constexpr int N = 30;

    int i0 = 100, j0 = 100;
    double vi = 2, vj = 3;

    std::vector<Image> images;
    images.reserve(N);

    for (int i = 0; i < N; i++)
    {
        images.push_back(img);
    }

    for (int n = 0; n < images.size(); n++)
    {
        images[n][i0 + (int)std::round(vi * n)][j0 + (int)std::round(vj * n)] += 500;
    }

    double bg = calc_bg(images);
    for (auto &image : images)
    {
        image -= bg;
    }

    
    return images;
}