//
// Created by Bear on 2023/8/11.
//

#include "bid_tracing.h"
#include "light.h"
#include "camera.h"
#include "factory.h"
beart::Spectrum beart::BidPathTracing::Li(const beart::Ray &ray,
                                          const beart::Scene &scene,
                                          const beart::Sampler &sampler,
                                          beart::Spectrum *normal,
                                          beart::Spectrum *albedos) const {
  auto camera = scene.camera();
  float multi_light_pdf{};
  const auto multi_light_sample = LightSample(sampler.Next2D(), sampler.Next1D());
  auto light = scene.SampleLight(multi_light_sample.t_, &multi_light_pdf); // sample multiple light sources
  if (multi_light_pdf <= 0.f && !light) {
    return {0.f};
  }
  // sample an outgoing ray from the light
  auto ls_pos = LightSample(sampler.Next2D(), sampler.Next1D());
  auto ls_dir = LightSample(sampler.Next2D(), sampler.Next1D());
  Ray r;
  float pdf_solid{};
  float pdf_area{};
  float cos_light{};
  auto light_le = light->SampleLi(ls_pos, ls_dir, r, &pdf_solid, &pdf_area, &cos_light);
  // map r to image for debugging
  auto wi = r;
  wi.is_primary_ray_ = true;
  wi.depth_ = 0;
  // emission =
  auto throughput = light_le * cos_light / (pdf_solid * multi_light_pdf);  // G / (pdf_solid * multi_light_pdf)
  std::vector<PathVertex> light_path_vertices;
  float rr{1.f};
  while (true) {
    if (light_path_vertices.size() > max_depth_) {
      break;
    }
    PathVertex v;
    if (!scene.Intersect(wi, &v.interection_info)) {
      break;
    }
    rr = 1.0;
    if (Intensity(throughput) < 0.01f) {
      rr = 0.5f;
    }
    // from light get the intersection
    v.p = v.interection_info.intersect_pos;
    v.normal = v.interection_info.Ns;
    v.wi = -wi.dir_;
    v.depth = light_path_vertices.size();
    v.throughput = throughput;
    v.rr = rr;
    v.event_ptr = std::make_unique<Event>(v.interection_info);

    light_path_vertices.push_back(v);

    // evaluate the light
    if (sampler.Next1D() > rr) {
      break;
    }
    if (v.interection_info.primitive->bxdfs()->empty()) {
      break;
    }
    // Next-event estimation at each vertex
    ConnectCamera(v, *light, scene, sampler);
    // sample a direction
    float bsdf_pdf{};
    const auto bs = BsdfSample(sampler.Next2D(), sampler.Next1D());
    auto f = v.event_ptr->SampleF(v.wi, v.wo, bs, &bsdf_pdf);  // sample bsdf get wo and pdf
    if (bsdf_pdf == 0.f || IsBlack(f)) {
      break;
    }
    bsdf_pdf *= rr;
    throughput *= f / bsdf_pdf;  // update throughput
    // build next ray
    wi = Ray(v.interection_info.intersect_pos, v.wo, light_path_vertices.size(), true, kEpsilon);
  }


  // path from camera
  Ray temp_r = ray;  // initialize wi as the ray from camera
  throughput = 1.0f;
  rr = 1.0f;
  float local_bounce{};  // length of path actually
  Spectrum li{0.f};
  while (local_bounce < max_depth_) {
    PathVertex v;
    v.depth = local_bounce;
    if (!scene.Intersect(temp_r, &v.interection_info)) {
      if (local_bounce == 0) { //
        li += scene.Le(temp_r);
      }
      break; // no intersection
    }
    // Path evaluation: it hits a light source
    if (v.interection_info.primitive->light() == light) {
      if (v.depth > 0 && v.depth <= max_depth_) {
        float emission_pdf_s;
        float direct_pdf_a;
        Spectrum
            li = v.interection_info.Le(-temp_r.dir_, &direct_pdf_a, &emission_pdf_s) * throughput / multi_light_pdf;
      } else if (v.depth == 0) {
        li = li + v.interection_info.Le(-temp_r.dir_) / multi_light_pdf;
      }
    }
    if (light_tracing_only_) {
      return li;
    }
    if (v.interection_info.primitive->bxdfs()->empty()) {
      break;
    }
    rr = 1.0f;
    if (Intensity(throughput) < 0.01f) {
      rr = 0.5f;
    }

    // get the intersection from camera
    v.p = v.interection_info.intersect_pos;
    v.normal = v.interection_info.Ns;
    v.wi = -temp_r.dir_;
    v.throughput = throughput;
    v.rr = rr;
    v.event_ptr = std::make_unique<Event>(v.interection_info);

    // NNE at each vertex
    li = li + ConnectLight(v, *light, scene, sampler) / multi_light_pdf;

    // Connect vertices for bidirectional path tracing
    for (const auto &pv : light_path_vertices) {
      li = li + ConnectVertices(pv, v, *light, scene, sampler);
    }
    local_bounce += 1;

    // Russian Roulette
    if (sampler.Next1D() > rr)
      break;

    auto bsdf_sample = BsdfSample(sampler.Next2D(), sampler.Next1D());
    float bsdf_pdf{};
    const auto f = v.event_ptr->SampleF(v.wi, v.wo, bsdf_sample, &bsdf_pdf);
    if (IsBlack(f) || bsdf_pdf == 0.f) {
      break;
    }
    bsdf_pdf *= rr;
    throughput *= f / bsdf_pdf;
    if (IsBlack(throughput)) {
      break;
    }
    temp_r = Ray(v.interection_info.intersect_pos, v.wo, local_bounce, true, kEpsilon);
  }
  return li;
}
void beart::BidPathTracing::ConnectCamera(const beart::PathVertex &light_vertex,
                                          const beart::Light &light,
                                          const beart::Scene &scene,
                                          const beart::Sampler &sampler) const {
  auto camera = scene.camera();
  float cos_camera{};
  float pdf_solid{};
  float pdf_area{};
  Spectrum we{};
  Point3f eye_p{};
  Visibility visibility(scene);
  Vec2i raster_coord = camera->GetScreenCoord(light_vertex.interection_info,
                                              visibility,
                                              sampler,
                                              cos_camera,
                                              &pdf_solid,
                                              &pdf_area,
                                              &we,
                                              &eye_p);
  if (raster_coord.x() < 0.f || raster_coord.x() >= camera->image_width() ||
      raster_coord.y() < 0.f || raster_coord.y() >= camera->image_height() || pdf_solid <= 0.f) {
    return;
  }

  Vec3f delta = light_vertex.p - eye_p;
  float dist = Norm(delta);
  float inv_sq_len = 1.0f / (norm(delta) * norm(delta));
  delta /= dist;
  if (Dot(delta, Normalize(camera->forward())) < 0.f) {  // check direction
    return;
  }
  if (light_vertex.interection_info.primitive->bxdfs()->empty()) {
    return;
  }
  auto bsdf_value = light_vertex.event_ptr->EvaluateBxDF(light_vertex.wi, -delta);
  if (IsBlack(bsdf_value)) {
    return;
  }
  if (!visibility.IsVisible()) {  // check visibility between camera and intersection point
    return;
  }
  float total_pixel_num = static_cast<float>(camera->image_width() * camera->image_height());
  const auto gterm = cos_camera * inv_sq_len;    // the other cos in the g-term is hidden in the 'bsdf_value'.
  // f * We * G * throughput
  auto radiance = bsdf_value * we * gterm * light_vertex.throughput / (pdf_area * total_pixel_num * sample_per_pixel_);
  if (!light_tracing_only_) {

  }

  // TODO: optimize this part, can not handle spp > 1 now
  auto color = camera->image()->color(raster_coord.x(), raster_coord.y());
  camera->image()->set_color(raster_coord.x(), raster_coord.y(), (radiance + color));
}
beart::Spectrum beart::BidPathTracing::ConnectLight(const beart::PathVertex &camera_vertex,
                                                    const beart::Light &light,
                                                    const beart::Scene &scene,
                                                    const beart::Sampler &sampler) const {
  if (camera_vertex.depth > max_depth_) {
    return {0.f};
  }
  auto light_sample = LightSample(sampler.Next2D(), sampler.Next1D());
  Visibility vis_test{scene};
  auto direct_pdf = 0.f;
  auto emisson_pdf = 0.f;
  auto bsdf_pdf = 0.f;
  auto distance{0.f};
  auto cos_light{0.f};
  Vec3f wi;
  // light sampling based solid angle
  auto li = light.SampleLi(camera_vertex.interection_info,
                           light_sample,
                           &wi,
                           &direct_pdf,
                           &emisson_pdf,
                           &distance,
                           &cos_light,
                           &vis_test);
  if (direct_pdf == 0.f || !vis_test.IsVisible()) {
    return {0.f};
  }
  // direct light
  li = li * camera_vertex.throughput * camera_vertex.event_ptr->EvaluateBxDF(camera_vertex.wi, wi) / direct_pdf;
  if (IsBlack(li)) {
    return {0.f};
  }
  return li;
}
beart::Spectrum beart::BidPathTracing::ConnectVertices(const beart::PathVertex &light_vertex,
                                                       const beart::PathVertex &camera_vertex,
                                                       const beart::Light &light,
                                                       const beart::Scene &scene,
                                                       const beart::Sampler &sampler) const {
  if (light_vertex.depth + camera_vertex.depth > max_depth_) {
    return {0.f};
  }
  if (light_vertex.interection_info.primitive->bxdfs()->empty()
      || camera_vertex.interection_info.primitive->bxdfs()->empty()) {
    return {0.f};
  }

  auto delta = light_vertex.p - camera_vertex.p;
  auto len = Norm(delta);
  auto inv_sq_len = 1.0f / SquareNorm(delta);
  auto delta_normalize = delta / len;

  Spectrum G = light_vertex.event_ptr->EvaluateBxDF(light_vertex.wi, -delta_normalize) *
      camera_vertex.event_ptr->EvaluateBxDF(camera_vertex.wi, delta_normalize) * inv_sq_len;

  if (IsBlack(G)) {
    return {0.f};
  }
  float cos_light_v = AbsDot(delta_normalize, light_vertex.normal);
  float cos_camera_v = AbsDot(delta_normalize, camera_vertex.normal);
  auto light_vertex_bsdf_pdf_solid = light_vertex.event_ptr->Pdf(light_vertex.wi, -delta_normalize);
  auto light_vertex_bsdf_pdf_solid_rev = light_vertex.event_ptr->Pdf(delta_normalize, light_vertex.wi);
  auto camera_vertex_bsdf_pdf_solid = camera_vertex.event_ptr->Pdf(camera_vertex.wi, delta_normalize);
  auto camera_vertex_bsdf_pdf_solid_rev = camera_vertex.event_ptr->Pdf(delta_normalize, camera_vertex.wi);

  float light_vertex_pdf_area = camera_vertex_bsdf_pdf_solid * cos_light_v * inv_sq_len;
  float camera_vertex_pdf_area = light_vertex_bsdf_pdf_solid * cos_camera_v * inv_sq_len;

//  float weight = (light_vertex_pdf_area * camera_vertex_pdf_area);
  float weight = 1;

  auto li = light_vertex.throughput * camera_vertex.throughput * G * weight;

  if (IsBlack(li)) {
    return {0.f};
  }

  Visibility visibility(scene);
  visibility.ray_ =
      Ray(light_vertex.p,
          delta_normalize,
          light_vertex.depth + camera_vertex.depth + 1,
          kEpsilon,
          kEpsilon,
          len - kEpsilon);
  if (!visibility.IsVisible()) {
    return 0.0f;
  }
  return li;
}
BEART_REGISTER_CLASS_IN_FACTORY(Integrator, BidPathTracing, "bdpt")

