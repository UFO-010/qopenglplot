## Description 

This is just a plain plotter made for real-time plotting with Qt, using OpenGL

## Requirements 

C++11

Qt, version higher than 4

Hardware support for OpenGL version 3.3 or higher

## Usage 

Check if you have supported OpenGL version

```
glxinfo | grep OpenGL
```

If you don't have it installed, install OpenGL dev files and tools

```
sudo apt-get install libgl1-mesa-dev mesa-utils 
```

When you use qmake, you have to link OpenGL manually, for example

```QMake
QT += opengl
CONFIG += opengl

win32: LIBS += -lopengl32
```

You can find example .pro file in folder qmake

When you use cmake (which is recommended), add directory with this library, for example,

```CMake
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sources/openglplot)
```

Then, you have link this library to your project

```CMake
find_library(OPENGLPLOT openglplot PATH_SUFFIXES openglplot)
target_link_libraries(${PROJECT_NAME} PRIVATE openglplot)
```

## TODO 

- Add the ability to put two graphs in one widget
- Improve text rendering
- Improve text characters loading perfomance
- Improve smoothing
- Add power sybols to render numbers with more than 7 characters
- Eliminate text and axis jitter 
- Add framebuffer copy if possible 
- Add custom graph signatures
- Add captions support on the graph
- Add support for saving graph

