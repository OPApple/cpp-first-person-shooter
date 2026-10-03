#!!!! Part of this cmake file has been AI generated based on makefile

# slop begins
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.14)
project(first_person_shooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enforce a preference for static linking throughout the project
set(CMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX .a)

# GLFW must be built as a static library.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# FREETYPE must be built as a static library.
# We explicitly disable the Caching functionality, as it relies on system installs
set(FT_WITH_ZLIB OFF CACHE BOOL "Disable ZLib dependency" FORCE)
set(FT_WITH_BZIP2 OFF CACHE BOOL "Disable BZip2 dependency" FORCE)

add_subdirectory(libs/glfw)

add_subdirectory(libs/freetype)

include_directories(
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/glad
    ${CMAKE_SOURCE_DIR}/libs/freetype
    ${CMAKE_SOURCE_DIR}/libs/glm
    ${CMAKE_SOURCE_DIR}/libs/stb

)

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/Data/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/GUI/*.cpp"
    "${CMAKE_SOURCE_DIR}/libs/glad.c"
)


add_executable(main 
    ${SOURCES}
    src/main.cpp
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:main>/resources
)


target_compile_options(main PRIVATE -Wall -Wextra -Wno-narrowing)


target_link_libraries(main
    glfw
    dl
    GL
    pthread
    m
    freetype
    sfml-audio
)

file(GLOB DATA_SOURCES
    "${CMAKE_SOURCE_DIR}/src/Data/*.cpp"
)


add_library(test_lib
    ${DATA_SOURCES}
)

file(GLOB DATA_SOURCES
    "${CMAKE_SOURCE_DIR}/src/Data/*.cpp"
)


include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)
