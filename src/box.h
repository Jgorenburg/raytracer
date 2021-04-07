#ifndef BOX_H_
#define BOX_H_

#include "hittable.h"
#include "AGLM.h"
#include "list"
#include "map"


using namespace glm;

class box : public hittable {
public:
   box() : c(0), ax(0), ay(0), az(0), hx(0), hy(0), hz(0), mat_ptr(0) {}
   box(const glm::point3& center, 
       const glm::vec3& xdir, const glm::vec3& ydir, const glm::vec3& zdir,
       float halfx, float halfy, float halfz,
       std::shared_ptr<material> m) : c(center), ax(normalize(xdir)), ay(normalize(ydir)), az(normalize(zdir)), 
          hx(halfx), hy(halfy), hz(halfz), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {
       // todo
       float tmin = -infinity;
       float tmax = infinity;

       // rec's normal will end up being the side tmin is on
       vec3 normal;

       vec3 p = c - r.origin();
       std::list<vec3> normals { ax, ay, az };
       std::map<int, float> halfDists = {
           {0, hx}, {1, hy}, {2, hz}
       };



       for (int i = 0; i < 3; i++) {
           vec3 ai = normals.front();
           normals.pop_front();
           float hi = halfDists[i];
           float e = dot(ai, p);
           float f = dot(ai, r.direction());
          
           const auto s = 1e-8;
           if (abs(f) > s) {
               float t1 = (e + hi) / f;
               float t2 = (e - hi) / f;
               if (t1 > t2) {
                   float placeholder = t1;
                   t1 = t2;
                   t2 = placeholder;
               }

               if (t1 > tmin) {
                   tmin = t1;
                   normal = ai;
               }
               else if (t1 == tmin) {
                   normal = normalize(normal + ai);
               }
               if (t2 < tmax) { tmax = t2; }
               if (tmin > tmax || tmax < 0) { return false; }
           }
           else if ((-e - hi) > 0 || (-e + hi) < 0) { return false; }
       }

       if (tmin > 0) {
           rec.t = tmin;
       } 
       else {
           rec.t = tmax;
       }

       rec.p = r.at(rec.t);
       rec.mat_ptr = mat_ptr;
       rec.set_face_normal(r, normal);

       return true;
     
   }

public:
   glm::vec3 c;
   glm::vec3 ax;
   glm::vec3 ay;
   glm::vec3 az;
   float hx;
   float hy;
   float hz;
   std::shared_ptr<material> mat_ptr;
};

#endif
