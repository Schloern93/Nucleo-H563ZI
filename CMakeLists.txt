# Mindestanforderungen an CMake-Version
cmake_minimum_required(VERSION 3.15)

# # Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Project
project(NucleoH5 LANGUAGES C CXX ASM)

# Add Subdirectory for Libs
add_subdirectory(Config)
add_subdirectory(Libraries)

# TODO: Check this why this is needet and waht it does
add_definitions(-DSTM32H563xx)

# TODO: Move to Toolchain files
# Add Compiler Specific System FIles
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC-specific startup file")
    set(STARTUP_FILE Source/System/startup_stm32h563xx_gcc.s)
elseif(CMAKE_C_COMPILER_ID STREQUAL "IAR")
    message(STATUS "Using IAR-specific startup file")
    set(STARTUP_FILE Source/System/startup_stm32h563xx_iar.s)
endif()

# Add source files
add_executable(${PROJECT_NAME}
    Source/System/system_stm32h5xx.c
    ${STARTUP_FILE}
    Source/main.cpp
)

#Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Util
)

# Create .elf file
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")

# TODO: Move to toolchain files
# Link Compiler Specific Libs
if(CMAKE_C_COMPILER_ID STREQUAL "GCC")
    target_link_libraries(${PROJECT_NAME}
        c
        nosys
    )
endif()

# Link Standard c runtime library and nosys
target_link_libraries(${PROJECT_NAME}
    FreeRTOS_Addons
    Stm32H563Hal
)
