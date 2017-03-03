QT -= gui

TARGET = GXEngine
CONFIG += console

TEMPLATE = app

INCLUDEPATH += \
    ../Include

HEADERS += \
    ../Include/GXEngine/opengl.h \
    ../Include/GXEngine/renderer.h \
    ../Include/GXEngine/renderer_thread.h \
    ../Include/GXEngine/shader_program.h \
    ../Include/GXEngine/renderable.h \
    ../Include/GXEngine/camera.h \
    ../Include/GXEngine/perspective_camera.h \
    ../Include/GXEngine/orthographic_camera.h \
    ../Include/GXEngine/mesh_geometry.h \
    ../Include/GXEngine/game.h \
    ../Include/GXEngine/texture.h \
    ../Include/GXEngine/material.h \
    ../Include/GXEngine/rigid_body.h \
    ../Include/GXEngine/physics_shape.h \
    ../Include/GXEngine/sphere_shape.h \
    ../Include/GXEngine/box_shape.h \
    ../Include/GXEngine/physics_simulator.h \
    ../Include/GXCommon/gx_memory.h \
    ../Include/GXCommon/gx_math.h \
    ../Include/GXCommon/gx_log.h \
    ../Include/GXCommon/gx_native_static_mesh.h \
    ../Include/GXCommon/gx_obj_loader.h \
    ../Include/GXCommon/gx_time.h \
    ../Include/GXCommon/gx_image_loader.h \
    ../Include/ParticlesGame/particles_game.h \
    ../Include/ParticlesGame/unlit_material.h \
    ../Include/ParticlesGame/static_mesh.h \
    ../Include/ParticlesGame/cook_torrance_renderer.h \
    ../Include/ParticlesGame/cook_torrance_simple_material.h \
    ../Include/ParticlesGame/cook_torrance_no_normal_map_material.h \
    ../Include/ParticlesGame/light.h \
    ../Include/ParticlesGame/directed_light.h \
    ../Include/ParticlesGame/directed_light_no_shadow_casting_material.h \
    ../Include/ParticlesGame/blur_material.h

SOURCES += \
    ../Sources/GXEngine/main.cpp \
    ../Sources/GXEngine/opengl.cpp \
    ../Sources/GXEngine/renderer.cpp \
    ../Sources/GXEngine/renderer_thread.cpp \
    ../Sources/GXEngine/shader_program.cpp \
    ../Sources/GXEngine/renderable.cpp \
    ../Sources/GXEngine/camera.cpp \
    ../Sources/GXEngine/perspective_camera.cpp \
    ../Sources/GXEngine/orthographic_camera.cpp \
    ../Sources/GXEngine/mesh_geometry.cpp \
    ../Sources/GXEngine/game.cpp \
    ../Sources/GXEngine/texture.cpp \
    ../Sources/GXEngine/material.cpp \
    ../Sources/GXEngine/rigid_body.cpp \
    ../Sources/GXEngine/physics_shape.cpp \
    ../Sources/GXEngine/sphere_shape.cpp \
    ../Sources/GXEngine/box_shape.cpp \
    ../Sources/GXEngine/physics_simulator.cpp \
    ../Sources/GXCommon/gx_math.cpp \
    ../Sources/GXCommon/gx_log.cpp \
    ../Sources/GXCommon/gx_native_static_mesh.cpp \
    ../Sources/GXCommon/gx_obj_loader.cpp \
    ../Sources/GXCommon/gx_time.cpp \
    ../Sources/GXCommon/gx_image_loader.cpp \
    ../Sources/GXCommon/gx_memory.cpp \
    ../Sources/ParticlesGame/particles_game.cpp \
    ../Sources/ParticlesGame/unlit_material.cpp \
    ../Sources/ParticlesGame/static_mesh.cpp \
    ../Sources/ParticlesGame/cook_torrance_renderer.cpp \
    ../Sources/ParticlesGame/cook_torrance_simple_material.cpp \
    ../Sources/ParticlesGame/cook_torrance_no_normal_map_material.cpp \
    ../Sources/ParticlesGame/light.cpp \
    ../Sources/ParticlesGame/directed_light.cpp \
    ../Sources/ParticlesGame/directed_light_no_shadow_casting_material.cpp \
    ../Sources/ParticlesGame/blur_material.cpp

OTHER_FILES += \
    ../Shaders/ParticlesGame/SimpleMesh_vs.txt \
    ../Shaders/ParticlesGame/SimpleMesh_fs.txt \
    ../Shaders/ParticlesGame/Ground_fs.txt \
    ../Shaders/ParticlesGame/Unlit_fs.txt \
    ../Shaders/ParticlesGame/VertexOnly_vs.txt \
    ../Shaders/ParticlesGame/CookTorrance_fs.txt \
    ../Shaders/ParticlesGame/CookTorranceSimple_fs.txt \
    ../Shaders/ParticlesGame/CookTorranceDirectedLightNoShadowCasting_fs.txt \
    ../Shaders/ParticlesGame/CookTorranceNoNormalMap_fs.txt \
    ../Shaders/ParticlesGame/Blur_fs.txt

bin_out.files = debug/GXEngine.exe debug/GXEngine.ilk debug/opengldemo.pdb
bin_out.path = ../bin/Debug/x86
bin_out.CONFIG = no_check_exist
INSTALLS += bin_out

DISTFILES += \
    ../Shaders/ParticlesGame/VertexNormalUVVelocity_vs.txt \
    ../Shaders/ParticlesGame/VertexNormalVelocity_vs.txt
