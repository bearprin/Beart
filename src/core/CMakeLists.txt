set(BERT_CORE_SOURCES)
list(
        APPEND
        BERT_CORE_SOURCES
        main.cpp
        common.h scene.h
        math/vec.h math/ray.h math/interection.h math/interection.cpp math/bbox.h math/aabb.cpp math/aabb.h math/transform.h math/matrix.h
        io/json_serializable.h
        shape/sphere.cpp shape/sphere.h shape/shape.h shape/primitive.h
        camera/camera.h camera/perspective_camera.h camera/perspective_camera.cpp
        film/film.h
        sampler/sampler.h
        light/light.h light/point_light.h light/point_light.cpp
        accelerator/accelerator.h
        material/bxdf_common.h material/bxdf.h material/normal.h
)

add_executable(Beart ${BERT_CORE_SOURCES})
target_include_directories(Beart PRIVATE . math shape io camera film sampler spectrum light material accelerator)
target_link_libraries(Beart PRIVATE ${BERT_PRIVATE_LIBS})