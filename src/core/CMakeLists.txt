project(Core)

add_executable(${PROJECT_NAME}
        common.h
        math/Vec.h
        math/Ray.h
        math/Mat.h
        math/Transform.h
        math/Quadratic.h
        math/AABB.h math/AABB.cpp
        primitives/IntersectionInfo.h primitives/IntersectionInfo.cpp
        primitives/Primitive.h
        primitives/Sphere.cpp primitives/Sphere.h
        camera/Camera.h
        Scene.h
        film/Film.cpp film/Film.h
        light/Light.cpp light/Light.h
        integrators/Integrator.cpp integrators/Integrator.h
        accel/Accelerator.h
        spectrum/Spectrum.cpp spectrum/Spectrum.h
        camera/PerspectiveCamera.cpp camera/PerspectiveCamera.h
        sampler/Samples.h
        Beart.cpp
        accel/BVH.cpp accel/BVH.h)

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
