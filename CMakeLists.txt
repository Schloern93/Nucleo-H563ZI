# Mindestanforderungen an CMake-Version
cmake_minimum_required(VERSION 3.15)

# Set CMake to create a static library instead of an executable during compiler tests
# This is necessary not fail if trying buil a test executable for the target platform
# Because the c compiler create  a file which runs on arm not on the host machine
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Toolchain ARM-GCC
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Projektname und Sprache
project(NucleoH5 LANGUAGES C CXX ASM)
#add_definitions(-DSTM32G491xx)

# STM32CubeG4 Library hinzufügen
# add_subdirectory(libs/CMSIS)
# add_subdirectory(libs/stm32g4xx-hal-driver)

# Set compiler to arm-none-eabi
set(CMAKE_C_COMPILER C:/ProgramData/chocolatey/bin/arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER C:/ProgramData/chocolatey/bin/arm-none-eabi-g++.exe)
set(CMAKE_ASM_COMPILER C:/ProgramData/chocolatey/bin/arm-none-eabi-gcc.exe)
#set(STM32_CHIP STM32G491RE)

# Compiler-Flags (angepasst für STM32)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -Wall -O0 -ffunction-sections -fdata-sections -g")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")


#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/linker/STM32G491RETX_FLASH.ld")
# Linker-Flags (angepasst für STM32)
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/Linker/STM32H563xx_FLASH.ld -lc -lnosys")


# Includepfade für HAL und CMSIS
include_directories(
    #${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/libs/STM32G4xx_HAL_Driver_mod/Inc
    ${CMAKE_SOURCE_DIR}/libs/CMSIS/Device/ST/STM32G4xx/Include
    ${CMAKE_SOURCE_DIR}/libs/CMSIS/Core/Include
)

# Hauptquelle hinzufügen
add_executable(${PROJECT_NAME} 
    Source/system_stm32h5xx.c
    Source/startup_stm32h563xx.s
    Source/main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")


# HAL-Bibliothek verlinken
#target_link_libraries(${PROJECT_NAME} STM32G4xx_HAL_Driver)

# Link Standard c runtime library and nosys
target_link_libraries(${PROJECT_NAME} c nosys)

target_link_options(${PROJECT_NAME} PRIVATE 
    -T${CMAKE_SOURCE_DIR}/Linker/STM32H563xx_FLASH.ld
    -Wl,--gc-sections
)
# Generate map file
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=output.map")

