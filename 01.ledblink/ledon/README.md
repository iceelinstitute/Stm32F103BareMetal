This folder contains following files

Source Files :
  1) ledon.c       (C Source File)
  2) stm32f103.ld  (Linker Script File)
  3) Makefile      (Make File)
 
Output Precompiled File :
  1)  ledon.elf
  2)  ledon.hex
  
Steps for creating LEDON.BIN File
# make o
# make elf
# make hex
# make bin


For writing into flash
# make flash

For removing all obejct files
# make clean

