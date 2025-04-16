# Erstelle interface Libs für die Config Files

# FreeRtos
add_library(FreeRtosConfig INTERFACE)
target_include_directories(FreeRtosConfig INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/FreeRtosConfig
)

# Hal Config stm32h563
add_library(Stm32h563HalConfig INTERFACE)
target_include_directories(Stm32h563HalConfig INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32h563halConfig
)
