CMAKE_MINIMUM_REQUIRED(VERSION 3.18.0)

project(openglplot LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
option(${PROJECT_NAME}_SANITIZE "Build with Leak Sanitizer" OFF)
option(${PROJECT_NAME}_BUILD_SHARED_LIBS "Build using shared libraries" ON)



# specify the C++ standard
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_c INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_cxx INTERFACE)

target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_c)
target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_cxx)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_11)


find_package(OpenGL REQUIRED)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGL REQUIRED)
if(${QT_VERSION_MAJOR} MATCHES "6")
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGLWidgets REQUIRED)
endif()


set(gcc_like_c "$<COMPILE_LANG_AND_ID:C,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(${PROJECT_NAME}_compiler_flags_c INTERFACE
  "$<${gcc_like_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wno-system-headers;-Wno-deprecated;-Wwrite-strings;-Wunused;-Wunused-variable;-Wunused-parameter;-Wunused-function;-Wcast-align;-Wpedantic;-Werror;-ffunction-sections;-fdata-sections>>"

  # Устанавливаем дополнительные парметры сборки для типов сборки:
  #         Debug
  #         Release
  #         MinSizeRel
  #         RelWithDebInfo
  "$<$<CONFIG:Debug>:$<${gcc_like_c}:$<BUILD_INTERFACE:-O0;-DDEBUG;>>>"
  "$<$<CONFIG:Release>:$<${gcc_like_c}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
  "$<$<CONFIG:MinSizeRel>:$<${gcc_like_c}:$<BUILD_INTERFACE:-Os;-DNDEBUG;>>>"
  "$<$<CONFIG:RelWithDebInfo>:$<${gcc_like_c}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
  )


set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(${PROJECT_NAME}_compiler_flags_cxx INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wattributes;-Wshadow;-Wno-system-headers;-Wno-deprecated;-Woverloaded-virtual;-Wwrite-strings;-Wunused;-Wunused-variable;-Wunused-parameter;-Wunused-function;-Wcast-align;-Wold-style-cast;-Wpedantic;-Wuninitialized;-ffunction-sections;-fdata-sections;>>"

  # Set build parameters:
  #         Debug
  #         Release
  #         MinSizeRel
  #         RelWithDebInfo
  "$<$<CONFIG:Debug>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O0;-DDEBUG;>>>"
  "$<$<CONFIG:Release>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
  "$<$<CONFIG:MinSizeRel>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Os;-DNDEBUG;>>>"
  "$<$<CONFIG:RelWithDebInfo>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
)


add_library(openglplot  SHARED
    "${CMAKE_CURRENT_LIST_DIR}/source/openglplot.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/openglplot.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/oglcolor.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/oglcolor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/tickspos.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/tickspos.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/rendertext.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/rendertext.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/graph.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/graph.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/axis.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/axis.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/include/grid.h"
    "${CMAKE_CURRENT_LIST_DIR}/source/grid.cpp"

)


if(${PROJECT_NAME}_BUILD_SHARED_LIBS)
    set(DLL_${PROJECT_NAME}_EXPORT ON)
    set( BUILD_SHARED_LIBS ON )
    set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${OPENGL_INCLUDE_DIRS}
    "${CMAKE_CURRENT_LIST_DIR}/source/"
    "${CMAKE_CURRENT_LIST_DIR}/include/"
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    ${PROJECT_NAME}_compiler_flags
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    ${OPENGL_LIBRARIES}
)

#if(WIN32)
#    target_link_libraries(${PROJECT_NAME} PUBLIC
#        freetype)
#endif()

if(${QT_VERSION_MAJOR} MATCHES "6")
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        )
endif()

if (${PROJECT_NAME}_SANITIZE)
  include(Sanitize)
endif()
