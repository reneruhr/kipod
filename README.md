

## Kipod

Kipod is a small graphics engine.

#### Employed for
* Quasicrystals https://github.com/reneruhr/quacry
  
#### Architecture
Kipod is written in C++17. Rendering happens with OpenGL. The UI is handled by GLFW and Dear ImGui. 

Other external libraries used: Eigen, glm, catch2, entt, spdlog, stb.

Key handling and framebuffer usage with ImGui is adapted from the Hazel engine (https://github.com/TheCherno/Hazel).

#### Instructions for building from Source for Linux, Mac, Windows:

The cmake will create a library. See the above linked quasicrystal module for an executable.
