#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <string>

#include "fitsio.h"

#include "fits_io.h"
#include "api.h"
#include "numeric_utils.h"
#include "normal_io.h"
#include "lsfit.h"
#include "test_api.h"

#include "stb_image_write.h"

#include "mock_data.h"
using namespace std;

void write_images(const vector<Image> &images)
{
    const char *first = "../data/first.fits";
    const char *last = "../data/last.fits";
    const char *combined = "../data/combined.fits";
    const char *sample_name = "../data/example.fits";
    cout << "Writing First Image: " << first << endl;
    cout << "Writing First Image: " << last << endl;
    images[0].save(first, sample_name, false);
    images[images.size() - 1].save(last, sample_name, false);

    auto combined_image = add_all_images(images, 2, 3);
    cout << "combined images: " << combined << endl;
    combined_image.save(combined, sample_name, false);
    cout << "finished" << endl;
}

void run()
{
    char ch = '\0';
    auto images = get_images();
    while (true)
    {
        cout << "\n----------------------" << endl;
        cout << "Choices\n";
        cout << "----------------------" << endl;
        cout << "1. Fit Images" << endl;
        cout << "2. Generate sample images" << endl;
        cout << "3. Quit\n"
             << endl;
        ch = (char)getchar();
        getchar();
        switch (ch)
        {
        case '1':
            cout << "Using 16 cores in this example" << endl;
            test_st_multi_core();
            break;
        case '2':
            write_images(images);
            break;
        case '3':
            return;
        default:
            cout << "Please choose 1,2,3" << endl;
            continue;
        }
    }
}

int main()
{
    run();
}