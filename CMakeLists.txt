cmake_minimum_required(VERSION 3.5)

project(nir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17
                                -Wall
                                -march=native)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/vendor/spdlog/include/)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SPDLOG_DIR})
include_directories(include)

file(GLOB imgui
    "vendor/imgui/*.h"
    "vendor/imgui/*.cpp"
)

add_executable(nir

  include/core.h
  include/utils/mat.h
  include/utils/vec.h
  
  include/utils/log.h
  src/utils/log.cpp





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

target_link_libraries(nir ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(nir libglfw.so.3)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
