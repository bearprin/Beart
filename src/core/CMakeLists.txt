set(BERT_CORE_SOURCES)


list(
        APPEND
        BERT_CORE_SOURCES
        main.cpp
        common.h scene.h scene.cpp
        math/vec.h math/ray.h math/interection.h math/interection.cpp math/bbox.h math/aabb.cpp math/aabb.h math/transform.h math/matrix.h
        io/json_serializable.h
        shape/sphere.cpp shape/sphere.h shape/shape.h shape/primitive.h shape/triangle.cpp shape/triangle.h shape/triangle_mesh.cpp shape/triangle_mesh.h
        shape/quad.cpp shape/quad.h
        camera/camera.h camera/perspective_camera.h camera/perspective_camera.cpp
        film/film.h film/film.cpp
        sampler/sample_common.h sampler/sampler.h sampler/sampler.cpp sampler/random_sampler.h
        light/light.h light/point_light.h light/point_light.cpp light/directional_light.cpp light/directional_light.h light/area_light.h light/area_light.cpp
        light/constant_light.cpp light/constant_light.h
        accelerator/accelerator.h
        accelerator/bvh.h accelerator/bvh.cpp accelerator/bvh_common.h
        material/bxdf.h material/phong.h material/phong.cpp material/event.h material/event.cpp material/microfacet.cpp
        material/microfacet.h material/conductor.cpp material/conductor.h material/conductor_data.h
        material/ior_data.h material/dielectric.cpp material/dielectric.h
        material/diffuse.h material/diffuse.cpp
        integrator/integrator.h integrator/integrator.cpp integrator/integrator_common.h integrator/ao.h integrator/ao.cpp
        integrator/direct.h integrator/direct.cpp integrator/path_tracing.cpp integrator/path_tracing.h
        integrator/bid_tracing.cpp integrator/bid_tracing.h integrator/bidpath/path_vertex.h integrator/light_tracing.cpp integrator/light_tracing.h
        io/scene_parser.cpp io/scene_parser.h io/factory.h
)

add_executable(Beart ${BERT_CORE_SOURCES})
target_include_directories(Beart PRIVATE . math shape io camera film sampler spectrum light material accelerator integrator)
target_link_libraries(Beart PRIVATE ${BERT_PRIVATE_LIBS})
target_compile_definitions(Beart PRIVATE BERT_ENABLE_OIDN)
# change Beart position
set_target_properties(Beart PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (BERT_ENABLE_OIDN)
    set(OIDN_INCLUDE_DIRS ${CMAKE_HOME_DIRECTORY}/third_party/oidn/include)
    target_include_directories(Beart PRIVATE ${OIDN_INCLUDE_DIRS})
    target_link_libraries(Beart PRIVATE OpenImageDenoise OpenImageDenoise_device_cpu)
endif ()