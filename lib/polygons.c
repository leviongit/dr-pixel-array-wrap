#include "polygons.h"
#include "lines.h"
#include "pixel_array.h"
#include <stdlib.h>

// --- Rectangles :
void stroke_rectangle(PixelArray* pixel_array,uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color) {
  for(size_t i = x; i < x + width; i += 1) {
    pixel_array->pixels[i + pixel_array->width * y] = color;
    pixel_array->pixels[i + pixel_array->width * (y + height - 1)] = color;
  }
  for(size_t i = y + 1; i < y + height - 1; i += 1) {
    pixel_array->pixels[x + pixel_array->width * i] = color;
    pixel_array->pixels[x + width - 1 + pixel_array->width * i] = color;
  }
}

void fill_rectangle(PixelArray* pixel_array,uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color) {
  for(size_t i = y; i < y + height - 1; i += 1)
    for(size_t j = x; j < x + width - 1; j += 1)
      pixel_array->pixels[j + pixel_array->width * i] = color;
}

// --- Arbitrary Polygons :
uint32_t* get_polygon_vertices_array(PixelArray* pixel_array) {
  return pixel_array->vertices;
}

void set_polygon_vertex_count(PixelArray* pixel_array,size_t count) {
  pixel_array->vertex_count = count;
}

void scan_polygon_left_edge(PixelArray* pixel_array,uint32_t x1,uint32_t y1,uint32_t x2,uint32_t y2) {
  int32_t x, y, dx, dy, x_increment, y_increment, d;

  // Set-up of the bresenham algorythm :
  x = x1;
  y = y1;

  dx = x2 - x1;
  dy = y2 - y1;

  x_increment = ( dx > 0 ) ? 1 : -1 ;
  y_increment = ( dy > 0 ) ? 1 : -1 ;


  // Edge cases :
  if (dx == 0) {  // vertical line
    for (size_t y = y1; y >= y2; y -= 1 )
      pixel_array->left_scan[y] = x1;
    return;
  }

  if (dy == 0) {  // horizontal line
    pixel_array->left_scan[y1]  = x1 < x2 ? x1 : x2;
    return;
  }

  dx = labs(dx);
  dy = labs(dy);


  // Drawing the line :

  // First Point :
  pixel_array->left_scan[y1]  = x1;

  // Rest of the Line :
  if (dx > dy) {

    d = dx / 2;

    for(size_t i = 1; i <= dx; i += 1) {
      x += x_increment;
      d += dy;

      if (d >= dx) {
        d -= dx;
        y += y_increment;
        pixel_array->left_scan[y] = x;
      }
      else {
        if (y_increment < 0)
          pixel_array->left_scan[y] = x;
        else
          pixel_array->left_scan[y+1] = x;
      }
    }
  }
  else {
    d = dy / 2;

    for(size_t i = 1; i <= dy; i += 1) {
      y += y_increment;
      d += dx;

      if (d >= dy) {
        d -= dy;
        x += x_increment;
      }

      pixel_array->left_scan[y] = x;
    }
  }
}

void scan_polygon_right_edge(PixelArray* pixel_array,uint32_t x1,uint32_t y1,uint32_t x2,uint32_t y2) {
  int32_t x, y, dx, dy, x_increment, y_increment, d;

  // Set-up of the bresenham algorythm :
  x = x1;
  y = y1;

  dx = x2 - x1;
  dy = y2 - y1;

  x_increment = ( dx > 0 ) ? 1 : -1 ;
  y_increment = ( dy > 0 ) ? 1 : -1 ;


  // Edge cases :
  if (dx == 0) {  // vertical line
    for (size_t y = y1; y >= y2; y -= 1 )
      pixel_array->right_scan[y] = x1;
    return;
  }

  if (dy == 0) {  // horizontal line
    pixel_array->right_scan[y1]  = x1 > x2 ? x1 : x2;
    return;
  }

  dx = labs(dx);
  dy = labs(dy);


  // Drawing the line :

  // First Point :
  pixel_array->right_scan[y1]  = x1;

  // Rest of the Line :
  if (dx > dy) {

    d = dx / 2;

    for(size_t i = 1; i <= dx; i += 1) {
      x += x_increment;
      d += dy;

      if (d >= dx) {
        d -= dx;
        y += y_increment;
        pixel_array->right_scan[y] = x;
      }
      else {
        if (y_increment < 0)
          pixel_array->right_scan[y] = x;
        else
          pixel_array->right_scan[y+1] = x;
      }
    }
  }
  else {
    d = dy / 2;

    for(size_t i = 1; i <= dy; i += 1) {
      y += y_increment;
      d += dx;

      if (d >= dy) {
        d -= dy;
        x += x_increment;
      }

      pixel_array->right_scan[y] = x;
    }
  }
}

