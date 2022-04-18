project(Core)

add_executable(${PROJECT_NAME}
        common.h
        math/vec.h
        math/ray.h
        math/mat.h
        math/transform.h
        math/quadratic.h
        math/aabb.h math/aabb.cpp
        primitive/intersection_info.h
        primitive/shape.h
        primitive/sphere.cpp primitive/sphere.h
        camera/camera.h
        camera/perspective_camera.cpp camera/perspective_camera.h
        scene.h
        film/film.cpp film/film.h
        light/light.cpp light/light.h
        integrator/integrator.cpp integrator/integrator.h
        accel/accelerator.h
        spectrum/spectrum.cpp spectrum/spectrum.h
        sampler/samples.h
        beart.cpp
        accel/bvh.cpp accel/bvh.h accel/bvh_common.h
        light/point_light.h light/point_light.cpp
        light/area_light.h
        integrator/path_tracing.cpp integrator/path_tracing.h
        primitive/intersection_info.h primitive/intersection_info.cpp
        primitive/primitive.h
        material/material.cpp material/material.h
        integrator/integrator_common.h
        integrator/direct.cpp integrator/direct.h material/bxdf.cpp material/bxdf.h material/event.cpp material/event.h
        material/phong.cpp material/phong.h
        material/bxdf_common.h material/normal.h)

add_library(${PROJECT_NAME}_lib SHARED
        common.h
        math/vec.h
        math/ray.h
        math/mat.h
        math/transform.h
        math/quadratic.h
        math/aabb.h math/aabb.cpp
        primitive/intersection_info.h
        primitive/shape.h
        primitive/sphere.cpp primitive/sphere.h
        camera/camera.h
        camera/perspective_camera.cpp camera/perspective_camera.h
        scene.h
        film/film.cpp film/film.h
        light/light.cpp light/light.h
        integrator/integrator.cpp integrator/integrator.h
        accel/accelerator.h
        spectrum/spectrum.cpp spectrum/spectrum.h
        sampler/samples.h
        beart.cpp
        accel/bvh.cpp accel/bvh.h accel/bvh_common.h
        light/point_light.h light/point_light.cpp
        light/area_light.h
        integrator/path_tracing.cpp integrator/path_tracing.h
        primitive/intersection_info.h primitive/intersection_info.cpp
        primitive/primitive.h
        material/material.cpp material/material.h
        integrator/integrator_common.h
        integrator/direct.cpp integrator/direct.h material/bxdf.cpp material/bxdf.h material/event.cpp material/event.h
        material/phong.cpp material/phong.h
        material/bxdf_common.h material/normal.h)

target_include_directories(${PROJECT_NAME}_lib PUBLIC
        .
        math
        primitive
        film
        light
        camera
        accel
        spectrum
        sampler
        material
        integrator
        )
target_include_directories(${PROJECT_NAME} PRIVATE
        .
        math
        primitive
        film
        light
        camera
        accel
        spectrum
        sampler
        material
        integrator
        )

target_link_libraries(Core PUBLIC eigen)
target_link_libraries(Core_lib PUBLIC eigen)
if (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_sources(${PROJECT_NAME} PUBLIC ${BACKWARD_ENABLE})
    add_backward(Core)
endif ()

