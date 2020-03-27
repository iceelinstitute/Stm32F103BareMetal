This tutorial will switch on the LED on STM32F103F6T8 Blue pill board connected on PC13 pin in sink Mode


Steps in  C Code :
1) Enable RCC Clock for GPIOC
2) Configure PC13 (GPIOC) in output mode with push-pull configuration
3) Switch on LED by writing 0 to pin PC13

This is Baremetal Programming.

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

Above steps can be executed on command line also

 * Compile          : arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -Wall -c ledon.c
 * Link and Locate  : arm-none-eabi-gcc  -march=armv7-m -nostartfiles --specs=nosys.specs -T stm32f103.ld ledon.o -o ledon.elf 
 * ELF to Hex       : arm-none-eabi-objcopy -O ihex ledon.elf 
 * ELF to Bin       : arm-none-eabi-objcopy -O binary ledon.elf
 * Clean            : rm ledon.elf ledon.hex ledon.bin


For writing into flash
# make flash

For removing all obejct files
# make clean

