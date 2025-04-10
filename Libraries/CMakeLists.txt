

include_directories(
    #${CMAKE_SOURCE_DIR}/Config
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

# Include-Pfade für FreeRTOS
target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_SOURCE_DIR}/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM33/non_secure/
)



