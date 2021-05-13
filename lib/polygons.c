#include "pixel_array.h"
#include "polygons.h"

void stroke_rectangle(PixelArray *pixel_array,size_t x,size_t y,size_t width,size_t height,uint32_t color) {
  for(size_t i = x; i < x + width; i += 1) {
    pixel_array->pixels[i + pixel_array->width * y] = color;
    pixel_array->pixels[i + pixel_array->width * (y + height - 1)] = color;
  }
  for(size_t i = y + 1; i < y + height - 1; i += 1) {
    pixel_array->pixels[x + pixel_array->width * i] = color;
    pixel_array->pixels[x + width - 1 + pixel_array->width * i] = color;
  }
}

void fill_rectangle(PixelArray *pixel_array,size_t x,size_t y,size_t width,size_t height,uint32_t color) {
  for(size_t i = y; i < y + height - 1; i += 1)
    for(size_t j = x; j < x + width - 1; j += 1)
      pixel_array->pixels[j + pixel_array->width * i] = color;
}
