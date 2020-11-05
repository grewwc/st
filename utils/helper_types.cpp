#include "helper_types.h"
#include "normal_io.h"
#include "numeric_utils.h"

#include <stdexcept>
#include <cmath>

#ifdef debug
#include <iostream>
#endif

using namespace std;

Image &Image::add(const Image &other, int row_shift, int col_shift)
{
    // cout << "here " << (row_shift + 100) << " " << (col_shift + 100) << endl;

    for (int i = m_y0; i < m_y1; i++)
    {
        for (int j = m_x0; j < m_x1; j++)
        {
            (*this)[i][j] += other[i + row_shift][j + col_shift];
        }
    }

    return *this;
}

Image Image::add_all(const vector<Image> &images, int row_shift, int col_shift)
{
    if (images.size() < 0)
    {
        throw new runtime_error("empty images");
    }

    Image res{images[0]};

    for (int i = 1; i < (int)images.size(); i++)
    {
        res.shift(row_shift, col_shift);
        res.add(images[i], row_shift * i, col_shift * i);
    }
    return res;
}

void Image::save(const char *outname, bool full)
{
    vector<float> vec;
    if (full)
    {
        vec = flatten(this->m_data, this->real_shape());
    }
    else
    {
        auto subimg = this->get_effect_img();
        vec.reserve(this->size());
        for (int i = 0; i < this->rows(); i++)
        {
            for (int j = 0; j < this->cols(); j++)
            {
                vec.push_back(subimg[i][j]);
            }
        }
    }
    write_float(outname, vec);
}