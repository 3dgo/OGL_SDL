# CMakeList.txt : CMake project for OGL_SDL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(OGL_SDL)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Add source to this project's executable.
add_executable(OGL_SDL src/OGL_SDL.cpp)
target_include_directories(OGL_SDL PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(OGL_SDL PRIVATE ${GLEW_INCLUDE_DIR})
target_link_libraries(OGL_SDL PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(OGL_SDL PRIVATE GLEW::GLEW)
# TODO: Add tests and install targets if needed.