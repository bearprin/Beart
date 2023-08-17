//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "vec.h"
#include "transform.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace beart {
using json = nlohmann::json;
}
namespace nlohmann {
template<class T, unsigned long N>
inline void from_json(const json &j, enoki::Array<T, N> &v) {
  if (j.is_object()) {
    spdlog::error("Can't parse a Vec{}. Expecting a json array, but got a json object.", N);
    return;
  }
  if (j.size() == 1) {
    if (j.is_array())
      spdlog::info("Incorrect array size when trying to parse a Vec3. "
                   "Expecting {} values but only found 1. "
                   "Creating a Vec of all '{}'s.",
                   N, j.get<T>());
    v = j.get<T>();
    return;
  } else if (N != j.size()) {
    spdlog::error("Incorrect array size when trying to parse a Vec. "
                  "Expecting {} values but found {} here:\n{}",
                  N, (int) j.size(), j.dump(4));
  }
  // j.size() == N
  for (auto i = 0u; i < N; ++i) {
    j.at(i).get_to(v[i]);
  }
}
template<class T, unsigned long N>
inline void to_json(json &j, const enoki::Array<T, N> &v) {
  j = json::array();
  for (auto i = 0u; i < 3; ++i) {
    j.push_back(v[i]);
  }
}
inline void from_json(const json &j, beart::Transform &m) {
  auto single_parse = [](const json &j) -> beart::Transform {
    beart::Transform m;
    if (j.count("o") || j.count("x") || j.count("y") || j.count("z")) {
      beart::Vec3f x(1, 0, 0);
      beart::Vec3f y(0, 1, 0);
      beart::Vec3f z(0, 0, 1);
      beart::Vec3f o(0, 0, 0);
      x = j.value("x", x);
      y = j.value("y", y);
      z = j.value("z", z);
      o = j.value("o", o);
      m = beart::Transform(beart::FromColVec4f(beart::Vec4f{x.x(), x.y(), x.z(), 0},
                                               beart::Vec4f{y.x(), y.y(), y.z(), 0},
                                               beart::Vec4f{z.x(), z.y(), z.z(), 0},
                                               beart::Vec4f{o.x(), o.y(), o.z(), 1}));
    } else if (j.count("translate")) {
      m = beart::Translate(j.value("translate", beart::Vec3f(0.f)));
    } else if (j.count("scale")) {
      m = beart::Scale(j.value("scale", beart::Vec3f(1.f)));
    } else if (j.count("rotate")) {
      auto vec4 = j.value("rotate", beart::Vec4f{0, 0, 0, 0});
      auto axis = beart::Vec3f{vec4.y(), vec4.z(), vec4.w()};
      auto angle = vec4.x();
      m = beart::Rotate(axis, angle);
    } else if (j.count("matrix")) {
      json jm = j.at("matrix");
      if (jm.size() == 1) {
        m = beart::Transform(jm.get<float>());
        spdlog::warn("Incorrect array size when trying to parse a Matrix. "
                     "Expecting 4 x 4 = 16 values but only found a single scalar. "
                     "Creating a 4 x 4 scaling matrix with '{}'s along the diagonal.",
                     jm.get<float>());
      } else if (16 != jm.size()) {
        spdlog::error("Incorrect array size when trying to parse a Matrix. "
                      "Expecting 4 x 4 = 16 values but found {}, here:\n{}.",
                      jm.size(), jm.dump(4));
      }
      // jm.size() == 16
      for (auto i = 0u; i < 4; ++i) {
        for (auto k = 0u; k < 4; ++k) {
          m.matrix_(i, k) = jm.at(i * 4 + k).get<float>();
        }
      }
    }
    return m;
  };
  if (j.is_array()) {
    for (auto trans : j) {
      m = single_parse(trans) * m;
    }
  } else if (j.is_object()) {
    m = single_parse(j);
  } else {
    spdlog::error("Can't parse a Matrix. Expecting a json array or object, but got a json {}.", j.type_name());
  }
}
inline void to_json(json &js, const beart::Transform &m) {
  js = json::array();
  for (auto i = 0u; i < 4; ++i) {
    for (auto j = 0u; j < 4; ++j) {
      js.push_back(m.matrix_(i, j));
    }
  }
}
}
