CMAKE_MINIMUM_REQUIRED(VERSION 3.18.0)

project(openglplot_example LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


# specify the C++ standard
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_c INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_cxx INTERFACE)

target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_c)
target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_cxx)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_11)


add_subdirectory(../ openglplot)

#insert your files and directories names here
set(SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

set(HEADER
    "${CMAKE_CURRENT_LIST_DIR}/mainwindow.h"
) 


set(EXTRA_APP_PARAM)
if(WIN32)
  set(EXTRA_APP_PARAM WIN32)
endif()


add_executable(${PROJECT_NAME} ${EXTRA_APP_PARAM} ${SOURCES} ${HEADER})


find_library(OPENGLPLOT openglplot PATH_SUFFIXES openglplot)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_compiler_flags
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    openglplot
)

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

