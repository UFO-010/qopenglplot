
#SET(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")

#target_compile_options(${PROJECT_NAME}_compiler_flags_c INTERFACE
#  "$<${gcc_like_c}:$<BUILD_INTERFACE:-fsanitize=address>>"
#)

#target_compile_options(${PROJECT_NAME}_compiler_flags_cxx INTERFACE
#  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fsanitize=address>>"
#)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

target_compile_options(${PROJECT_NAME}_compiler_flags_c INTERFACE
  "$<${gcc_like_c}:$<BUILD_INTERFACE:-fsanitize=address>>"
)

target_compile_options(${PROJECT_NAME}_compiler_flags_cxx INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fsanitize=address>>"
)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -fno-sanitize-recover=all
    -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(isMultiConfig)
    if(NOT "Asan" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Asan)
    endif()
else()
    set(allowedBuildTypes Asan Debug Release RelWithDebInfo MinSizeRel)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowedBuildTypes}")

    if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
        message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
    endif()
endif()

set(CMAKE_C_FLAGS_ASAN
    "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer" CACHE STRING
    "Flags used by the C compiler for Asan build type or configuration." FORCE)

set(CMAKE_CXX_FLAGS_ASAN
    "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer" CACHE STRING
    "Flags used by the C++ compiler for Asan build type or configuration." FORCE)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address" CACHE STRING
    "Linker flags to be used to create executables for Asan build type." FORCE)

set(CMAKE_SHARED_LINKER_FLAGS_ASAN
    "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -fsanitize=address" CACHE STRING
    "Linker lags to be used to create shared libraries for Asan build type." FORCE)

set(SYMBOLIZER_PATH "/usr/bin/llvm-symbolizer")

add_compile_definitions("symbolize=1:detect_stack_use_after_return=1:external_symbolizer_path=${SYMBOLIZER_PATH}:verbosity=1:log_threads=1:fast_unwind_on_malloc=0;")
#add_compile_definitions("verbosity=1:log_threads=1:fast_unwind_on_malloc=0;")

#SET(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
