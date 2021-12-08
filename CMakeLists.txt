cmake_minimum_required(VERSION 3.21)
project(Beart)

set(CMAKE_CXX_STANDARD 17)


#add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp src/core/math/Vec.h
        src/core/common.h
        src/core/primitives/IntersectionInfo.h src/core/primitives/IntersectionInfo.cpp
        src/core/math/Ray.h
        src/core/primitives/Primitive.cpp src/core/primitives/Primitive.h
        src/core/primitives/Sphere.cpp src/core/primitives/Sphere.h
        src/core/math/Mat.h
        src/core/math/Transform.h
        src/core/camera/Camera.h
        src/core/Scene.h
        src/core/film/Film.cpp src/core/film/Film.h
        src/core/light/Light.cpp src/core/light/Light.h
        src/core/integrators/Integrator.cpp src/core/integrators/Integrator.h
        src/core/accel/Accelerator.cpp src/core/accel/Accelerator.h
        src/core/spectrum/Spectrum.cpp src/core/spectrum/Spectrum.h
        src/core/math/Quadratic.h
        src/core/camera/PerspectiveCamera.cpp src/core/camera/PerspectiveCamera.h
        src/core/sampler/Samples.h
        )


target_include_directories(${PROJECT_NAME} PUBLIC
        src/core/
        src/core/math
        src/core/primitives
        src/core/film
        src/core/light
        src/core/camera
        src/core/accel
        src/core/spectrum
        src/core/sampler
        )

target_include_directories(${PROJECT_NAME} PUBLIC third_party/Eigen)
