/*
 * File Name  : systick.c Ver 1.0 
 *
 * Description:
 *   Systick example  : Blink LEDs at 1 second intervals (PC13)
 *   		Four Mode : 1. APH Clock Interrupt Disable
 *					    2. APH Clock Interrupt Enable 
 *   		            3. Processor Clock Interrupt Disable
 *					    4. Processor Interrupt Enable 
 *    
 * Author: 
 *       ICEEL.NET (iceelinstitute@gmail.com)
 * 
 * Date  : 27 March 2020
 * 
 * License : GNU General Public License v3.0
 *
 * Hardware : 
 *      STM32F103C8T6 Blue Pill Board 
 * 		    Processor Detail    :
 *
 *      Ext. Clock Freq     :   8 MHz   
 *
 * Led Connection Details : PC13 
 *
 * Compile          : arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -Wall -c systick.c
 * Link and Locate  : arm-none-eabi-gcc  -march=armv7-m -nostartfiles --specs=nosys.specs -T stm32f103.ld systick.o -o systick.elf 
 * ELF to Hex       : arm-none-eabi-objcopy -O ihex systick.elf 
 * ELF to Bin       : arm-none-eabi-objcopy -O binary systick.elf
 * Clean            : rm systick.elf systick.hex systick.bin
 * Flash Command    : st-flash write systick.bin
 *
 * Issue make command for building this applicaton
 */


/*************STEPS for SYSTICK **********************

1.	Enable Clock for GPIOC Port
2.	Configure PC13 pin as Output Push Pull

3.	Select Clock Source for Systick
4.	Configure Interrupt Enable or Disable
5.  Set Systick Reload Value
5.  Set Systick Current Value
6   Enable Systick

7.  Set systick handler in IVT if interrupt is enabled.
		
*****************************************************/

/********** stm32f1 Register Address and Structures  ***************/
#include "stm32f1reg.h"

#define GPIO_PIN					(13)  // LED connected on PC13


/*********** Function declarations ****************/
void systickHandler(void);
void delayMilliSec(volatile uint32_t s);

int32_t main(void);

/********** Interrupt Vector Table ***************/

uint32_t (* const vector_table[])
__attribute__ ((section(".isr_vector"))) = {
	(uint32_t *) STACKINIT,         /* 0x00 Stack Pointer */
	(uint32_t *) main,              /* 0x04 Reset         */
	0,                              /* 0x08 NMI           */
	0,                              /* 0x0C HardFaullt    */
	0,                              /* 0x10 MemManage     */
	0,                              /* 0x14 BusFault      */
	0,                              /* 0x18 UsageFault    */
	0,                              /* 0x1C Reserved      */
	0,                              /* 0x20 Reserved      */
	0,                              /* 0x24 Reserved      */
	0,                              /* 0x28 Reserved      */
	0,                              /* 0x2C SVCall        */
	0,                              /* 0x30 Debug Monitor */
	0,                              /* 0x34 Reserved      */
	0,                              /* 0x38 PendSV        */
	(uint32_t *) systickHandler,   /* 0x3C SysTick       */
};


/********** Function Defintion ******************/

/*
 * Funtion Name		: systickHandler 
 * Description 		: Toggles output on  PC13
 *					  This is interrupt handler for systick.
 *					  Initialized in Interrupt Vector Table
 * Input			: None
 * Return Value		: None
*/

void systickHandler(void)
{
		GPIOC->ODR ^= (1 << GPIO_PIN);
}

/*
 * Funtion Name		: delayMilliSec
 * Description 		: millisecond delay using polling 
 *						without interrupt
 * Input			: mSec
 * Return Value		: None
*/

void delayMilliSec(volatile uint32_t mSec)
{
	for(; mSec>0; mSec--){
		while(!(SYSTICK->CSR & (1 << 16))); // Wait until COUNTFLAG is 1
	}
}

