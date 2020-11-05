#pragma once 
#include <vector> 

std::vector<char> read_byte(const char* fname);
std::vector<int> read_int(const char* fname);
std::vector<float> read_float(const char* fname);
std::vector<double> read_double(const char* fname);
std::vector<unsigned int> read_unsigned_int(const char* fname);

void write_byte(const char* fname, const std::vector<char>& data);
void write_int(const char* fname, const std::vector<int>& data);
void write_float(const char* fname, const std::vector<float>& data);
void write_double(const char* fname, const std::vector<double>& data);
void write_unsigned_int(const char* fname, const std::vector<unsigned int>& data);