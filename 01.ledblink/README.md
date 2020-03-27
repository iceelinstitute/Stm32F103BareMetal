This Directory contains the tutorial about the baremetal programming of STM32F103C6T8 based Blue Pill Board.

It contains following programs

	1.	ledon Directory
		First go to this directory and see first bare metal program

	2.	Other Files for blinking LED

		C Files :
			ledblink.c (Copy of ledblink_min.c)	
			ledblink_bssr.c 	
			ledblink_org.c 	
			ledblink_odr.c 	
			ledblink_odr_xor.c 	

		Makefile
			makefile

		Linker Script File
			stm32f103.ld

		Binary File
		 	ledblink.bin

	First explore ledon directory for switching ON LED on blue pill board

	These directory contains four other 'C' files for blinking LED
	
Ledon directory contains one C file along with its Makefile and linker script. ledon program will switch on the LED on blue pill

This directory contains four programs for blinking LED. Programs use different ways to blink LED

1. ledblink_bssr.c    :	uses BSSR/BSR for blinking
2. ledblink_odr.c     :	uses ODR 
3. ledblink_odr_xor.c : uses ODR with XOR functionality

4. ledblink_org.c     : contains code for blinking using all three methods. 

ledblink.c is used in Makefile for generating ledblink.elf ledblink.hex and ledblink.bin
So copy any one of above four 'C' file as ledblink.c and create blink program.

Issue make command at command terminal.




	




