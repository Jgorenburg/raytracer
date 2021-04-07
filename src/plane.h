#ifndef PLANE_H_
#define PLANE_H_

#include "hittable.h"
#include "AGLM.h"

class plane : public hittable {
public:
   plane() : a(0), n(0), mat_ptr(0) {}
   plane(const glm::point3& p, const glm::vec3& normal, 
      std::shared_ptr<material> m) : a(p), n(normalize(normal)), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {
       // todo
       float t;

       float top = dot((a - r.origin()), n);
       float bottom = dot(r.direction(), n);
       const auto s = 1e-8;

       if (abs(bottom) < s) {
          // std::cout << "got 0\n";
           if (abs(top) > s) { return false; }
           else {
               t = 0;
           }
       }
       else {
           t = top / bottom;
       }

       
       rec.t = t;
       rec.p = r.at(rec.t);
       rec.mat_ptr = mat_ptr;
       rec.set_face_normal(r, n);


       return true;
   }

public:
   glm::vec3 a;
   glm::vec3 n;
   std::shared_ptr<material> mat_ptr;
};

#endif
