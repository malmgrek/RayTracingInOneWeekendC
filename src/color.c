#include "color.h"

color_t Color(double r, double g, double b) {
  color_t color = { r, g, b };
  return color;
}

void write_color(color_t *pixel_color, int samples_per_pixel) {

  // Divide the color by the number of samples
  printf("%d %d %d\n",
         (int) (256 * clip(sqrt(pixel_color->x / samples_per_pixel), 0.0, 0.999)),
         (int) (256 * clip(sqrt(pixel_color->y / samples_per_pixel), 0.0, 0.999)),
         (int) (256 * clip(sqrt(pixel_color->z / samples_per_pixel), 0.0, 0.999)));
}