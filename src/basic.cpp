// Raytracer framework from https://raytracing.github.io by Peter Shirley, 2018-2020
// alinen 2021, modified to use glm and ppm_image class

#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"
#include "sphere.h"
#include "box.h"
#include "plane.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

using namespace glm;
using namespace agl;
using namespace std;

color ray_color(const ray& r, const hittable_list& world, int depth)
{
   hit_record rec;
   if (depth <= 0)
   {
      return color(0);
   }

   if (world.hit(r, 0.001f, infinity, rec))
   {
      ray scattered;
      color attenuation;
      if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      {
         color recurseColor = ray_color(scattered, world, depth - 1);
         return attenuation * recurseColor;
      }
      return attenuation;
   }
   vec3 unit_direction = normalize(r.direction());
   auto t = 0.5f * (unit_direction.y + 1.0f);
   return (1.0f - t) * color(1, 1, 1) + t * color(0.5f, 0.7f, 1.0f);
}

color normalize_color(const color& c, int samples_per_pixel)
{
   // todo: implement me!
   float scale = 1.0f / samples_per_pixel;
   float r = std::min(0.999f, std::max(0.0f, c.r * scale));
   float g = std::min(0.999f, std::max(0.0f, c.g * scale));
   float b = std::min(0.999f, std::max(0.0f, c.b * scale));

   // apply gamma correction 
   r = sqrt(r);
   g = sqrt(g);
   b = sqrt(b);

   return color(r, g, b);
}

void ray_trace(ppm_image& image)
{
   // Image
   int height = image.height();
   int width = image.width();
   float aspect = width / float(height);
   int samples_per_pixel = 10; // higher => more anti-aliasing
   int max_depth = 10; // higher => less shadow acne

   // World
   shared_ptr<material> gray = make_shared<lambertian>(color(0.5f));
   shared_ptr<material> red = make_shared<lambertian>(color(1, 0, 0));
   shared_ptr<material> blue = make_shared<lambertian>(color(0, 0, 1));
   shared_ptr<material> whitish = make_shared<metal>(color(0.9, 0.9, 0.9), 0);
   shared_ptr<material> purple = make_shared<phong>(color(0.7, 0, 0.7), color(1, 1, 1), color(0.1, 0, 0.1), point3(5, 5, 1), 
       point3(0,0,0), 0.45f,  0.45f,  0.1f, 10);
   shared_ptr<material> refract = make_shared<dielectric>(1.5);

   shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
   shared_ptr<material> material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
   shared_ptr<material> material_left = make_shared<dielectric>(1.5);
   shared_ptr<material> material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

   hittable_list world;
   /*
   world.add(make_shared<sphere>(point3(0, 0, -1), 0.5f, refract));
   world.add(make_shared<box>(point3(2, 0.5, -2), vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, 0, 1), 0.5f, 0.5f, 0.5f, purple));
   world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, blue));
   world.add(make_shared<plane>(point3(-2, 0, 0), vec3(1, 0, 0), whitish));
   */

   world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
   world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
   world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
   world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
   world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

   // refract
   //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, gray));
   //world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, refract));

   // Camera
   vec3 camera_pos(0);
   float viewport_height = 2.0f;
   float focal_length = 1.0; 
   camera cam(camera_pos, viewport_height, aspect, focal_length);

   // Ray trace
   for (int j = 0; j < height; j++)
   {
      for (int i = 0; i < width; i++)
      {
         color c(0, 0, 0);
         for (int s = 0; s < samples_per_pixel; s++) // antialias
         {
            float u = float(i + random_float()) / (width - 1);
            float v = float(height - j - 1 - random_float()) / (height - 1);

            ray r = cam.get_ray(u, v);
            c += ray_color(r, world, max_depth);
         }
         c = normalize_color(c, samples_per_pixel);
         image.set_vec3(j, i, c);
      }
   }

   image.save("basic.png");
}
