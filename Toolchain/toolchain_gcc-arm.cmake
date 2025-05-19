
# Set CMake to create a static library instead of an executable during compiler tests
# This is necessary not fail if trying buil a test executable for the target platform
# Because the c compiler create  a file which runs on arm not on the host machine
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Toolchain ARM-GCC
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER_ID "GNU" FORCE)

# Set Compiler
set(CMAKE_C_COMPILER "C:/ProgramData/chocolatey/bin/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "C:/ProgramData/chocolatey/bin/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "C:/ProgramData/chocolatey/bin/arm-none-eabi-gcc.exe")

# Set Compiler Options
set(CMAKE_C_FLAGS "-mcpu=cortex-m33 -mthumb -Wall -O0 -ffunction-sections -fdata-sections -g" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" FORCE)

# Linker
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/Linker/STM32H563xx_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS
    "-T${LINKER_SCRIPT} -Wl,--gc-sections -specs=nosys.specs -Wl,-Map=output.map"
    CACHE STRING "" FORCE
)