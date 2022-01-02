project(Core)

add_executable(${PROJECT_NAME}
        common.h
        math/vec.h
        math/ray.h
        math/mat.h
        math/transform.h
        math/quadratic.h
        math/aabb.h math/aabb.cpp
        primitives/intersectionInfo.h primitives/intersectionInfo.cpp
        primitives/primitive.h
        primitives/sphere.cpp primitives/sphere.h
        camera/camera.h
        scene.h
        film/film.cpp film/film.h
        light/light.cpp light/light.h
        integrators/integrator.cpp integrators/integrator.h
        accel/accelerator.h
        spectrum/spectrum.cpp spectrum/spectrum.h
        camera/perspective_camera.cpp camera/perspective_camera.h
        sampler/samples.h
        beart.cpp
        accel/bvh.cpp accel/bvh.h accel/bvh_util.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        .
        math
        primitives
        film
        light
        camera
        accel
        spectrum
        sampler
        )
target_link_libraries(Core PUBLIC eigen)
