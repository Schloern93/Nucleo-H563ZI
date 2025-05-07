# Nucleo-H5663ZI

# How to Start Debugging:
1. Start OpenOcd in a mingw64 terminal. TODO: This is needet until the .cfg fiels are not in the offical openOcd with choco
- openocd -f Debug/stlink.cfg -f Debug/st_nucleo_h5.cfg
- openocd -f Debug/stlink-dap.cfg -f Debug/stm32h5x.cfg

2. Start Debug Seasison by pressing Run and DEbug
- This will first flash the device
- After taht u have to connect the openOcd manually in the ming64 terminal
- Should stop while enterring the main function
