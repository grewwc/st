#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <numeric>

#include "helper_traits.h"

/* make high dimension arrays*/
template <typename T = double>
inline T **make_2d(int rows, int cols, bool fill = false,
                   T fill_val = (T)0)
{
    const auto num = rows * cols;
    T **res = (T **)::operator new(rows * sizeof(T *));
    res[0] = (T *)::operator new(num * sizeof(T));
    if (fill)
    {
        std::fill_n(res[0], num, fill_val);
    }
    for (int i = 1; i < rows; i++)
    {
        res[i] = res[i - 1] + cols;
    }
    return res;
}

template <typename T = double>
T ***make_3d(int rows, int cols, int dim_z,
             bool fill = false, T fill_val = (T)0)
{
    const auto dim_xy = rows * cols, dim_yz = cols * dim_z;
    const auto dim_xyz = rows * cols * dim_z;
    T ***res = (T ***)::operator new(rows * sizeof(T **));
    res[0] = (T **)operator new(dim_xy * sizeof(T *));
    res[0][0] = (T *)operator new(dim_xyz * sizeof(T));
    if (fill)
    {
        std::fill_n(res[0][0], dim_xyz, fill_val);
    }
    for (int j = 1; j < cols; j++)
    {
        res[0][j] = res[0][j - 1] + dim_z;
    }
    for (int i = 1; i < rows; i++)
    {
        res[i] = res[i - 1] + cols;
        res[i][0] = res[i - 1][0] + dim_yz;
        for (int j = 1; j < cols; j++)
        {
            res[i][j] = res[i][j - 1] + dim_z;
        }
    }
    return res;
}

/* reshape vectors 
    vec: source 
    shape: shape of the target array
*/
template <typename T>
inline T **to_2d(const std::vector<T> &vec, const std::vector<int> &shape)
{
    assert(shape.size() == 2);
    using size_type = typename std::vector<T>::size_type;
    const size_type rows = shape[0], cols = shape[1];
    const size_type num = rows * cols;
    T **res = make_2d<T>((int)rows, (int)cols);
    std::copy(vec.cbegin(), vec.cend(), res[0]);
    return res;
}

template <typename T>
inline T ***to_3d(const std::vector<T> &vec, const std::vector<int> &shape)
{
    assert(shape.size() == 3);
    const auto rows = shape[0], cols = shape[1],
               dim_z = shape[2];
    T ***res = make_3d<T>((int)rows, (int)cols, (int)dim_z);
    std::copy(vec.cbegin(), vec.cend(), res[0][0]);
    return res;
}

/* get how many "*" before the type*/
template <typename T>
inline constexpr int ptr_level()
{
    if constexpr (std::is_pointer_v<T>)
        return ptr_level<std::remove_pointer_t<T>>() + 1;
    else
        return 0;
}

/* delete allocated ndarray 
    remember to call this function after calling "to_2d" and "to_3d"
    tested using "valgrind" with no errors
*/
template <typename T,
          typename = std::enable_if_t<std::is_pointer_v<T>>>
void del_nd(T arg)
{
    constexpr int LEVEL = ptr_level<T>();
    if constexpr (LEVEL > 1)
    {
        del_nd(*arg);
        delete arg;
    }
    else
    {
        delete arg;
    }
}

template <typename T,
          typename = std::enable_if_t<std::is_pointer_v<T>, void *>>
auto flatten(T arr, const std::vector<int> &shape)
{
    const int dims = shape.size();
    const int num = std::accumulate(shape.cbegin(), shape.cend(), 1,
                               std::multiplies<int>());
    using type = typename underlying_type<T>::type;
    std::vector<type> res(num);
    switch (dims)
    {
    case 1:  // here is problematic
        std::copy((type *)arr, (type *)arr + num, res.begin());
        break;
    case 2:
        std::copy((type *)*arr, (type *)*arr + num, res.begin());
        break;
    default:
        std::cerr << "shape wrong" << std::endl;
        exit(-1);
    }
    return res;
}