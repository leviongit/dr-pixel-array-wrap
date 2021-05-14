#include "pixel_array.h"
#include "errhandler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************************************************************************/
/* FUNCTIONS :                                                                */
/******************************************************************************/

/* ---=== CREATOR AND DESTRUCTOR : ===--- */

PixelArray *new_pixel_array(size_t width, size_t height, char *name) {

  // --- Creating the pixel array :
  PixelArray *pixel_array = (PixelArray *)malloc(sizeof(PixelArray));

  // --- Setting the console's dimensions :
  pixel_array->width = width;
  pixel_array->height = height;
  pixel_array->name = name;

  // --- Setting the console's pixel array :
  pixel_array->pixels = (uint32_t *)calloc(
      pixel_array->width * pixel_array->height, sizeof(uint32_t));
  clear_pixel_array(pixel_array);

  // --- Polygons rasterizing arrays :
  pixel_array->left_scan    = (uint32_t*)calloc(height, sizeof(uint32_t));
  pixel_array->right_scan   = (uint32_t*)calloc(height, sizeof(uint32_t));
  pixel_array->vertices     = (uint32_t*)calloc(2*MAX_VERTICES, sizeof(uint32_t));
  pixel_array->vertex_count = 0;

  // --- Miscellaneous :
  pixel_array->drb_upload_pixel_array =
      drb_symbol_lookup("drb_upload_pixel_array");

  return pixel_array;
}

void free_pixel_array(PixelArray *pixel_array) {
  free(pixel_array->pixels);
  free(pixel_array);
}

/* ---=== UPDATE : ===--- */

void upload_pixel_array(const PixelArray *pixel_array) {
  pixel_array->drb_upload_pixel_array(pixel_array->name, pixel_array->width,
                                      pixel_array->height, pixel_array->pixels);
}

/* ---=== GEOMETRY MANAGEMENT : ===--- */

size_t get_pixel_array_width(const PixelArray *pixel_array) {
  return pixel_array->width;
}

size_t get_pixel_array_height(const PixelArray *pixel_array) {
  return pixel_array->height;
}

/* ---=== DRAWING : ===--- */

// --- Clearing the pixel array :

void clear_pixel_array(PixelArray *pixel_array) {
  for (size_t y = 0; y < pixel_array->height; y += 1)
    for (size_t x = 0; x < pixel_array->width; x += 1)
      pixel_array->pixels[x + pixel_array->width * y] = C0;
}

// --- Single pixels :

uint32_t unsafe_get_pixel(const PixelArray *pixel_array, uint32_t x, uint32_t y) {
  return pixel_array->pixels[x + pixel_array->width * y];
}

uint32_t get_pixel(const PixelArray *pixel_array, uint32_t x, uint32_t y) {
  int errcode = 0;
  __ERROR_CHECK(x > pixel_array->width, errcode = 1, "X-CORDERR",
                "Out-of-bounds X array access max %lu: provided %lu",
                pixel_array->width, x)
  __ERROR_CHECK(y > pixel_array->height, errcode = 1, "Y-CORDERR",
                "Out-of-bounds Y array access max %lu: provided %lu",
                pixel_array->height, y)
  __HANDLE_ERROR(errcode, return C0;)

  return unsafe_get_pixel(pixel_array, x, y);
}

uint32_t get_pixel_bt(const PixelArray *pixel_array, uint32_t x, uint32_t y) {
  return get_pixel(pixel_array, x, (pixel_array->height - 1) - y);
}

void unsafe_set_pixel(PixelArray *pixel_array, uint32_t x, uint32_t y,
                      uint32_t color) {
  pixel_array->pixels[x + pixel_array->width * y] = color;
}

void set_pixel(PixelArray *pixel_array, uint32_t x, uint32_t y, uint32_t color) {
  int errcode = 0;
  __ERROR_CHECK(x > pixel_array->width, errcode = 1, "X-CORDERR",
                "Out-of-bounds X array access max %lu: provided %lu",
                pixel_array->width, x)
  __ERROR_CHECK(y > pixel_array->height, errcode = 1, "Y-CORDERR",
                "Out-of-bounds Y array access max %lu: provided %lu",
                pixel_array->height, y)
  __HANDLE_ERROR(errcode, return;)

  unsafe_set_pixel(pixel_array, x, y, color);
}

void set_pixel_bt(PixelArray *pixel_array, uint32_t x, uint32_t y, uint32_t color) {
  set_pixel(pixel_array, x, (pixel_array->height - 1) - y, color);
}

// --- Copy :

void unsafe_copy(const PixelArray *src, PixelArray *dest, uint32_t sx, uint32_t sy,
                 size_t sw, size_t sh, size_t dx, size_t dy) {
  for (size_t i = 0; i < sh; ++i)
    memcpy(dest->pixels + dx + ((dy + i) * dest->width),
           src->pixels + dx + ((sy + i) * src->width), sw * sizeof(uint32_t));
}

void copy(const PixelArray *src, PixelArray *dest, uint32_t sx, uint32_t sy,
          size_t sw, size_t sh, size_t dx, size_t dy) {
  int errcode = 0;
  __ERROR_CHECK(dy + sh > dest->height, errcode = 1, "DESTERR",
                "Out-of-bounds Y paste : max %lu, dy = %lu, sh = %lu",
                dest->height, dy, sh)
  __ERROR_CHECK(dx + sw > dest->width, errcode = 1, "DESTERR",
                "Out-of-bounds X paste : max %lu, dy = %lu, sh = %lu",
                dest->width, dx, sw)
  __ERROR_CHECK(sy + sh > src->height, errcode = 1, "SRCERR",
                "Out-of-bounds Y copy : max %lu, dy = %lu, sh = %lu",
                src->height, sy, sh)
  __ERROR_CHECK(sx + sw > src->width, errcode = 1, "SRCERR",
                "Out-of-bounds X copy : max %lu, dy = %lu, sh = %lu",
                src->width, sx, sw)

#if DEBUG_BUILD
  printf("\x1b[1;34m[SRCDBG][INFO]> X max %lu, sx = %lu, sw = %lu, sum = "
         "%lu\x1b[0m\n",
         src->width, sx, sw, sx + sw);
  printf("\x1b[1;34m[SRCDBG][INFO]> Y max %lu, sx = %lu, sw = %lu, sum = "
         "%lu\x1b[0m\n",
         src->height, sy, sh, sy + sh);
  printf("\x1b[1;34m[DSTDBG][INFO]> X max %lu, sx = %lu, sw = %lu, sum = "
         "%lu\x1b[0m\n",
         dest->width, dx, sw, dx + sw);
  printf("\x1b[1;34m[DSTDBG][INFO]> Y max %lu, sx = %lu, sw = %lu, sum = "
         "%lu\x1b[0m\n",
         dest->height, dy, sh, dy + sh);
#endif

  __HANDLE_ERROR(errcode, return;)

  unsafe_copy(src, dest, sx, sy, sw, sh, dx, dy);
}
