#include "fits_io.h"

#include "fitsio.h"

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