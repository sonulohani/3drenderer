cmake_minimum_required(VERSION 3.10)

# Set the project name
project(3drenderer)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add an executable
add_executable(3drenderer src/main.c src/display.c src/vector.c)
target_link_libraries(3drenderer ${SDL2_LIBRARIES} m)
