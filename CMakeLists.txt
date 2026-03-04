cmake_minimum_required(VERSION 3.19)
project(CPU_SIMULATOR)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPU_SIMULATOR
    src/cpu/ALU.cpp
    src/cpu/ControlUnit.cpp
    src/cpu/CPU.cpp
    src/memory/Memory.cpp
    src/utils/Constants.cpp
    src/Simulator.cpp
    src/main.cpp
)

target_include_directories(CPU_SIMULATOR PRIVATE ${CMAKE_SOURCE_DIR}/include)