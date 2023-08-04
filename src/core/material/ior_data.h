//
// Created by Bear on 2023/8/1.
//

#pragma once

#include "vec.h"

#include <vector>
#include <tuple>
namespace beart {
// copyed from Mitsuba
static const std::vector<std::tuple<std::string, float>> IORData = {
    {"vacuum", 1.0f},
    {"helium", 1.000036f},
    {"hydrogen", 1.000132f},
    {"air", 1.000277f},
    {"carbon dioxide", 1.00045f},
    //////////////////////////////////////
    {"water", 1.3330f},
    {"acetone", 1.36f},
    {"ethanol", 1.361f},
    {"carbon tetrachloride", 1.461f},
    {"glycerol", 1.4729f},
    {"benzene", 1.501f},
    {"silicone oil", 1.52045f},
    {"bromine", 1.661f},
    //////////////////////////////////////
    {"water ice", 1.31f},
    {"fused quartz", 1.458f},
    {"pyrex", 1.470f},
    {"acrylic glass", 1.49f},
    {"polypropylene", 1.49f},
    {"bk7", 1.5046f},
    {"sodium chloride", 1.544f},
    {"amber", 1.55f},
    {"pet", 1.5750f},
    {"diamond", 2.419f},
    {"glass", 1.504f}
};
static float LoadIORData(std::string_view name) {
  auto iter = std::find_if(std::begin(IORData), std::end(IORData), [name](const auto &data) {
    return std::get<0>(data) == name;
  });
  if (iter == std::end(IORData)) {
    // TODO: log warning
    std::cerr << "Warning: " << name << " is not found in DielectricData\n";
    return 1.0;
  }
  return std::get<1>(*iter);
}
}