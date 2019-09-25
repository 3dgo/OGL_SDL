# CMakeList.txt : CMake project for OGL_SDL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(OGL_SDL)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_CXX_STANDARD 17)
set(usd /home/lucien/GameDev/Library/vcpkg/packages/usd_x64-linux/share/usd)

find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-Image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add source to this project's executable.
add_executable(OGL_SDL src/main.cpp src/core/common_include.h src/core/singleton.h src/systems/system.h src/systems/log.h)
target_link_libraries(OGL_SDL PRIVATE ${OPENGL_LIBRARY})
target_link_libraries(OGL_SDL PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(OGL_SDL PRIVATE SDL2::SDL2_image)
target_link_libraries(OGL_SDL PRIVATE GLEW::GLEW)
target_link_libraries(OGL_SDL PRIVATE spdlog::spdlog)
target_link_libraries(OGL_SDL PRIVATE glm)
# TODO: Add tests and install targets if needed.