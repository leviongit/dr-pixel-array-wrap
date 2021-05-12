#ifndef BITMAP_DECODER_H
#define BITMAP_DECODER_H

#include "pixel_array.h"

void decode_uint64(const uint64_t bitmask, PixelArray *pixel_array);
uint64_t encode_pixel_array(const PixelArray *pixel_array);

#endif