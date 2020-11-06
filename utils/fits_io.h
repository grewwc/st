#pragma once

#include <vector>
#include "helper_types.h"


Image read_image_2d(const char *fname);

void write_image_2d(const char *fname, const char* sample_name, const Image& img);
