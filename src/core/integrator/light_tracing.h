//
// Created by Bear on 2023/8/14.
//

#pragma once

#include "bid_tracing.h"
namespace beart {
class LightTracing : public BidPathTracing {
 public:
  LightTracing() : BidPathTracing(true) {}
  LightTracing(const json &j) : BidPathTracing(j) {
    sample_per_pixel_ = j.value("sample_per_pixel", 1);
    light_tracing_only_ = true;
  }
};
}
