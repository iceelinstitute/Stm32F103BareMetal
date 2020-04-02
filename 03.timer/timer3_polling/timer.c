/*
 * File Name  : timer.c Ver 1.0  with Polling
 *
 * Description:
 *   timer3 example  : Blink LEDs at 1 second intervals (PC13) using timer 3
 *   		
 *                      Two Examples
 *                      1. Interrupt Disable (Delay in Polling)
 *					    2. Interrupt Enable 
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
 *      Int. Default Freq   :   8 MHz
 *
 * Led Connection Details : PC13 
 *
 * Step for generating bin : make
 *  
 * Compile          : arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -Wall -c timer.c
 * Link and Locate  : arm-none-eabi-gcc  -march=armv7-m -nostartfiles --specs=nosys.specs -T stm32f103.ld timer.o -o timer.elf 
 * ELF to Hex       : arm-none-eabi-objcopy -O ihex timer.elf 
 * ELF to Bin       : arm-none-eabi-objcopy -O binary timer.elf
 * Clean            : rm timer.elf timer.hex timer.bin
 * Flash Command    : st-flash write timer.bin
 *
 * Issue make command for building this applicaton
 */


/*************STEPS for Timer 3 **********************

1.	Enable Clock for GPIOC Port
2.	Configure PC13 pin as Output Push Pull

3.  Reset Control Register 1
4.  Setting PSC to 7999 (dividing 8000 to generate 1Mhz Clock)
5.  Load Automatic Reload Register (APR) to 1000 for 1 Second
9.	Finally enable TIM3 module

10. Set timer 3 handler in IVT if interrupt is enabled.

11. while loop put delay and toggled LED
		
*****************************************************/

/********** stm32f1 Register Address and Structures  ***************/
#include "stm32f1reg.h"

#define GPIO_PIN					(13)  // LED connected on PC13

/*********** Function declarations ****************/
int32_t main(void);
void delayMS(volatile uint32_t s);

//#include "stm32f1ivt.h"
/*********** Interrupt Vector Table ************************/

uint32_t (* const vector_table[])
__attribute__ ((section(".isr_vector"))) = {
	(uint32_t *) STACKINIT,         /* 0x000 Stack Pointer                   */
	(uint32_t *) main,              /* 0x004 Reset                           */
	0,                              /* 0x008 Non maskable interrupt          */
};


/*
 * Funtion Name		: delayMS
 * Description 		: millisecond delay using polling  without interrupt
                        Check if the 1 MHz timer reached to 1000.
                        1000 will generate 1 milli-second

 * Input			: mSec
 * Return Value		: None
*/
void delayMS(volatile uint32_t uiTime)
{
	for(; uiTime>0; uiTime--){

		TIM3->EGR |= 0x0001;        // Reset the timer
 	
    	while(TIM3->CNT < 1000);    // Wait until timer reaches to 1000
	}
}

/*************************************************
* Main code starts from here
*************************************************/
int32_t main(void)
{
   	//  Register RCC->APB2ENR
	// Enabling Clock for GPIOC RCC Register
    //  15     14     13   12   11    10    09    08   07   06    05    04    03   02    01   00
    //  Res    USERT  Res  SP1  TIM1  ADC2  ADC1  Res  Res  IOPE  IOPD  IOPC IOPB  IOPA  Res  AFIO
    //         1EN    -    EN   EN    EN    EN              EN    EN    EN   EN    EN         EN

    RCC->APB2ENR |= (1 << 4); // Enable GPIOC CLK
	RCC->APB1ENR |= (1 << 1); // Enable Timer 3 CLK

    // Make GPIOC output (PA1)  General Purpose Push Pull Output 2 Mhz
    // Conf : 0x0 Mode : 0x2 ------> 4 Bit 0x2 General Purpose Push Pull Output 2 Mhz
    // GPIOC->CRH |= 0x00020000; PIN_12         GPIOC->CRH |= 0x00200000; PIN_13

	GPIOC->CRH |= 0x00200000; // Configure 

	// Timer  Control Register 1
	//15-10		9-8      	7   	6-5   	 	4   	3    		2    	1 	 			0	
    //Res    	CKD[1:0]  	ARPE  	CMS  		DIR  	OPM  		URS  	UDIS  			CEN
    //         	00			0		00			0		0			0		0				0
    //        	No Div		NO	(Edge Align)  	UP	(Counter Run)	0		UpdateDisable	Counter Disable

	// Reset Control Register 1
	TIM3->CR1 = 0x0000; //Direction Up/Edige Align/One Pulse Diable/Update Disable/Timer Disable 

    // 16 Bit PSC Register 
	//Setting PSC
	// fCK_PSC / (PSC[15:0] + 1)
	// 8 Mhz / 7 + 1 = 1 Mhz timer clock speed
	TIM3->PSC = 7;

	// Timer 3 Auto Reload Register (APR) 16 Bit
    TIM3->ARR = 0xFFFF; //Setting APR to maximum value. Time will be controlleed in delayMS function

	// Finally enable TIM3 module
	TIM3->CR1 |= (1 << 0); //Enable CEN Bit

	while(1){
		delayMS(100);
		GPIOC->ODR ^= (1 << 13);
	}
	
	// Should never reach here
	return 0;
}


