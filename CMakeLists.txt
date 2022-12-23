cmake_minimum_required(VERSION 3.24)
project(silicon_giant)

set(CMAKE_CXX_STANDARD 20)

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif()

add_executable(silicon_giant src/main.cpp src/Neuron.cpp src/Neuron.h lib/silicon_giant/util.cpp lib/silicon_giant/util.h src/Synapse.cpp src/Synapse.h src/Network.cpp src/Network.h)