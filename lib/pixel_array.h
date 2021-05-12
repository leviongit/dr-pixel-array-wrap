#include <stddef.h>
#include <stdint.h>

/******************************************************************************/
/* CONSTANTS :                                                                */
/******************************************************************************/
#define C0 0x00000000
#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF

/******************************************************************************/
/* DATA TYPES :                                                               */
/******************************************************************************/
extern void *(*drb_symbol_lookup)(const char *sym);
typedef void (*drb_upload_pixel_array_fn)(const char *name, const int w,
                                          const int h, const uint32_t *pixels);

/* ---=== FOR THE PIXEL ARRAY : ===--- */
typedef struct PixelArray {
  size_t width;
  size_t height;
  uint32_t *pixels;
  char *name;

  drb_upload_pixel_array_fn drb_upload_pixel_array;
} PixelArray;

/******************************************************************************/
/* FUNCTION PROTOTYPES :                                                      */
/******************************************************************************/

// --- Creator and Destructor :
PixelArray *new_pixel_array(size_t width, size_t height, char *name);
void free_pixel_array(PixelArray *pixel_array);

// --- Geometry Management :
size_t get_pixel_array_width(const PixelArray *pixel_array);
size_t get_pixel_array_height(const PixelArray *pixel_array);

// --- Update :
void upload_pixel_array(const PixelArray *pixel_array);

// --- Drawing :
// --- Clearing the console :
void clear_pixel_array(PixelArray *pixel_array);

// --- Single Pixels :
uint32_t unsafe_get_pixel(const PixelArray *pixel_array, size_t x, size_t y);
uint32_t get_pixel(const PixelArray *pixel_array, size_t x, size_t y);
uint32_t get_pixel_bt(const PixelArray *pixel_array, size_t x, size_t y);
void unsafe_set_pixel(PixelArray *pixel_array, size_t x, size_t y,
                      uint32_t color);
void set_pixel(PixelArray *pixel_array, size_t x, size_t y, uint32_t color);
void set_pixel_bt(PixelArray *pixel_array, size_t x, size_t y, uint32_t color);

// --- Copy :
void unsafe_copy(const PixelArray *src, PixelArray *dest, size_t sx, size_t sy,
                 size_t sw, size_t sh, size_t dx, size_t dy);
void copy(const PixelArray *src, PixelArray *dest, size_t sx, size_t sy,
          size_t sw, size_t sh, size_t dx, size_t dy);
void copy_bt(const PixelArray *src, PixelArray *dest, size_t sx, size_t sy,
             size_t sw, size_t sh, size_t dx, size_t dy);
