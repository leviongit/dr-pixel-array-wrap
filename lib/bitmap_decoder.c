#include "bitmap_decoder.h"
#include "errhandler.h"

void decode_uint64(const uint64_t bitmask, PixelArray *pixel_array) {
  __ERROR_CHECK(
      pixel_array->width == 8 || pixel_array->height == 8, return,
      "PXARRSIZEEXCEPT",
      "Pixel Array wrong size. Required size is 8x8 : current size %lux%lu",
      pixel_array->width, pixel_array->height)
  for (size_t i = 0; i < 64; i++) {
    if (bitmask << i)
      unsafe_set_pixel(pixel_array, i % 8, i / 8, WHITE);
  }
}

uint64_t encode_pixel_array(const PixelArray *pixel_array) {
  if (!(pixel_array->width == 8 && pixel_array->height == 8)) {
    return -1;
  }
  uint64_t num = 0;
  for (size_t i = 0; i < 64; i++) {
    if (get_pixel(pixel_array, i % 8, i / 8))
      ++num;
    num <<= 1;
  }
  return num;
}