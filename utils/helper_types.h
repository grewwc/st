#pragma once

#include <vector>
#include <tuple>
#include <iostream>
#include <vector>
#include "numeric_utils.h"

// using Image = std::vector<float>;

/* this class is only useful for calculating background
    NOT used in any other place
*/
class Pixel
{
public:
    explicit Pixel(float pixel_val, bool is_background = true) : m_val{pixel_val}, m_background{is_background}
    {
    }

    bool is_background() const noexcept
    {
        return m_background;
    }
    void mark_not_background() noexcept
    {
        m_background = false;
    }

    float get_pixel_val() const noexcept
    {
        return m_val;
    }

    void set_pixel_val(float pixel_val) noexcept
    {
        m_val = pixel_val;
    }

    operator float() const noexcept
    {
        return get_pixel_val();
    }

private:
    bool m_background;
    float m_val;
};

class Image
{
private:
    class PrintableShape;

public:
    /**
     * 0 based index
     * effective image after adding multiple images
     * smaller than the original image
     */
    class EffectiveImage
    {
    public:
        explicit EffectiveImage(float **data, int x0, int y0, int x1, int y1)
            : m_x0{x0}, m_y0{y0}, m_x1{x1}, m_y1{y1}, m_data{data}
        {
        }

        const float *operator[](int i) const
        {
            return m_data[i + m_y0] + m_x0;
        }

        PrintableShape shape()
        {
            return {m_y1 - m_y0, m_x1 - m_x0};
        }

    private:
        int m_x0, m_y0, m_x1, m_y1;
        const float *const *m_data = nullptr;
    };

public:
    explicit Image(const std::vector<float> &data, const std::vector<int> &shape)
    {
        m_rows = shape[0];
        m_cols = shape[1];
        m_x0 = m_y0 = 0;
        m_y1 = m_rows;
        m_x1 = m_cols;
        m_data = to_2d(data, shape);
    }

    Image(const Image &other)
    {
        *this = other;
    }

    Image &operator=(const Image &other)
    {
        m_rows = other.m_rows;
        m_cols = other.m_cols;
        m_x0 = other.m_x0;
        m_y0 = other.m_y0;
        m_x1 = other.m_x1;
        m_y1 = other.m_y1;
        m_data = make_2d<float>(other.rows(), other.cols());
        std::copy(other[0], other[0] + other.size(), m_data[0]);
        return *this;
    }

    float *operator[](int i) const noexcept
    {
        return m_data[i];
    }

    Image &operator+=(double val)
    {
        // std::cout << "here " << val << std::endl;
        for (int i = 0; i < rows(); i++)
        {
            for (int j = 0; j < cols(); j++)
            {
                (*this)[i][j] += val;
            }
        }
        return *this;
    }

    Image &operator-=(double val)
    {
        return this->operator+=(-val);
    }

    float *data(bool full = false) const
    {
        if (full)
        {
            return flatten(m_data, real_shape()).data();
        }
        std::vector<float> vec;
        auto subimg = this->get_effect_img();
        vec.reserve(this->size());
        for (int i = 0; i < this->rows(); i++)
        {
            for (int j = 0; j < this->cols(); j++)
            {
                vec.push_back(subimg[i][j]);
            }
        }
        return vec.data();
    }
    /** 
     * effective size
     */
    int size() const noexcept
    {
        return rows() * cols();
    }

    /**
     * effective rows
     */
    int rows() const noexcept
    {
        return m_y1 - m_y0;
    }

    /**
     * effective cols
     */
    int cols() const noexcept
    {
        return m_x1 - m_x0;
    }

    int real_rows() const noexcept
    {
        return m_rows;
    }

    int real_cols() const noexcept
    {
        return m_cols;
    }

    int real_size() const noexcept
    {
        return real_rows() * real_cols();
    }

    int x0() const noexcept
    {
        return m_x0;
    }

    int x1() const noexcept
    {
        return m_x1;
    }

    int y0() const noexcept
    {
        return m_y0;
    }

    int y1() const noexcept
    {
        return m_y1;
    }

    /** x, y can be both POSITIVE and NEGATIVE
     *  if x > 0, then m_x0 ==> larger
     *  y is similar
    */
    void shift(int row, int col) noexcept
    {
        m_x0 = std::max(0, std::min(m_cols, m_x0 - col));
        m_y0 = std::max(0, std::min(m_rows, m_y0 - row));
        m_x1 = std::max(0, std::min(m_cols, m_x1 - col));
        m_y1 = std::max(0, std::min(m_rows, m_y1 - row));
    }

    PrintableShape shape() const
    {
        return {rows(), cols()};
    }

    PrintableShape real_shape() const
    {
        return {real_rows(), real_cols()};
    }

    EffectiveImage get_effect_img() const
    {
        return EffectiveImage{m_data, m_x0, m_y0, m_x1, m_y1};
    }

    ~Image()
    {
        if (m_data)
        {
            del_nd(m_data);
            m_data = nullptr;
        }
    }

    Image &add(const Image &other, int row_shift, int col_shift);
    void save(const char *fname, const char *sample_name, bool full = false) const;

    static Image add_all(const std::vector<Image> &images, int row_shift, int col_shift);

private:
    int m_x0, m_x1, m_y0, m_y1;
    int m_rows, m_cols;
    float **m_data = nullptr;

    class PrintableShape : public std::tuple<int, int>
    {
    public:
        PrintableShape(int rows, int cols)
            : std::tuple<int, int>(rows, cols)
        {
        }

        operator std::vector<int>() const noexcept
        {
            return {static_cast<int>(std::get<0>(*this)), static_cast<int>(std::get<1>(*this))};
        }

        friend std::ostream &operator<<(std::ostream &os, const PrintableShape &shape)
        {
            os << "[" << std::get<0>(shape) << ", " << std::get<1>(shape) << "]\n";
            return os;
        }
    };
};