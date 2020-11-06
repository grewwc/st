#include <iostream>
#include <string>
#include "mock_data.h"

#include <future>

using namespace std;

void test_st()
{
    auto images = get_images();
    auto res = fit(images, {90, 110, 21}, {90, 110, 21}, {1.5, 2.5, 10}, {1.5, 4.5, 20});
    cout << res.i0 << "  " << res.j0 << "  " << res.vi << "  " << res.vj << endl;
}


void test_st_multi_core()
{
    vector<Image> images = get_images();
    FittingSpace x {90, 110, 10};
    FittingSpace y {90, 110, 10};
    FittingSpace vx {1.5, 2.5, 10};
    FittingSpace vy {2.5, 3.5, 10};
    cout << "\tthe sample image is: ./data/example.fits\n";
    cout << "\tIn the example, there are 30 images in total\n";
    cout << "\tAdd dots in these images (the function is: get_images, the value is: 500 )\n";
    cout << "\tParameters are listed below (the origin is upper-left)\n";
    cout << "\t\t X0: 100" << endl;
    cout << "\t\t Y0: 100" << endl;
    cout << "\t\t Vx: 2.0 pixel/s" << endl;
    cout << "\t\t Vy: 3.0 pixel/s" << endl;
    auto res = fit_multi(images, x, y, vx, vy, 2, true);
    cout << res << endl;
}