#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "scene.h"
#include "utils.h"
#include "vec3.h"

// TODO: Refactor to reasonable modules
// TODO: Makefile
// TODO: Readme
// TODO: GitHub

color_t ray_color(ray_t ray, world_t world, int depth) {

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) {
    return black;
  }

  hit_record_t rec = hit(ray, world);

  if (rec.count > 0) {
    color_t attenuation;
    ray_t scattered;
    if (scatter(ray, rec, &attenuation, &scattered)) {
      return emul(attenuation, ray_color(scattered, world, depth-1));
    }
    return black;
  }

  vec3_t unit = unit_vector(ray.direction);
  double t = 0.5 * (unit.y + 1.0);
  color_t color = add(mul(1.0 - t, white), mul(t, lightblue));
  return color;

}

int main() {

  /* Image */
  const double aspect_ratio = 3.0 / 2.0;
  const int image_width = 400;
  const int image_height = (int) (image_width / aspect_ratio);
  const int samples_per_pixel = 4;
  const int max_depth = 50;

  /* Camera */
  const vec3_t lookfrom = { 13.0, 2.0, 3.0 };
  const vec3_t lookat = { 0.0, 0.0, 0.0 };
  const vec3_t vup = { 0.0, 1.0, 0.0 };
  const double vfov = 20.0;
  const double aperture = 0.1;
  // double dist_to_focus = norm(sub(lookfrom, lookat));
  double dist_to_focus = 10.0;
  camera_t cam = Camera(lookfrom,
                        lookat,
                        vup,
                        vfov,
                        aspect_ratio,
                        aperture,
                        dist_to_focus);

  world_t *world = example_scene();

  /* Render */
  printf("P3\n%d %d\n255\n", image_width, image_height);
  for (int j = image_height-1; j >= 0; --j) {
    /* Progress bar ------------------------------------------------------------ */
    progress_bar(1.0 - (double) j / image_height);
    /* ------------------------------------------------------------------------- */
    for (int i = 0; i < image_width; ++i) {


      color_t pixel_color = { 0.0, 0.0, 0.0 };
      for (int q = 0; q < samples_per_pixel; ++q) {
        double s = (i + random_double_unit()) / (image_width - 1);
        double t = (j + random_double_unit()) / (image_height - 1);
        ray_t ray = get_ray(cam, s, t);
        pixel_color = add(pixel_color, ray_color(ray, *world, max_depth));
      }
      write_color(pixel_color, samples_per_pixel);

    }
  }

  destroy_world(world);

}
