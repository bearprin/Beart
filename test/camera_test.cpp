//
// Created by Bear on 2022/4/17.
//
#include "perspective_camera.h"

#include <iostream>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("PerspectiveCamera", "[ProjectionMatrix]") {
  auto camera = beart::PerspectiveCamera{400, 300};
  auto ray = camera.GenerateRay(0, 300, beart::PixelSample{0, 0});
  std::cout << ray.ori().x() << " " << ray.ori().y() << " " << ray.ori().z() << std::endl;
  std::cout << ray.dir().x() << " " << ray.dir().y() << " " << ray.dir().z() << std::endl;
}