void fill_polygon(PixelArray* pixel_array,uint32_t color) {
  size_t i;

  // Clearing the rasterizer's buffers :
  for(i = 0; i < pixel_array->height; i += 1) {
    pixel_array->left_scan[i]   = UINT8_MAX;
    pixel_array->right_scan[i]  = 0;
  }


  // Finding the top and bottom of the polygon :

  // Initializing the loop with the first point y value :
  uint32_t max_y_index = 0;
  uint32_t min_y_index = 0;

  uint32_t max_y  = pixel_array->vertices[1];
  uint32_t min_y  = pixel_array->vertices[1];

  for(i = 1; i < pixel_array->vertex_count; i += 1) {

    // Check the bottom :
    if ( pixel_array->vertices[2*i+1] < min_y ) {
      min_y       = pixel_array->vertices[2*i+1];
      min_y_index = i;
    }

    // Check the top :
    if ( pixel_array->vertices[2*i+1] > max_y ) {
      max_y       = pixel_array->vertices[2*i+1];
      max_y_index = i;
    }

  }


  // Discard null height polygons :
  if ( min_y == max_y )
    return;


  // Devise the winding order :
  uint32_t  previous_point_x  = pixel_array->vertices[2*(( max_y_index - 1 + pixel_array->vertex_count) % pixel_array->vertex_count)];
  uint32_t  next_point_x      = pixel_array->vertices[2*(( max_y_index + 1 ) % pixel_array->vertex_count)];
  int32_t   winding_order     = previous_point_x < next_point_x ? -1 : 1;

  // Left :
  i = 0;
  while ( pixel_array->vertices[2 * ( ( ( max_y_index + i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1] != min_y ) {
    scan_polygon_left_edge( pixel_array,
                            pixel_array->vertices[2 * ( ( ( max_y_index + i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) ],
                            pixel_array->vertices[2 * ( ( ( max_y_index + i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1],
                            pixel_array->vertices[2 * ( ( ( max_y_index + ( i + 1 ) * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) ],
                            pixel_array->vertices[2 * ( ( ( max_y_index + ( i + 1 ) * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1] );

    i += 1;
  }

  // Right :
  i = 0;
  while ( pixel_array->vertices[2 * ( ( ( max_y_index - i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1] != min_y ) {
    scan_polygon_right_edge(  pixel_array,
                              pixel_array->vertices[2 * ( ( ( max_y_index - i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count )],
                              pixel_array->vertices[2 * ( ( ( max_y_index - i * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1],
                              pixel_array->vertices[2 * ( ( ( max_y_index - ( i + 1 ) * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) ],
                              pixel_array->vertices[2 * ( ( ( max_y_index - ( i + 1 ) * winding_order ) + pixel_array->vertex_count ) % pixel_array->vertex_count ) + 1] );

    i += 1;
  }


  // Draw the polygon :
  for(i = min_y; i <= max_y; i += 1)
    draw_horizontal_line( pixel_array,
                          pixel_array->left_scan[i],
                          pixel_array->right_scan[i],
                          i,
                          color );
}
