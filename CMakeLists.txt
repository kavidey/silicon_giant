cmake_minimum_required(VERSION 3.24)
project(silicon_giant)

set(CMAKE_CXX_STANDARD 17)

add_executable(silicon_giant main.cpp Neuron.cpp Neuron.h util.cpp util.h Synapse.cpp Synapse.h)
