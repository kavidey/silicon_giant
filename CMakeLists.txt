cmake_minimum_required(VERSION 3.24)
project(silicon_giant)

set(CMAKE_CXX_STANDARD 20)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

## Find dependencies
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)
find_package(imgui REQUIRED)

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif()

add_executable(silicon_giant src/main.cpp src/Neuron.cpp src/Neuron.h lib/silicon_giant/util.cpp lib/silicon_giant/util.h src/Synapse.cpp src/Synapse.h src/Network.cpp src/Network.h)

target_link_libraries(
        silicon_giant
        PRIVATE
        Freetype::Freetype
        glfw
        glad::glad
        glm::glm
        imgui::imgui
        "-framework OpenGL"
)