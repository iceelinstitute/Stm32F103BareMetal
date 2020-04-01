/*
 * File Name  : ledblink.c Ver 1.0
 *
 * Description:
 *              LED ON Program 
 *
 * Author: 
 *              ICEEL.NET (iceelinstitute@gmail.com)
 * 
 * Date  : 27 March 2020
 * 
 * License : GNU General Public License v3.0
 *
 * Hardware : 
 *      STM32F103C8T6 Blue Pill Board 
 *
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
 * Led Connection Details : PA5 [ Connect External LED - 3.3V (positive) and PA5 (negative) ]
 *
 * Compile          : arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -Wall -c ledblink.c
 * Link and Locate  : arm-none-eabi-gcc  -march=armv7-m -nostartfiles --specs=nosys.specs -T stm32f103.ld ledblink.o -o ledblink.elf 
 * ELF to Hex       : arm-none-eabi-objcopy -O ihex ledblink.elf 
 * ELF to Bin       : arm-none-eabi-objcopy -O binary ledblink.elf
 * Clean            : rm ledblink.elf ledblink.hex ledblink.bin
 * Flash Command    : st-flash write ledblink.bin
 */

/*********************************************************
* Type Defination for variables Type for easy readibility
*********************************************************/
#define uint32_t        unsigned int

/********************************
* Base addresses for peripherals
********************************/

#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define GPIOA_BASE      (PERIPH_BASE + 0x10800) // GPIOA 
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
#define GPIOA   ((GPIO_type *)  GPIOA_BASE)
#define RCC     ((RCC_type *)     RCC_BASE)

#define GPIO_PIN   (5)   //PA5

int main(void);

/*************************************************
* ISR Vector Table
*************************************************/
uint32_t (* const vector_table[])
__attribute__ ((section(".isr_vectors"))) = {
	(uint32_t *) STACKINIT,         /* 0x00 Stack Pointer */
	(uint32_t *) main,              /* 0x04 Reset         */
};


/************************************************************
 *  Function Description : ledblink_bsrr
 *      Set/Resets GPIO  on/off using BSRR/BRR 
 ************************************************************/
void ledblink_bsrr()
{
    int i;
    GPIOA->BSRR = (1 << GPIO_PIN); // LED OFF (3.3V will switch OFF LED)
	for (i = 0; i < 800000; ++i) __asm__("nop");

    GPIOA->BRR = (1 << GPIO_PIN); // LED ON (OV will switch on LED)
    for (i = 0; i < 200000; ++i) __asm__("nop");
}


/***************************************************************
 *  Function Description : ledblink_odr
 *      Set/Resets GPIO on/off using AND/OR 
 **************************************************************/

void ledblink_odr()
{
    int i;
    
    GPIOA->ODR |= (1 << GPIO_PIN);
	for (i = 0; i < 100000; ++i) __asm__("nop");
		
    GPIOA->ODR &= (0 << GPIO_PIN);
	for (i = 0; i < 500000; ++i) __asm__("nop");
}

/****************************************************************
 *  Function Description : 
 *      Set/Resets GPIO on/off using exclution OR 
 *****************************************************************/
void ledblink_odr_xor()
{
    int i;
    GPIOA->ODR ^= (1 << GPIO_PIN);
	for (i = 0; i < 100000; ++i) __asm__("nop");
}

int main(void)
{
    //  Register RCC->APB2ENR
	// Enabling Clock for GPIOA RCC Register
    //  15     14     13   12   11    10    09    08   07   06    05    04    03   02    01   00
    //  Res    USERT  Res  SP1  TIM1  ADC2  ADC1  Res  Res  IOPE  IOPD  IOPC IOPB  IOPA  Res  AFIO
    //         1EN    -    EN   EN    EN    EN              EN    EN    EN   EN    EN         EN

	RCC->APB2ENR |= (1 << 2); // Enabling Clock for GPIOA RCC Register
    	
	// Make GPIOA output (PA1)  General Purpose Push Pull Output 2 Mhz
    // Conf : 0x0 Mode : 0x2 ------> 4 Bit 0x2 General Purpose Push Pull Output 2 Mhz
    
    // GPIOA->CRL |= 0x00000002; PIN_0          GPIOA->CRL |= 0x00000020; PIN_1
    // GPIOA->CRL |= 0x00000200; PIN_2          GPIOA->CRL |= 0x00002000; PIN_3
    // GPIOA->CRL |= 0x00020000; PIN_4          GPIOA->CRL |= 0x00200000; PIN_5
    // GPIOA->CRL |= 0x02000000; PIN_6          GPIOA->CRL |= 0x20000000; PIN_7

    // GPIOA->CRH |= 0x00000002; PIN_8          GPIOA->CRH |= 0x00000020; PIN_9
    // GPIOA->CRH |= 0x00000200; PIN_10         GPIOA->CRH |= 0x00002000; PIN_11
    // GPIOA->CRH |= 0x00020000; PIN_12         GPIOA->CRH |= 0x00200000; PIN_13
    // GPIOA->CRH |= 0x02000000; PIN_14         GPIOA->CRH |= 0x20000000; PIN_15


//       GPIOA->CRL |= 0x00200000; // PA5
	          GPIOA->CRL |= 0x22222222; // PA5	

//     OR
//     GPIOA->CRL |= (0x2 << (4*(GPIO_PIN))) ; // 

	//GPIOA->BRR = (1 << 13); // LED ON
	while(1) {
            ledblink_bsrr();
    //    ledblink_odr_xor();
    //    ledblink_odr_xor();
	}

	return 0;
}

