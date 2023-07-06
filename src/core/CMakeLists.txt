set(BERT_CORE_SOURCES)
list(
        APPEND
        BERT_CORE_SOURCES
        main.cpp
        common.h
        math/vec.h math/ray.h math/interection.cpp math/interection.h math/bbox.h math/aabb.cpp math/aabb.h math/transform.h math/matrix.h
        io/JsonSerializable.cpp io/JsonSerializable.h
        shape/sphere.cpp shape/sphere.h shape/shape.h
        #  # cmake-format: off
        #  # Additional files for PA6 below
        #  include/darts/medium.h
        #  include/darts/photon.h
        #  include/darts/point_kdtree.h
        #  src/photon.cpp
        #  src/media/homogeneous.cpp
        #  src/media/vacuum.cpp
        #  src/tests/photon_map_test.cpp
        #  # Additional files for PA5 below
        #  src/tests/surface_sample_test.cpp
        #  # Additional files for PA4 below
        #  src/samplers/independent.cpp
        #  include/darts/sampler.h
        #  src/tests/material_sample_test.cpp
        #  # Additional files for PA3 below
        #  # Additional files for PA2 below
        #  include/darts/box.h
        #  include/darts/mesh.h
        #  include/darts/triangle.h
        #  src/surfaces/bbh.cpp
        #  src/surfaces/mesh.cpp
        #  src/surfaces/triangle.cpp
        #  src/tests/intersection_test.cpp
        #  # Additional files for PA1 below
        #  include/darts/camera.h
        #  include/darts/factory.h
        #  include/darts/json.h
        #  include/darts/material.h
        #  include/darts/sampling.h
        #  include/darts/scene.h
        #  include/darts/sphere.h
        #  include/darts/stats.h
        #  include/darts/surface.h
        #  include/darts/surface_group.h
        #  include/darts/test.h
        #  include/darts/transform.h
        #  src/camera.cpp
        #  src/example_scenes.cpp
        #  src/parser.cpp
        #  src/scene.cpp
        #  src/stats.cpp
        #  src/materials/dielectric.cpp
        #  src/materials/diffuse_light.cpp
        #  src/materials/lambertian.cpp
        #  src/materials/material.cpp
        #  src/materials/metal.cpp
        #  src/surfaces/quad.cpp
        #  src/surfaces/sphere.cpp
        #  src/surfaces/surface.cpp
        #  src/surfaces/surface_group.cpp
        #  src/tests/material_scatter_test.cpp
        #  src/tests/test.cpp
        #  # The files below are included in the PA0 basecode
        #  include/darts/array2d.h
        #  include/darts/common.h
        #  include/darts/fwd.h
        #  include/darts/image.h
        #  include/darts/math.h
        #  include/darts/parallel.h
        #  include/darts/progress.h
        #  include/darts/ray.h
        #  include/darts/spherical.h
        #  src/common.cpp
        #  src/image.cpp
        #  src/math.cpp
        #  src/progress.cpp
        #  # cmake-format: on
)

add_executable(Beart ${BERT_CORE_SOURCES})
target_include_directories(Beart PRIVATE . math shape io)