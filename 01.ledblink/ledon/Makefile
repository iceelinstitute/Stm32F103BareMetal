# Steps for creating bin
# make o
# make elf
# make hex
# make bin
# For writing into flash
# make flash
# For removing all obejct files
# make clean

o : ledon.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -Wall -c ledon.c

elf : ledon.o
	arm-none-eabi-gcc  -march=armv7-m -nostartfiles --specs=nosys.specs -T stm32f103.ld ledon.o -o ledon.elf 

hex : ledon.elf
	arm-none-eabi-objcopy -O ihex ledon.elf ledon.hex

bin : ledon.elf
	arm-none-eabi-objcopy -O binary ledon.elf ledon.bin

clean : 
	rm ledon.elf ledon.hex ledon.bin ledon.o

flash : ledon.bin
	st-flash write ledon.bin 0x8000000
