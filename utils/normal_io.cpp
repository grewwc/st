#include "normal_io.h"
#include <fstream>
#include <ios>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <memory>

using namespace std;

template <typename T>
static vector<T> read_T(const char *fname)
{
    ifstream input;
    input.open(fname, ios::in | ios::binary);
    if (!input.is_open())
    {
        cout << "cannot open file ( " << fname << " )\n";
        exit(-1);
    }
    vector<T> img;
    constexpr unsigned int _1K = 1024L * 1024L;
    img.reserve(_1K);
    auto buf = unique_ptr<T[]>(new T[_1K]);
    while (input.read((char *)buf.get(), _1K * sizeof(T)))
    {
        auto num_red = input.gcount();
        img.insert(img.end(), buf.get(),
                   buf.get() + num_red / sizeof(T));
    }
    if (input.eof())
    {
        auto num_read = input.gcount();
        img.insert(img.end(), buf.get(),
                   buf.get() + num_read / sizeof(T));
    }
    input.close();
    return img;
}

template <typename T>
static void write_T(const char *fname, const vector<T> &data)
{
    ofstream output{fname, ios::binary | ios::out};
    if (!output.is_open())
    {
        cout << "cannot open file ( " << fname << " )\n";
        exit(-1);
    }

    output.write((char *)data.data(), sizeof(T) * data.size());
    output.close();
}

vector<char> read_byte(const char *fname)
{
    return read_T<char>(fname);
}

vector<int> read_int(const char *fname)
{
    return read_T<int>(fname);
}

vector<unsigned int> read_unsigned_int(const char *fname)
{
    return read_T<unsigned int>(fname);
}

vector<float> read_float(const char *fname)
{
    return read_T<float>(fname);
}

vector<double> read_double(const char *fname)
{
    return read_T<double>(fname);
}

void write_byte(const char *fname, const vector<char> &data)
{
    write_T<char>(fname, data);
}

void write_int(const char *fname, const vector<int> &data)
{
    write_T<int>(fname, data);
}

void write_float(const char *fname, const vector<float> &data)
{
    write_T<float>(fname, data);
}

void write_double(const char *fname, const vector<double> &data)
{
    write_T<double>(fname, data);
}

void write_unsigned_int(const char *fname, const vector<unsigned int> &data)
{
    write_T<unsigned int>(fname, data);
}
