/* 
	Linker Script for STM32F103C8 with 64K Flash and 20K SRAM 
*/

MEMORY {
    rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

EXTERN(isr_vectors);
ENTRY(main);

SECTIONS {
	.text :
	{
		KEEP(*(.isr_vectors))  /* ISR Vector table */
		*(.text*)          /* Program code */
		*(.rodata*)        /* Read only data */
		. = ALIGN(4);
	} >rom

	.data :
	{
		*(.data*)      /* Read-write initialized data */
		. = ALIGN(4);
	} >ram AT >rom

	.bss :
	{
		. = ALIGN(4);
		*(.bss*)       /* Read-write zero initialized data */
		*(COMMON)
		. = ALIGN(4);
	} >ram
}
