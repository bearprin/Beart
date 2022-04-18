//
// Created by Bear on 2021/12/3.
//

#pragma once

#include <vector>

#include <memory>
#include <utility>

#include <cmath>

namespace beart {
static constexpr float kMinFloat = std::numeric_limits<float>::lowest();
static constexpr float kMaxFloat = std::numeric_limits<float>::max();
static constexpr float kInfinity = std::numeric_limits<float>::infinity();
static constexpr float kPi = 3.1415926535897932384626433832795028f;
static constexpr float kInvPi = 0.3183098861837906715377675267450287f;
static constexpr float kInvTwoPi = 0.159154943091895335768883763372514f;

static constexpr float kEpsilon = 1e-6f;
static constexpr unsigned int kMaxBxdf = 8;
// forward declare
class Primitive;
class Light;

}