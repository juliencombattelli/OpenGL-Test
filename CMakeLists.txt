cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(OpenGL MODULE REQUIRED)
find_package(GLEW MODULE REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(SFML MODULE REQUIRED COMPONENTS system window)

add_executable(hello src/gl.cpp)
set_property(TARGET hello PROPERTY CXX_STANDARD 17)

target_include_directories(hello PRIVATE OpenGL::GL GLEW::GLEW ${GLM_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})

target_link_libraries(hello LINK_PRIVATE OpenGL::GL GLEW::GLEW ${ASSIMP_LIBRARIES} ${SFML_LIBRARIES})
