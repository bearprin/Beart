#include "Primitive.h"
#include "Sphere.h"
#include "PerspectiveCamera.h"
#include "Samples.h"

#include <iostream>
int main() {
  std::shared_ptr<beart::Primitive> sphere = std::make_shared<beart::Sphere>(beart::Vec3f{0.f, 0.f, -8.f}, 0.5f);
  beart::PerspectiveCamera camera{400, 300};
  FILE *fp = fopen("sphere_normal.ppm", "wb");
  (void) fprintf(fp, "P3\n%d %d\n255\n", 400, 300);
  for (unsigned j = camera.GetImageHeight(); j > 0; --j) {
    for (unsigned int i = 0; i < camera.GetImageWidth(); ++i) {

      beart::Ray r = camera.GenerateRay(i, j, beart::PixelSample{0.f, 0.f});
      std::shared_ptr<beart::IntersectionInfo> info = std::make_shared<beart::IntersectionInfo>();
      sphere->IntersectInfo(r, info);
      if (info->happened) {
        beart::Vec3f nor = (info->Ns + beart::Vec3f(1, 1, 1)) * 0.5;
        (void) fprintf(fp,
                       "%d %d %d\n",
                       static_cast<int>(255.999 * nor.x()),
                       static_cast<int>(255.999 * nor.y()),
                       static_cast<int>(255.999 * nor.z()));
      } else {
        (void) fprintf(fp, "0 0 0\n");
      }
    }
  }
  return 0;
}
