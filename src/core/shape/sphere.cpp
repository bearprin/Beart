//
// Created by Bear on 2023/7/6.
//

#include "sphere.h"
#include "sample_common.h"
#include "factory.h"

bool beart::Sphere::Intersect(const beart::Ray &ray) const {
  return Intersect(ray, nullptr);
}
bool beart::Sphere::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  Vec3f L = ray.ori_ - this->center_;
  float a = Dot(ray.dir_, ray.dir_);
  float b = 2.f * Dot(ray.dir_, L);
  float c = Dot(L, L) - radius_ * radius_;
  auto [interect, t0, t1] = SolveQuadratic(a, b, c);
  if (!interect) {
    return false;
  }
  const auto limits = inter ? inter->t_curr : kMaxFloat;
  if (t0 > limits || t1 < 0.f) {
    return false;
  }
  if (t0 < ray.t_min_ && t1 > limits) {
    return false;
  }
  float t{};
  if (t0 > 0.f) {
    t = t0;
  } else if (t1 > limits) {
    return false;
  } else {
    t = t1;
  }
  if (t > ray.t_max_ || t < ray.t_min_) {
    return false;
  }
  if (inter) {
    inter->t_curr = t;
    inter->intersect_pos = ray(t);
    inter->view = -ray.dir_;
    inter->Ns = Normalize((inter->intersect_pos - this->center_));
    inter->Ng = inter->Ns;  // Ng eq Ns since this is an implicit equation
    Vec3f v1;
    Vec3f v2;
    CoordinateSystem(inter->Ns, &v1, &v2); // build local coordinate system with shading normal
    inter->tangent = v1;
    inter->hit = true;
  }
  return true;
}
const beart::AABB &beart::Sphere::bbox() const {
  if (!bbox_) {
    bbox_ = std::make_unique<AABB>(center_ - radius_, center_ + radius_);
  }
  return *bbox_;
}
float beart::Sphere::SurfaceArea() const {
  return 4 * kPi * radius_ * radius_;
}
float beart::Sphere::Volume() const {
  return 4.f / 3.f * kPi * radius_ * radius_ * radius_;
}
beart::Point3f beart::Sphere::SampleDirect(const beart::LightSample &ls,
                                           const beart::Point3f &inter_pos,
                                           beart::Vec3f &wi,
                                           beart::Vec3f *n,
                                           float *pdf_solid) const {
  Vec3f L = center_ - inter_pos; // point to light
  float dist = Norm(L);
  Vec3f dir = Normalize(L);

  // compute solid angle
  float sin_theta_max = radius_ / dist;
  float cos_theta_max = std::sqrtf(std::fmax(0.f, 1.f - sin_theta_max * sin_theta_max)); // sin^2 + cos^2 = 1

  SurfaceInterection inter;
  // sample a direction
  wi = SampleUniformCone(ls.u_, ls.v_, cos_theta_max);
  if (pdf_solid) {
    *pdf_solid = SampleUniformConePdf(cos_theta_max);
  }
  Vec3f v1;
  Vec3f v2;
  CoordinateSystem(dir, &v1, &v2);

  // transform to world coordinate from sphere coordinate
  Transform m{{v1.x(), v2.x(), dir.x(), 0.f,
               v1.y(), v2.y(), dir.y(), 0.f,
               v1.z(), v2.z(), dir.z(), 0.f,
               0.f, 0.f, 0.f, 1.f}};
  wi = m.TransformVector(wi);

  // try to find intersection point (since sample point may not be on the sphere)
  Ray ray{inter_pos, wi, 1, false, kEpsilon, dist - kEpsilon};
  if (!Intersect(ray, &inter)) { // no intersection, sampled outside the sphere, projection to the sphere
    inter.intersect_pos = ray(Dot(L, wi));
  }
  if (n) {
    *n = Normalize(inter.intersect_pos - center_);
  }

  return inter.intersect_pos;
}
float beart::Sphere::DirectPdf(const beart::Point3f &p, const beart::Vec3f &wi) const {
  float dist = Norm(center_ - p);
  float sin_theta_max = radius_ / dist;
  float cos_theta_max = std::sqrtf(std::fmax(0.f, 1.f - sin_theta_max * sin_theta_max)); // sin^2 + cos^2 = 1
  return SampleUniformConePdf(cos_theta_max);
}
void beart::Sphere::SampleDirect(const beart::LightSample &ls_pos,
                                 const beart::LightSample &ls_dir,
                                 beart::Ray &ray,
                                 beart::Vec3f &n,
                                 float *pdf_solid) const {
  // sample direction
  auto ray_ori = SampleUniformSphere(ls_pos.u_, ls_pos.v_) * radius_ + center_;
  auto ray_dir = SampleUniformSphere(ls_dir.v_, ls_dir.u_); // may have some problem
  if (Dot(ray_dir, ray_ori - center_) < 0.f) {
    ray_dir = -ray_dir;
  }
  n = Normalize(ray_ori - center_);
  ray = Ray(ray_ori, ray_dir, ray.depth_, ray.is_primary_ray_, kEpsilon);
  if (pdf_solid) {
    *pdf_solid = 1.f / (8 * kPi * kPi * radius_ * radius_); // based on solid angle
  }
}
BEART_REGISTER_CLASS_IN_FACTORY(Shape, Sphere, "sphere");

