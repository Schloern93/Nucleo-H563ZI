# Cmsis
add_library(Cmsis INTERFACE)
target_include_directories(Cmsis INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_5/CMSIS/Core/Include
)

# cmsis-device-h5
add_library(CmsisDeviceH5 INTERFACE)
target_include_directories(CmsisDeviceH5 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis-device-h5/Include
)

# Erstelle  stm32h5xx hal lib
add_library(stm32h5xx_hal STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Src/stm32h5xx_hal.c
)
target_compile_definitions(stm32h5xx_hal PUBLIC STM32H563xx)
target_include_directories(stm32h5xx_hal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h5xx-hal-driver/Inc
)
target_link_libraries(stm32h5xx_hal PUBLIC
    Stm32h563HalConfig
    CmsisDeviceH5
    Cmsis
)

# Erstelle die FreeRTOS Library
add_library(FreeRTOS STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33/non_secure/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/MemMang/heap_4.c # Falls du heap_4 nutzen willst
)
target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33/non_secure/
)
target_link_libraries(FreeRTOS PUBLIC
    FreeRtosConfig
)




