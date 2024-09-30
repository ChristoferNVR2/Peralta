cmake_minimum_required(VERSION 3.10)
project(Peralta)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLUT
find_package(GLUT REQUIRED)

# Add include directory for stb_image.h (assuming it's in the 'include' folder)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable
add_executable(Peralta main.cpp)

# Link libraries
target_link_libraries(Peralta PRIVATE OpenGL::GL GLUT::GLUT)
