
# Set CMake to create a static library instead of an executable during compiler tests
# This is necessary not fail if trying buil a test executable for the target platform
# Because the c compiler create  a file which runs on arm not on the host machine
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Toolchain ARM-GCC
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER_ID "IAR" FORCE)

# Set Compiler
set(CMAKE_C_COMPILER "C:/iar/ewarm-9.60.3/arm/bin/iccarm.exe")
set(CMAKE_CXX_COMPILER "C:/iar/ewarm-9.60.3/arm/bin/iccarm.exe")
set(CMAKE_ASM_COMPILER "C:/iar/ewarm-9.60.3/arm/bin/iasmarm.exe")

# Set Compiler Options
set(CMAKE_C_FLAGS "--cpu=Cortex-M33 --thumb --debug -e" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "--cpu Cortex-M33 --thumb" CACHE STRING "" FORCE)

# Linker
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/Linker/stm32h563xx_flash.icf")
set(CMAKE_EXE_LINKER_FLAGS
    "--config ${LINKER_SCRIPT}"
    CACHE STRING "" FORCE
)