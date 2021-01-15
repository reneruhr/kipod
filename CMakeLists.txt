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

  include/core.h
  include/utils/mat.h
  include/utils/vec.h
  
  include/utils/log.h
  src/utils/log.cpp

  include/utils/image_loader.h

  include/texture.h
  src/texture.cpp

  include/math/lattice.h
  src/math/lattice.cpp

  include/math/sl.h
  src/math/sl.cpp

  include/math/minkowski_embedding.h
  src/math/minkowski_embedding.cpp

  include/math/polygon.h
  src/math/polygon.cpp

  include/pointset.h
  src/pointset.cpp

  include/shapes.h
  src/shapes.cpp

  src/main.cpp

  include/camera.h
  src/camera.cpp

  include/light.h
  src/light.cpp

  include/initshader.h
  src/initshader.cpp

  include/meshmodel.h
  src/meshmodel.cpp

  include/primmeshmodel.h
  src/primmeshmodel.cpp

  include/render_manager.h
  src/render_manager.cpp

  include/render_object.h
  src/render_object.cpp

  include/renderer.h
  src/renderer.cpp

  include/glrenderer.h
  src/glrenderer.cpp

  include/softrenderer.h
  src/softrenderer.cpp

  include/scene.h
  src/scene.cpp


  include/gui.h
  src/gui.cpp

  include/guielement.h
  src/guielement.cpp

  include/guimodule.h
  src/guimodule.cpp

  include/guimathcontrol.h
  src/guimathcontrol.cpp

  include/window.h
  src/window.cpp

  include/graphicsalgorithms.h
  src/graphicsalgorithms.cpp

  include/inputmanager.h
  src/inputmanager.cpp

  include/eventmanager.h
  src/eventmanager.cpp

  include/quacry.h
  src/quacry.cpp

  ${imgui}
)

target_link_libraries(kipod ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(kipod GLEW::GLEW)
target_link_libraries(kipod glfw)


file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
