# RGDC OpenGL Examples
If you're reading this, it's already too late. You are doomed to write countless render engines of your own.
## To get this piece of junk working
You will need to install the following packages. 
 - glew 
 - glm
 - glfw3

I recommend using [vcpkg](https://github.com/microsoft/vcpkg). The built in sln files are for VS Community 2022 and will work with the integrate install for vcpkg.
## What is this nightmare?
- Shader.h/.cpp - a small utility class for compiling shaders
- stb_image.h - one file header lib for loading images
- ExampleWindow.cpp - simple window creation
- ExampleTriangle.cpp - making a simple triangle
- ExampleTextures.cpp - loading and mapping textures
- ExampleCameras.cpp - shows using of cameras in renderers
- ChaosIncarnate.cpp - an accursed file
