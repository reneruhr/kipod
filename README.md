

## Kipod

Kipod is a small graphics engine.

#### Employed for
* Quasicrystals https://github.com/reneruhr/quacry
* Lorenz Attractors Quasicrystals https://github.com/reneruhr/lorenz
* 
#### Architecture
Kipod is written in C++17. Rendering happens with OpenGL.

External libraries used: Eigen, glm, catch2, entt, spdlog, stb, GLFW, Dear ImGui

#### Instructions for building from Source for Linux, Mac, Windows:

The cmake will create a library. 

Building uses cmake and git to receive the source. Dependencies are GLM, GLFW and GLEW, and are installed via a package manager (on Mac/Linux) or can be cloned as submodules ( git clone https://github.com/reneruhr/kipod.git --recursive )

### Ubuntu

#### Step 1:
Install necessary packages:

```console
~$ sudo apt install build-essential libgl1-mesa-dev cmake git 
~$ sudo apt install libglfw3-dev glew-utils libglm-dev libglew-dev
```

#### Step 2: 
Clone and Building Instruction:

```console
~$ git clone https://github.com/reneruhr/kipod.git
~$ cd kipod
~$ mkdir build
~$ cd build
~$ cmake ..
~$ make

### MacOS (Intel/ARM)

#### Step 1
Install homebrew (https://brew.sh/) and xcode command line developer tools:

```console
~$ xcode-select --install
~$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
#### Step 2
Then install cmake, git and the necessary libraries glew, glfw3, glm:

```console
~$ brew install git cmake glew glfw3 glm
```

#### Step 3: 
See Step 2 Ubuntu.




