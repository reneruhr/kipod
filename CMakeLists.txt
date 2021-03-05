cmake_minimum_required(VERSION 3.5)

#project(kipod LANGUAGES CXX)
project(kipod)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_definitions(-std=c++17
#                               -Wall
#                              -march=native)


include_directories(include)

set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/external_libs/spdlog/include/)
include_directories(${SPDLOG_DIR})


file(GLOB imgui
    "external_libs/imgui/*.h"
    "external_libs/imgui/*.cpp"
)

file(GLOB stb
    "external_libs/stb/*.h"
)

file(GLOB IconsFontAwesome
    "external_libs/IconsFontAwesome/*.h"
)

add_executable(kipod

  src/kipod.h

  src/core.h
  src/utils/mat.h
  src/utils/vec.h
  
  src/utils/buffer_packing.h

  src/utils/log.h
  src/utils/log.cpp

  src/utils/random.h


  src/utils/image_loader.h
  src/utils/image_loader.cpp

  src/utils/obj_parsing.h
  src/utils/key_table.h
  src/utils/imgui_utils.h
  src/utils/imgui_console.h

  src/math/lattice.h
  src/math/lattice.cpp

  src/math/sl.h
  src/math/sl.cpp

  src/math/minkowski_embedding.h
  src/math/minkowski_embedding.cpp

  src/pointset.h
  src/pointset.cpp

  src/main.cpp

  src/camera.h
  src/camera.cpp

  src/light.h
  src/light.cpp






  src/modules/meshmodels/meshmodel_opengl_module.h
  src/modules/meshmodels/meshmodel_opengl_module.cpp
  src/modules/meshmodels/meshmodel_opengl_scene.h
  src/modules/meshmodels/meshmodel_opengl_scene.cpp
  src/modules/meshmodels/meshmodel_sidebar.h
  src/modules/meshmodels/meshmodel_sidebar.cpp
  src/modules/meshmodels/meshmodel_console.h
  src/modules/meshmodels/meshmodel_console.cpp
  src/modules/meshmodels/meshmodel.h
  src/modules/meshmodels/meshmodel.cpp
  src/modules/meshmodels/meshmodel_primitive.h
  src/modules/meshmodels/meshmodel_primitive.cpp
  src/modules/meshmodels/meshmodel_boundingbox.h
  src/modules/meshmodels/meshmodel_boundingbox.cpp
  src/modules/meshmodels/meshmodel_parsing.h
  src/modules/meshmodels/meshmodel_parsing.cpp


  src/modules/shapes/shapes_module.h
  src/modules/shapes/shapes_module.cpp
  src/modules/shapes/shape.h
  src/modules/shapes/shape.cpp
  src/modules/shapes/shapes_polygon.h
  src/modules/shapes/shapes_polygon.cpp
  src/modules/shapes/shapes_scene.h
  src/modules/shapes/shapes_scene.cpp
  src/modules/shapes/shapes_sidebar.h
  src/modules/shapes/shapes_sidebar.cpp



  src/modules/softrenderer/softrenderscene.h
  src/modules/softrenderer/softrenderscene.cpp

  src/modules/quasicrystals/quasicrystals_pointset.h
  src/modules/quasicrystals/quasicrystals_pointset.cpp
  src/modules/quasicrystals/quasicrystal.h
  src/modules/quasicrystals/quasicrystal.cpp
  src/modules/quasicrystals/quasicrystals_scene.h
  src/modules/quasicrystals/quasicrystals_scene.cpp
  src/modules/quasicrystals/quasicrystals_sidebar.h
  src/modules/quasicrystals/quasicrystals_sidebar.cpp
  src/modules/quasicrystals/quasicrystals_console.h
  src/modules/quasicrystals/quasicrystals_console.cpp
  src/modules/quasicrystals/quasicrystals_module.h
  src/modules/quasicrystals/quasicrystals_module.cpp
  src/modules/quasicrystals/quasicrystals_examples.h
  src/modules/quasicrystals/quasicrystals_examples.cpp
  src/modules/quasicrystals/quasicrystals_physical_window.h
  src/modules/quasicrystals/quasicrystals_physical_window.cpp


  src/engine/engine_module.h
  src/engine/engine_module.cpp

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

  src/engine/engine_menu.h
  src/engine/engine_menu.cpp

  src/engine/engine_menu_events.h
  src/engine/engine_menu_events.cpp

  src/engine/engine_sidebar.h
  src/engine/engine_sidebar.cpp

  src/engine/engine_console.h
  src/engine/engine_console.cpp



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

  src/render/render_layout.h
  src/render/render_layout.cpp

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


  src/render/render_framebuffer.h
  src/render/render_framebuffer.cpp

  src/render/opengl/opengl_engine.h
  src/render/opengl/opengl_engine.cpp
  src/render/opengl/opengl_window.h
  src/render/opengl/opengl_window.cpp
  src/render/opengl/opengl_layout.h
  src/render/opengl/opengl_layout.cpp
  src/render/opengl/opengl_buffer.h
  src/render/opengl/opengl_buffer.cpp
  src/render/opengl/opengl_buffer_450.h
  src/render/opengl/opengl_buffer_450.cpp
  src/render/opengl/opengl_buffer_410.h
  src/render/opengl/opengl_buffer_410.cpp



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

    find_package(OpenGL REQUIRED)
    if(NOT OPENGL_FOUND)
        message("No OpenGL found! There is no alternative at this point...")
    endif()
    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(kipod ${OPENGL_LIBRARIES})

    find_package(GLEW)
    find_package(glfw3 3.3)
    find_package(glm)

    if(GLEW_FOUND)
        message("Glew found on machine.")
        include_directories(${GLEW_INCLUDE_DIR})
        target_link_libraries(kipod GLEW::GLEW)
    else()
        message("Adding glew as git submodule. For this to work, you have to clone recursively: git clone --recursive git@github.com:reneruhr/kipod.git ")
        add_definitions(-DGLEW_STATIC)
        add_subdirectory(external_libs/glew-cmake/ EXCLUDE_FROM_ALL)
        target_link_libraries(kipod libglew_static)
    endif()

    if(glfw3_FOUND)
        message("GLFW found on machine.")
        include_directories(${glfw3_INCLUDE_DIR})
        target_link_libraries(kipod glfw)
    else()
        message("Adding glfw as git submodule. For this to work, you have to clone recursively: git clone --recursive git@github.com:reneruhr/kipod.git ")
        add_subdirectory(external_libs/glfw/ EXCLUDE_FROM_ALL)
        target_link_libraries(kipod glfw)
    endif()

    if(glm_FOUND)
        message("GLM found on machine.")
        include_directories(${glm_INCLUDE_DIR})
        target_link_libraries(kipod glm::glm)
    else()
        message("Adding glm as git submodule. For this to work, you have to clone recursively: git clone --recursive git@github.com:reneruhr/kipod.git ")
        add_subdirectory(external_libs/glm/ EXCLUDE_FROM_ALL)
        target_link_libraries(kipod glm::glm)
    endif()


 

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
