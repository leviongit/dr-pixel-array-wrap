#include "lines.h"
#include <stdlib.h>

// --- Lines :

void draw_horizontal_line(PixelArray *pixel_array,uint32_t x1,uint32_t x2,uint32_t y,uint32_t color) {
  uint32_t start, end;

  if (x1 <= x2) {
    start = x1;
    end = x2;
  } else {
    start = x2;
    end = x1;
  }

  for (uint32_t x = start; x <= end; x += 1)
    pixel_array->pixels[x + pixel_array->width * y] = color;
}


void draw_vertical_line(PixelArray *pixel_array,uint32_t x,uint32_t y1,uint32_t y2,uint32_t color) {
  uint32_t start, end;

  if (y1 <= y2) {
    start = y1;
    end = y2;
  } else {
    start = y2;
    end = y1;
  }

  for (size_t y = start; y <= end; y += 1)
    pixel_array->pixels[x + pixel_array->width * y] = color;
}


void draw_line(PixelArray *pixel_array,uint32_t x1,uint32_t y1,uint32_t x2,uint32_t y2,uint32_t color) {
  int32_t x, y, dx, dy, x_increment, y_increment, d;

  // Set-up of the bresenham algorythm :
  x = x1;
  y = y1;

  dx = x2 - x1;
  dy = y2 - y1;

  x_increment = (dx > 0) ? 1 : -1;
  y_increment = (dy > 0) ? 1 : -1;

  // Edge cases :
  if (dx == 0) { // vertical line
    draw_vertical_line(pixel_array, x1, y1, y2, color);
    return;
  }

  if (dy == 0) { // horizontal line
    draw_horizontal_line(pixel_array, x1, x2, y1, color);
    return;
  }

  dx = abs(dx);
  dy = abs(dy);

  // Drawing the line :

  // First Point :
  pixel_array->pixels[x1 + pixel_array->width * y1] = color;

  // Rest of the Line :
  if (dx > dy) {

    d = dx / 2;

    for (size_t i = 1; i <= dx; i += 1) {
      x += x_increment;
      d += dy;

      if (d >= dx) {
        d -= dx;
        y += y_increment;
        pixel_array->pixels[x + pixel_array->width * y] = color;
      } else {
        if (y_increment < 0)
          pixel_array->pixels[x + pixel_array->width * y] = color;
        else
          pixel_array->pixels[x + pixel_array->width * (y + 1)] = color;
      }
    }
  } else {
    d = dy / 2;

    for (size_t i = 1; i <= dy; i += 1) {
      y += y_increment;
      d += dx;

      if (d >= dy) {
        d -= dy;
        x += x_increment;
      }

      pixel_array->pixels[x + pixel_array->width * y] = color;
    }
  }
}
