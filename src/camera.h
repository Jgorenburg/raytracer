// camera.h, from https://raytracing.github.io by Peter Shirley, 2018-2020
// modified 2021, by alinen, to support glm/float

#ifndef CAMERA_H
#define CAMERA_H

#include "AGLM.h"

class camera 
{
public:

   camera() : origin(0), horizontal(2, 0, 0), vertical(0, 2, 0)
   {
      lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - glm::vec3(0,0,1);
   }

   
   camera(glm::point3 pos, float viewport_height, float aspect_ratio, float focal_length) 
   {
      origin = pos;
      float viewport_width = aspect_ratio * viewport_height;
      horizontal = viewport_width * glm::vec3(1, 0, 0);
      vertical = viewport_height * glm::vec3(0, 1, 0);
      lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - glm::vec3(0,0,focal_length);
   }


   // camera with a depth of field
   camera(glm::point3 pos, float viewport_height, float aspect_ratio, float focal_length, float aperture)
   {
       origin = pos;
       float viewport_width = aspect_ratio * viewport_height;
       horizontal = focal_length * viewport_width * glm::vec3(1, 0, 0);
       vertical = focal_length * viewport_height * glm::vec3(0, 1, 0);
       lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - glm::vec3(0, 0, focal_length);

       lensR = aperture / 2;
   }

   camera(glm::point3 lookfrom,
          glm::point3 lookat,
          glm::vec3   vup,
          float vfov, // vertical field-of-view in degrees
          float aspect_ratio) 
   {
      // todo
   }


   virtual ray get_ray(float u, float v) const 
   {
       vec3 randOffset;
       bool inDisk = false;
       while (!inDisk) {
           vec3 p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
           if (pow(p.length(), 2) >= 1) { 
               inDisk = true; 
               randOffset = p;
           }
          
       }

       vec3 offset = lensR * randOffset;
      
       return ray(origin + offset, lower_left_corner + u*horizontal + v*vertical - origin - offset);
   }

   

protected:
  glm::point3 origin;
  glm::point3 lower_left_corner;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  float lensR = 0;

 
};


#endif

