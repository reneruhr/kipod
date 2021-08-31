

## Kipod

Kipod is a small engine for various rendering tasks and math visualizations.
It is departmentalized in the following modules:

#### Rendering:
* Meshmodels https://github.com/reneruhr/meshes

#### Math Visualization:
* Quasicrystals https://github.com/reneruhr/quacry
  
#### Architecture
Kipod is written in C++17. Rendering happens with OpenGL. The UI is handled by GLFW and Dear ImGUI.



## Instructions for building from Source for Linux, Mac, Windows:

[The following describes the build for the old standalone version of kipod and will fail since path names have been changed. 
Instead, follow these instructions but with a specific module listed above.]

Building uses cmake and git to receive the source. Dependencies (GLM, GLFW and GLEW) are installed via a package manager (Unix) or can be cloned as submodules.

### Ubuntu

#### Step 1:
Install necessary packages:

```console
~$ sudo apt install build-essential libgl1-mesa-dev cmake git 
~$ sudo apt install libglfw3-dev glew-utils libglm-dev
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
~$ ./kipod
```


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


### Windows (Visual Studio 2019)

#### Step 1
Get git e.g. via https://gitforwindows.org/.
The required libraries are included as git submodules. Use the tag --recursive to add these when cloning:

```console
~$ git clone --recursive https://github.com/reneruhr/kipod.git
```

#### Step 2
It is convenient to use the Visual Studio 2019 "C++ CMake tools for Windows" component. It is already installed with the 
"Desktop development with C++" and "Linux Development with C++" workload.
Opening the folder will detect the cmake file automatically and you can compile after selecting the "Startup Item" kipod.exe.

