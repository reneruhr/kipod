

## Kipod

Kipod is a small engine for various rendering tasks and math visualizations.
It is departmentalized in the following modules:

#### Rendering:
* Wireframes with OpenGL (GPU)
* Wireframes with software renderer (CPU)
* 2D Geometry with simple shapes (Cube, Octagon, ...)

#### Math Visualization:
* Quasicrystals as cut and project sets from lattices in 4D and 2D internal/external projections.
  
#### Architecture
Kipod is written in C++17. Rendering happens with OpenGL via GLEW. The UI is handled by GLFW and Dear ImGUI.



## Instructions for building from Source for Linux, Mac, Windows:

Building uses cmake and git to receive the source.

### Ubuntu

#### Step 1:
Install necessary packages:

$ sudo apt install build-essential libgl1-mesa-dev cmake git 
$ sudo apt install libglfw3-dev glew-utils libglm-dev

#### Step 2: 
Clone and Building Instruction:

$ git clone https://github.com/reneruhr/kipod.git
$ cd kipod
$ mkdir build
$ cd build
$ cmake ..
$ make
& ./kipod


### MacOS (Intel CPU)

#### Step 1
Install homebrew if needed (which may result in automatically installing the xcode command line developer tools):

$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

#### Step 2
Then install cmake and git and necessary libraries glew, glfw3, glm:

$ brew install git cmake glew glfw3 glm

#### Step 3: 
See Step 2 Ubuntu.


### Windows

#### Step 1
Get git e.g. via https://gitforwindows.org/.
The required libraries are included as git submodules. Use the tag --recursive to add these:

$ git clone --recursive https://github.com/reneruhr/kipod.git

#### Step 2
Get Visual Studio 2019 and its "C++ CMake tools for Windows" component. It is already installed with the 
"Desktop development with C++" and "Linux Development with C++" workload.
Opening the folder will detect the cmake file automatically and you can compile after selecting the "Startup Item" kipod.exe.

