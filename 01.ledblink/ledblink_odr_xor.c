/*
 * File Name  : ledblink.c Ver 1.0
 *
 *
 * Description:
 *              LED Blink Program with software delay using NOP                
 *
 * Author: 
 *              ICEEL.NET (iceelinstitute@gmail.com)
 * 
 * Date  : 27 March 2020
 * 
 * License : GNU General Public License v3.0
 *
 * 
 * Hardware : 
 *      STM32F103C8T6 Blue Pill Board 
 
 *      Processor Detail    :
 *          Chip                :   STM32F103C8T6    
 *          Processor Type      :   ARM ® 32-bit Cortex ® -M3 CPU Core 
 *          Device Type         :   F1 Medium-density device
 *          Floating Point Unit :   Not Present
  *          Flash memory       :   64K Bytes (65536 Bytes)
 *                                  (st-info --flash : 0x10000) (pagesize: 1024)
 *                                  Location  :: 0x800 0000
 *          SRAM                :   20 Kbytes (65536 Bytes)
 *                                  (st-info --sram : 0x5000 ) 
 *                                  Location  :: 0x20000000   
 *          Chip ID             :   0x0410    
 *
 *      Ext. Clock Freq     :   8 MHz   
 *
 * Led Connection Details : PC13
 *
 * Compile          :
 * Link and Locate  :         
 * Flash Command    :
 *              
 */

/*********************************************************
* Type Defination for variables Type for easy readibility
*********************************************************/
#define uint32_t        unsigned int

/********************************
* Base addresses for peripherals
********************************/

#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define GPIOC_BASE      (PERIPH_BASE + 0x11000) // GPIOC base address is 0x40011000
#define RCC_BASE        (PERIPH_BASE + 0x21000) // RCC base address is 0x40021000

/**************************
* Stack Initiallization : 
* Two ways of doing stack intializtion
*   1)  Defining STACKINIT in C File (used in this example)
*   2)  Defining STACKINIT in linker Script File
***************************/

#define STACKINIT       (0x20002000)

/***********************
* Peripheral Structure 
************************/
typedef struct
{
	uint32_t CRL;      /* GPIO port configuration register low,      Address offset: 0x00 */
	uint32_t CRH;      /* GPIO port configuration register high,     Address offset: 0x04 */
	uint32_t IDR;      /* GPIO port input data register,             Address offset: 0x08 */
	uint32_t ODR;      /* GPIO port output data register,            Address offset: 0x0C */
	uint32_t BSRR;     /* GPIO port bit set/reset register,          Address offset: 0x10 */
	uint32_t BRR;      /* GPIO port bit reset register,              Address offset: 0x14 */
	uint32_t LCKR;     /* GPIO port configuration lock register,     Address offset: 0x18 */
} GPIO_type;

typedef struct
{
	uint32_t CR;       /* RCC clock control register,                Address offset: 0x00 */
	uint32_t CFGR;     /* RCC clock configuration register,          Address offset: 0x04 */
	uint32_t CIR;      /* RCC clock interrupt register,              Address offset: 0x08 */
	uint32_t APB2RSTR; /* RCC APB2 peripheral reset register,        Address offset: 0x0C */
	uint32_t APB1RSTR; /* RCC APB1 peripheral reset register,        Address offset: 0x10 */
	uint32_t AHBENR;   /* RCC AHB peripheral clock enable register,  Address offset: 0x14 */
	uint32_t APB2ENR;  /* RCC APB2 peripheral clock enable register, Address offset: 0x18 */
	uint32_t APB1ENR;  /* RCC APB1 peripheral clock enable register, Address offset: 0x1C */
	uint32_t BDCR;     /* RCC backup domain control register,        Address offset: 0x20 */
	uint32_t CSR;      /* RCC control/status register,               Address offset: 0x24 */
	uint32_t AHBRSTR;  /* RCC AHB peripheral clock reset register,   Address offset: 0x28 */
	uint32_t CFGR2;    /* RCC clock configuration register2,         Address offset: 0x2C */
} RCC_type;


/****************************************
* Peripheral Structure Definatio
*****************************************/
#define GPIOC   ((GPIO_type *)  GPIOC_BASE)
#define RCC     ((RCC_type *)     RCC_BASE)

int main(void);

/*************************************************
* ISR Vector Table
*************************************************/
uint32_t (* const vector_table[])
__attribute__ ((section(".isr_vectors"))) = {
	(uint32_t *) STACKINIT,         /* 0x00 Stack Pointer */
	(uint32_t *) main,              /* 0x04 Reset         */
};

int main(void)
{
    int i;
	// Enabling Clock
	RCC->APB2ENR |= (1 << 4); // Enable GPIOC
	
	// Make GPIOD Pin1 output
	GPIOC->CRH |= 0x00200000;

	//GPIOC->BRR = (1 << 13); // LED ON
	while(1) {
        GPIOC->ODR ^= (1 << 13);
    	for (i = 0; i < 100000; ++i) __asm__("nop");
	}

	return 0;
}

