Building from Source Instructions:

MacOS (Intel CPU)

Step 1
Install homebrew if needed (which may result automatically installing the xcode command line developer tools):

$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Step 2
Then install cmake and git and necessary libraries glew, glfw3, glm:

$ brew install git cmake glew glfw3 glm

Step 3: 
Clone and Building Instruction:

$ git clone https://github.com/reneruhr/kipod.git
$ cd kipod
$ mkdir build
$ cd build
$ cmake ..
$ make
& ./kipod


Ubuntu

Step 1

Then install necessary packages:

$ sudo apt install build-essential libgl1-mesa-dev cmake git 
$ sudo apt install libglfw3-dev glew-utils libglm-dev

Step 2: 
See Step 3 for Mac.


Windows

Get git e.g. via https://gitforwindows.org/.
The required libraries are included as git submodules. Use the tag --recursively to add these:

$ git clone --recursive https://github.com/reneruhr/kipod.git

Get Visual Studio 2019 and its "C++ CMake tools for Windows" component. It is already installed with the 
"Desktop development with C++" and "Linux Development with C++" workload.
Opening the folder will detect the cmake file automatically and you can compile after selecting the "Startup Item" kipod.exe.


