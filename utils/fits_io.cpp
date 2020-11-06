#include "fits_io.h"

#include "fitsio.h"
#include <filesystem>

#ifdef debug
#include <iostream>
#endif

using namespace std;

Image read_image_2d(const char *fname)
{
    fitsfile *fptr;
    int status = 0, nfound, anynull;
    fits_open_file(&fptr, fname, READONLY, &status);
    fits_report_error(stderr, status);
    long naxis[2], fpixel = 1, nullval = 0;

    fits_read_keys_lng(fptr, "NAXIS", 1, 2, naxis, &nfound, &status);
    fits_report_error(stderr, status);

#ifdef debug
    cout << naxis[0] << " " << naxis[1] << endl;
    cout << nfound << endl;
#endif

    long nelem = naxis[0] * naxis[1];
    vector<float> res_data(nelem);
    fits_read_img(fptr, TFLOAT, fpixel, nelem, &nullval, res_data.data(), &anynull, &status);
    fits_report_error(stderr, status);

    fits_close_file(fptr, &status);
    fits_report_error(stderr, status);

#ifdef debug
    for (int i = 0; i < 20; i++)
    {
        cout << buffer[i] << "  ";
    }
    cout << endl;
#endif

    return Image{res_data, {static_cast<int>(naxis[1]), static_cast<int>(naxis[0])}};
}

void write_image_2d(const char *fname, const char *sample_name, const Image &img)
{
    fitsfile *fptr, *infptr;
    int status = 0, nfound, anynull;
    if (!fits_open_file(&infptr, sample_name, READONLY, &status))
    {
        if (filesystem::exists(fname))
        {
            if (remove(fname) != 0)
            {
                perror((string("error deleting file: ") + fname).c_str());
            };
        }
        if (!fits_create_file(&fptr, fname, &status))
        {
            fits_copy_file(infptr, fptr, 1, 1, 1, &status);
            fits_report_error(stderr, status);
        }
    }

    long naxis[2], fpixel = 1, nullval = 0;

#ifdef debug
    cout << naxis[0] << " " << naxis[1] << endl;
    cout << nfound << endl;
#endif
    fits_write_img_flt(fptr, 0, fpixel, img.real_size(), img.data(true), &status);
    // fits_read_img(,fptr, TFLOAT, fpixel, nelem, &nullval, res_data.data(), &anynull, &status);
    fits_report_error(stderr, status);

    fits_close_file(fptr, &status);
    fits_report_error(stderr, status);

#ifdef debug
    for (int i = 0; i < 20; i++)
    {
        cout << buffer[i] << "  ";
    }
    cout << endl;
#endif
}
