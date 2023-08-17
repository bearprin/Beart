//
// Created by Bear on 2023/8/13.
//

#pragma once

#include "interection.h"
#include "event.h"
namespace beart {
struct PathVertex {
  PathVertex() = default;
  PathVertex(const PathVertex &vertex) {
    wi = vertex.wi;
    wo = vertex.wo;
    p = vertex.p;
    normal = vertex.normal;
    interection_info = vertex.interection_info;
    event_ptr = std::make_unique<Event>(interection_info);
    throughput = vertex.throughput;
    depth = vertex.depth;
    rr = vertex.rr;
  }
  Vec3f wi;  // incident direction
  Vec3f wo;  // outgoing direction
  Point3f p;  // position
  Vec3f normal;  // normal of the position

  SurfaceInterection interection_info;
  std::unique_ptr<Event> event_ptr;
  Spectrum throughput;
  size_t depth = 0;
  float rr{};

};
};