/*  

Function : Main

Here Four Option
APH Clock APH/8 = 1Mz
Processor Clock = 8Mz

1. 	APH CLOCK with Interrupt Disable (Polling Delay) : 2 Sec
				SYSTICK->CSR = (0 << 2);// 0x00004; // AHB/8 1 Mhz
				SYSTICK->CSR &= 0xFFFFFFFD; //~(1 << 1); // Disable Interrupt
				SYSTICK->RVR = 1000; 		// 1 ms for without interrupt AHB
				while(1){
					delayMilliSec(2000);  // 2 second
					GPIOC->ODR ^= (1 << GPIO_PIN);

				}


2. 	APH CLOCK with Interrupt Enable  : 1 Sec
				SYSTICK->CSR = (0 << 2);// 0x00004; // AHB/8 1 Mhz
				SYSTICK->CSR |= (1 << 1);	// Enable Interrupt
				SYSTICK->RVR = 1000000; 		// 1 sec for without interrupt AHB
				while(1);				


3. 	Processor CLOCK with Interrupt Disable (Polling Delay) : 500ms
				SYSTICK->CSR = (1 << 2);// 0x00004; // Processsor 8  Mhz
				SYSTICK->CSR &= 0xFFFFFFFD; //~(1 << 1); // Disable Interrupt
				SYSTICK->RVR = 8*1000; 		// 1 ms for without interrupt AHB
				while(1){
					delayMilliSec(500);  // 500 mSecond
					GPIOC->ODR ^= (1 << GPIO_PIN);

				}


4. 	Processor CLOCK with Interrupt Enable  : 
				SYSTICK->CSR = (1 << 2);// 0x00004; // Processsor 1 Mhz
				SYSTICK->CSR |= (1 << 1);	// Enable Interrupt
				SYSTICK->RVR = 8*100000; 		// 100 ms for without interrupt AHB
				while(1);				

*/

int32_t main(void)
{
	 //  Register RCC->APB2ENR
	// Enabling Clock for GPIOC RCC Register
    //  15     14     13   12   11    10    09    08   07   06    05    04    03   02    01   00
    //  Res    USERT  Res  SP1  TIM1  ADC2  ADC1  Res  Res  IOPE  IOPD  IOPC IOPB  IOPA  Res  AFIO
    //         1EN    -    EN   EN    EN    EN              EN    EN    EN   EN    EN         EN

	RCC->APB2ENR |= (1 << 4); // Enable GPIOC


	// Make GPIOC output (PA1)  General Purpose Push Pull Output 2 Mhz
    // Conf : 0x0 Mode : 0x2 ------> 4 Bit 0x2 General Purpose Push Pull Output 2 Mhz
    
    // GPIOC->CRL |= 0x00000002; PIN_0          GPIOC->CRL |= 0x00000020; PIN_1
    // GPIOC->CRL |= 0x00000200; PIN_2          GPIOC->CRL |= 0x00002000; PIN_3
    // GPIOC->CRL |= 0x00020000; PIN_4          GPIOC->CRL |= 0x00200000; PIN_5
    // GPIOC->CRL |= 0x02000000; PIN_6          GPIOC->CRL |= 0x20000000; PIN_7
    // GPIOC->CRH |= 0x00000002; PIN_8          GPIOC->CRH |= 0x00000020; PIN_9
    // GPIOC->CRH |= 0x00000200; PIN_10         GPIOC->CRH |= 0x00002000; PIN_11
    // GPIOC->CRH |= 0x00020000; PIN_12         GPIOC->CRH |= 0x00200000; PIN_13
    // GPIOC->CRH |= 0x02000000; PIN_14         GPIOC->CRH |= 0x20000000; PIN_15


	// Configure GPIOC PC13 
	GPIOC->CRH |= 0x00200000; //PC13

		
	// SYSTICK Control and Status Register
	//  31-17      16       15-3      2        1       0
    //  RES      CNTFLAG    RES     CLKSRC   TICKINT  ENABLE

	//  CNTFLAG   1 -> Counter counted to 0 (Status)	
	//  CLKSRC    1 -> Processor Clock     0 -> Ext Clock
	//	TICKINT   1 -> Systick Exception ENable 
	//  ENABLE    1 -> Counter Enabled  0 -> Disabled

	// Setting Clock Source  
    SYSTICK->CSR = (1 << 2);// 0x00004; // Processor  8 Mhz
    // Configure Interrupt  Interrupt
	SYSTICK->CSR &= 0xFFFFFFFD; //~(1 << 1); // Disable Interrupt
    // Load the reload value (4 Values for 4 Modes to get differnt timing)
	SYSTICK->RVR = 8*1000; 		// 1 ms for without interrupt AHB

	SYSTICK->CVR = 0;           // Set the current value to 0

	SYSTICK->CSR |= (1 << 0);   // Enable SysTick

    
    while(1){
			delayMilliSec(500);  // 500 mSecond
			GPIOC->ODR ^= (1 << GPIO_PIN);
	}
	
}

