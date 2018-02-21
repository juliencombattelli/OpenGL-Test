cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules")

find_package(OpenGL MODULE REQUIRED)
find_package(GLEW MODULE REQUIRED)
find_package(SFML REQUIRED MODULE COMPONENTS system window graphics)

add_executable(hello src/gl.cpp)

target_include_directories(hello PRIVATE OpenGL::GL GLEW::GLEW ${SFML_INCLUDE_DIR})

target_link_libraries(hello LINK_PRIVATE OpenGL::GL GLEW::GLEW ${SFML_LIBRARIES})
