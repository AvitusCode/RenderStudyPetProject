cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME jdRenderEngine)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenGl REQUIRED)

option(ROOT_INFO "set root dir" ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(jdEngineCore)

add_executable(${PROJECT_NAME} main.cpp Application.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PROJECT_NAME} PRIVATE jdEngineCore)