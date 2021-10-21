cmake_minimum_required(VERSION 3.16)
project(graphics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
find_package(ASSIMP REQUIRED)

file(GLOB_RECURSE CPPSOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE HSOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")


add_executable(out ${CPPSOURCES} ${HSOURCES})
target_link_libraries(out GLEW::GLEW OpenGL::GL GLUT::GLUT glm::glm ${ASSIMP_LIBRARIES})