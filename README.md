# Nucleo-H5663ZI

# Debugging:
Starten des Debuggens von der Commandozeile in dem mingw fenster!!!
- openocd -f Debug/stlink.cfg -f Debug/st_nucleo_h5.cfg

Neues Terminal GDB starten
- arm-none-eabi-gdb CmakeBuild/NucleoH5.elf
- target remote localhost:3333

Debug befehle
- monitor halt
- monitor reset
-  info register


disassemble /m Reset_Handler
Lader der Linker scipt variablen bzw anzeigen
 p &_sidata

 c -_> Conntinue
 n --> next springt über funktion
 s --> Spring in funktion
 bt -> backtrace :: Zeigt die Stackcall an
