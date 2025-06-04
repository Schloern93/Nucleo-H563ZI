# Cmsis
add_library(CmsisCore INTERFACE)
target_include_directories(CmsisCore INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
)

# cmsis-device-h5
add_library(CmsisDeviceH5 INTERFACE)
target_include_directories(CmsisDeviceH5 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-h5/Include
)

# Erstelle  stm32h5xx hal lib
add_library(Stm32H563Hal STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_adc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal_cortex.c
)
target_compile_definitions(Stm32H563Hal PUBLIC STM32H563xx)
target_include_directories(Stm32H563Hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Inc
)
target_link_libraries(Stm32H563Hal PUBLIC
    Stm32h563HalConfig
    CmsisDeviceH5
    CmsisCore
)

# Erstelle die FreeRTOS Library
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC Compiler")
    set(FREERTOS_PORT GCC_ARM_CM33_NTZ_NONSECURE CACHE STRING "FreeRTOS port for GCC")
elseif(CMAKE_C_COMPILER_ID STREQUAL "IAR")
    message(STATUS "Using IAR Compiler")
    set(FREERTOS_PORT IAR_ARM_CM33_NTZ_NONSECURE CACHE STRING "FreeRTOS port for IAR")
endif()

set(FREERTOS_HEAP 4)

add_library(freertos_config ALIAS FreeRtosConfig) # TODO: Check why is needet herre(Without hes not find the config)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel)

# add_library(FreeRtosKernel STATIC
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/tasks.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/queue.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/list.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/timers.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/event_groups.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure/port.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure/portasm.c
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/MemMang/heap_4.c # Falls du heap_4 nutzen willst
# )
# target_include_directories(FreeRtosKernel PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/include
# ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33_NTZ/non_secure/
# )
target_link_libraries(freertos_kernel PUBLIC
    FreeRtosConfig
)

add_library(FreeRTOS_Addons STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/ccondition_variable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cevent_groups.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cmem_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cmutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cqueue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cread_write_lock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/csemaphore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/ctasklet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/ctickhook.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/ctimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/cworkqueue.cpp
)

target_include_directories(FreeRTOS_Addons PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/include
)

target_link_libraries(FreeRTOS_Addons PUBLIC
    freertos_kernel
)
