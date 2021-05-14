#include "pixel_array.h"

// --- Rectangles :
void stroke_rectangle(PixelArray *pixel_array,uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color);
void fill_rectangle(PixelArray *pixel_array,uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color);

// --- Arbitrary polygons :
uint32_t* get_polygon_vertices_array(PixelArray* pixel_array);
void    set_polygon_vertex_count(PixelArray* pixel_array,size_t count);
void    fill_polygon(PixelArray* pixel_array,uint32_t color);
