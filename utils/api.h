#pragma once
#include "helper_types.h"
#include <deque>
#include <algorithm>

struct CombinedSpace;

struct FittingResult
{
    int i0, j0;
    double vi, vj;

    double cost = 0.0;

    FittingResult(int i0, int j0, double vi, double vj)
        : i0{i0}, j0{j0}, vi{vi}, vj{vj} {}

    friend std::ostream &operator<<(std::ostream &os, const FittingResult &result)
    {
        os << "i0: " << result.i0 << ", j0: " << result.j0 << ", vi: " << result.vi << ", vj: " << result.vj;
        return os;
    }
};

struct FittingSpace
{
    double lo, hi;
    int n;

    FittingSpace(double lo = 0, double hi = 0, int n = 1)
        : lo{lo}, hi{hi}, n{n} {}

    FittingSpace(const FittingSpace &other)
    {
        lo = other.lo;
        hi = other.hi;
        n = other.n;
    }

    /** pixel value should be integer */ 
    FittingSpace& to_int()
    {
        lo = (int)lo;
        hi = (int)hi;
        return *this;
    }

    explicit operator std::vector<double>() const noexcept
    {
        std::vector<double> res;
        res.reserve(n);
        double step = (hi - lo) / (double)n;

        for (int i = 0; i < n; i++)
        {
            res.push_back(step * i + lo);
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const FittingSpace &space)
    {
        os << "lo: " << space.lo << ", hi: " << space.hi << ", n: " << space.n;
        return os;
    }

    std::vector<FittingSpace> split(int n_split = 2) const
    {
        if (n_split <= 1)
        {
            throw new std::logic_error{"n_split should > 2"};
        }
        int n_each_bucket = n / n_split;
        int n_last_bucket = n - n_each_bucket * (n_split - 1);
        double step = (hi - lo) / (double)n_split;

        std::vector<FittingSpace> res;
        res.reserve(n_split);

        for (int i = 0; i < n_split - 1; i++)
        {
            res.emplace_back(lo + i * step, lo + (i + 1) * step, n_each_bucket);
        }

        // treat last bucket differently
        res.emplace_back(hi - step, hi, n_last_bucket);
        return res;
    }

private:
    friend CombinedSpace;
    friend void bfs(const CombinedSpace &s, int n_split, int curr_idx, std::deque<CombinedSpace> &st);
};

struct CombinedSpace
{
public:
    CombinedSpace(std::initializer_list<FittingSpace> spaces)
        : m_spaces{spaces}, m_size{static_cast<int>(spaces.size())}
    {
    }

    CombinedSpace(const CombinedSpace &other)
    {
        m_size = other.m_size;
        m_spaces.resize(m_size);
        std::copy(other.m_spaces.cbegin(), other.m_spaces.cend(), m_spaces.begin());
    }

    const FittingSpace &operator[](int i) const
    {
        return m_spaces[i];
    }

    FittingSpace &operator[](int i)
    {
        return m_spaces[i];
    }

    int size() const noexcept
    {
        return m_size;
    }

    friend std::ostream &operator<<(std::ostream &os, const CombinedSpace &space)
    {
        os << "{\n";
        for (const auto &e : space.m_spaces)
        {
            os << "\t" << e << "\n";
        }
        os << "}";
        return os;
    }

    std::vector<CombinedSpace> split(int n = 2) const;

private:
    std::vector<FittingSpace> m_spaces;
    int m_size;

    void push_back(const FittingSpace &space)
    {
        m_size++;
        m_spaces.push_back(space);
    }
};

/** calculate the background of a set of images */
double calc_bg(const std::vector<Image> &images, int sigma = 5);

// /** read 1 2D image from fits file with cfitsio */
// Image read_image_2d(const char *fname);

/** add images with shift */
Image add_all_images(const std::vector<Image> &images, int row_shift, int col_shift);


FittingResult fit(const std::vector<Image> &images, const FittingSpace &x0,
                  const FittingSpace &y0, const FittingSpace &vi, const FittingSpace &vj);


FittingResult fit_multi(const std::vector<Image> &images, const FittingSpace &x0,
                  const FittingSpace &y0, const FittingSpace &vi, const FittingSpace &vj, int cores,
                  bool verbose=false);
