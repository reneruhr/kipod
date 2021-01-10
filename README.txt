
MacOS Building from Source:

Step 1: Copy the folder shaders into your home folder.

Step 2: Install homebrew if needed (which may result automatically installing the xcode command line developer tools):

rene$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

Then install cmake (if needed) and the three libraries needed for the application:

rene$ brew install cmake
rene$ brew install glew
rene$ brew install glfw3
rene$ brew install glm

Step 3: Building Instruction:

Open Terminal in the nir folder. 
Create a the build folder.
Move into the build folder.
Run cmake and make.
Run the application.

These commands look like that in the terminal:

rene$ cd kipod
rene$ mkdir build
rene$ cd build
rene$ cmake ..
rene$ make
rene& ./kipod



