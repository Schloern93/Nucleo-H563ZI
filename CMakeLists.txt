# Mindestanforderungen an CMake-Version
cmake_minimum_required(VERSION 3.15)

## Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Project
project(NucleoH5 LANGUAGES C CXX ASM)

#Add Subdirectory for Libs
add_subdirectory(Config)
add_subdirectory(Libraries)

# TODO: Check this why this is needet and waht it does
add_definitions(-DSTM32H563xx)

# Add source files
add_executable(${PROJECT_NAME}
    Source/system_stm32h5xx.c
    Source/startup_stm32h563xx.s
    Source/main.cpp
)

# Create .elf file
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

# Link Standard c runtime library and nosys
target_link_libraries(${PROJECT_NAME}
    c
    nosys
    FreeRTOS_Addons
    Stm32H563Hal
)






