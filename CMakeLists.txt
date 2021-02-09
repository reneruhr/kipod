cmake_minimum_required(VERSION 3.5)

project(kipod LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17
                                -Wall
                                -march=native)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/vendor/spdlog/include/)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${glfw3_INCLUDE_DIR} ${SPDLOG_DIR})
include_directories(include)

file(GLOB imgui
    "vendor/imgui/*.h"
    "vendor/imgui/*.cpp"
)

file(GLOB stb
    "vendor/stb/*.h"
)

add_executable(kipod

  src/kipod.h

  src/core.h
  src/utils/mat.h
  src/utils/vec.h
  
  src/utils/buffer_packing.h

  src/utils/log.h
  src/utils/log.cpp

  src/utils/image_loader.h
  src/utils/obj_parsing.h
  src/utils/key_table.h


  src/math/lattice.h
  src/math/lattice.cpp

  src/math/sl.h
  src/math/sl.cpp

  src/math/minkowski_embedding.h
  src/math/minkowski_embedding.cpp

  src/math/polygon.h
  src/math/polygon.cpp

  src/pointset.h
  src/pointset.cpp

  src/shapes.h
  src/shapes.cpp

  src/main.cpp

  src/camera.h
  src/camera.cpp

  src/light.h
  src/light.cpp


  src/meshmodel.h
  src/meshmodel.cpp

  src/primmeshmodel.h
  src/primmeshmodel.cpp


  src/engine/engine_application.h
  src/engine/engine_application.cpp

  src/engine/engine_input.h
  src/engine/engine_input.cpp

  src/engine/engine_events.h
  src/engine/engine_events.cpp

  src/engine/engine_controls.h
  src/engine/engine_controls.cpp

  src/engine/engine_gui.h
  src/engine/engine_gui.cpp

  src/engine/engine_transform.h
  src/engine/engine_transform.cpp

  src/engine/engine_clock.h
  src/engine/engine_clock.cpp


  src/render/initshader.h
  src/render/initshader.cpp

  src/render/render_engine.h
  src/render/render_engine.cpp

  src/render/render_shader.h
  src/render/render_shader.cpp

  src/render/render_manager.h
  src/render/render_manager.cpp

  src/render/render_object.h
  src/render/render_object.cpp

  src/render/render_buffer.h
  src/render/render_buffer.cpp

  src/render/render_uniform.h
  src/render/render_uniform.cpp

  src/render/render_primitive.h
  src/render/render_primitive.cpp

  src/render/render_scene.h
  src/render/render_scene.cpp

  src/render/render_material.h
  src/render/render_material.cpp

  src/render/render_light.h
  src/render/render_light.cpp

  src/render/render_camera.h
  src/render/render_camera.cpp

  src/render/render_texture.h
  src/render/render_texture.cpp

  src/render/render_window.h
  src/render/render_window.cpp


  src/renderer.h
  src/renderer.cpp

  src/glrenderer.h
  src/glrenderer.cpp

  src/softrenderer.h
  src/softrenderer.cpp

  src/scene.h
  src/scene.cpp

  src/gui.h
  src/gui.cpp

  src/guielement.h
  src/guielement.cpp

  src/guimodule.h
  src/guimodule.cpp

  src/guimathcontrol.h
  src/guimathcontrol.cpp



  src/graphicsalgorithms.h
  src/graphicsalgorithms.cpp

  src/inputmanager.h
  src/inputmanager.cpp

  src/eventmanager.h
  src/eventmanager.cpp

  src/quacry.h
  src/quacry.cpp

  ${imgui}
)

target_link_libraries(kipod ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(kipod GLEW::GLEW)
target_link_libraries(kipod glfw)


file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
