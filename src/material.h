#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"

using namespace glm;
using namespace std;


class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const = 0;
  virtual ~material() {}
};

class lambertian : public material {
public:
  lambertian(const glm::color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
  {
     // todo
     
      using namespace glm;
    /*
      vec3 unitn = normalize(rec.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - rec.p);
      color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;


      attenuation = diffuse;
      return false;
    */

      vec3 scatter_dir = rec.normal + random_unit_vector();
      if (near_zero(scatter_dir)) {
          scatter_dir = rec.normal;
      }
      scattered = ray(rec.p, scatter_dir);
      attenuation = albedo;
      return true;
  }

public:
  glm::color albedo;
};

class phong : public material {
public:
  phong(const glm::vec3& view) :
     diffuseColor(0,0,1), 
     specColor(1,1,1),
     ambientColor(.01f, .01f, .01f),
     lightPos(5,5,0),
     viewPos(view), 
     kd(0.45), ks(0.45), ka(0.1), shininess(10.0) 
  {}

  phong(const glm::color& idiffuseColor, 
        const glm::color& ispecColor,
        const glm::color& iambientColor,
        const glm::point3& ilightPos, 
        const glm::point3& iviewPos, 
        float ikd, float iks, float ika, float ishininess) : 
     diffuseColor(idiffuseColor), 
     specColor(ispecColor),
     ambientColor(iambientColor),
     lightPos(ilightPos),
     viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess) 
  {}

  virtual bool scatter(const ray& r_in, const hit_record& hit, 
     glm::color& attenuation, ray& scattered) const override 
  {
      // ambient
      color ambient = ka * ambientColor;

      // diffuse
      vec3 unitn = normalize(hit.normal); 
      vec3 lightDir = normalize(lightPos - hit.p); 
      color diffuse = kd * max(vec3(0), dot(unitn, lightDir)) * diffuseColor;

      // spec
      vec3 unitr = normalize(2 * dot(lightDir, unitn) * unitn - lightDir);
      vec3 viewDir = normalize(viewPos - hit.p);
      float highlight = dot(viewDir, unitr);
      if (highlight < 0) { highlight = 0; }
      color spec = ks * pow(highlight, shininess) * specColor;

      attenuation = ambient + diffuse + spec;
      return false;
  }

public:
  glm::color diffuseColor;
  glm::color specColor;
  glm::color ambientColor;
  glm::point3 lightPos;
  glm::point3 viewPos; 
  float kd; 
  float ks;
  float ka; 
  float shininess;
};

class metal : public material {
public:
   metal(const glm::color& a, float f) : albedo(a), fuzz(glm::clamp(f,0.0f,1.0f)) {}

   virtual bool scatter(const ray& r_in, const hit_record& rec, 
      glm::color& attenuation, ray& scattered) const override 
   {
       vec3 reflected = reflect(normalize(r_in.direction()), normalize(rec.normal));
       scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
       attenuation = albedo;
       return (dot(scattered.direction(), rec.normal) > 0);
   }

public:
   glm::color albedo;
   float fuzz;
};

class dielectric : public material {
public:
  dielectric(float index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
   {
      attenuation = color(1);
      float refraction_ratio = ir;
      if (rec.front_face) {
          refraction_ratio = 1 / ir;
      }
      vec3 udir = normalize(r_in.direction());
      vec3 unorm = normalize(rec.normal);

      float costheta = fmin(dot(-udir, unorm), 1.0);
      float sintheta = sqrt(1.0 - pow(costheta, 2));

      double r0 = pow((1 - refraction_ratio) / (1 + refraction_ratio), 2);
      double shlick = r0 + (1 - r0) * pow((1 - costheta), 5);


      if (refraction_ratio * sintheta > 1.0f || shlick > random_float(0, 1)) {
          vec3 reflected = reflect(udir, unorm);
          scattered = ray(rec.p, reflected);
          return (dot(scattered.direction(), rec.normal) > 0);
      }
      else {
          vec3 rperp = refraction_ratio * (udir + costheta * unorm);
          float lperp = length(rperp);
          float lpar = -sqrt(abs(1.0f - pow(lperp, 2)));
          vec3 rpar = lpar * unorm;
          vec3 refracted = rperp + rpar;

          scattered = ray(rec.p, refracted);
          return true;
      }
   }

public:
  float ir; // Index of Refraction
};


#endif

