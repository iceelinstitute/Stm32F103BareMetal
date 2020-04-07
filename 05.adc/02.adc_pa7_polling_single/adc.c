/*
 * File Name  : adc.c 
 *
 * Description:
 *              : Reads single channel ADC channel 7 (PA07)
 *					Single Channel
 *                  Software Trigger for start of conversion
 *                  Polling Mode
 *                  if binary value is less than  0x800 it will switch on LED connected to PC13  
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
 * ADC Channel used       : PA7 (Channel 7)
 *							
 * Step for generating bin : make
 */

/*************STEPS for ADC1 **********************

1.	Enable Clock for GPIOA Port,Analog Block, and Alternate Function (RCC->APB2ENR)
2.	Configure PC13 pin as Output Push Pull and PB5 as alternate function (GPIOC->CRH)

3.  Configure PA7 as analog input (GPIOA->CRL)

4.  Reset ADC1 Control Register 1 (ADC1->CR1)
5.  Configure ADC1->SQR1 for single Conversion (0x00000000)
6.  Select channel 7 (PA7) as first channel in ADC1->SQR3 =  (Channel Number << 0);
7.  Select External event select for regular group SWSTART by setting 111 in bit 17,18 & 19 ADC1->CR2 (Bit 17 18 19)
8.  Enable Conversion on external event setting bit 10 of ADC1->CR2  (Bit 20)
9.  Selection Single conversion mode by setting bit 1 of ADC1->CR2 (Bit 1)
10. Switch on A/D converter by setting bit 0 of ADC1->CR2 (Bit 0)
11. Wait for A/D Stabilization (T-stab time)
12. Reset calibration  by setting bit 3 of ADC1->CR2 (Bit 3)
13. Wait for calibration reset  (Bit 3 of ADC1->CR2 should be 0)(Bit 3)
12. Start calibration by setting bit 2 of ADC1->CR2 (Bit 2)
13. Wait for calibration completed (Bit 2 of ADC1->CR2 should be 0)(Bit 2)

14. Make Start of conversion for first reading by setting bit 22 of ADC1->CR2 (Bit 22)
15. Check conversion has started (bit 4 of ADC1->SR will be set for regular channel)
15. Wait until conversion is over (wait until bit 1 of ADC1->SR is set to 1)
16. Read ADC1->DR register to read the binary value of Analog channel 0 (PA0);
17. Set ADC1->SR = 0x0; to reset EOC flag.


18. For repeated conversion, follow step 14 to 17

19. If ADC1->DR is less than 0x800 (1.8V), Switch ON LED (PC13) else switch off LED

*****************************************************/

/*************************************************
* Definitions
*************************************************/
#define int32_t         int
#define int16_t         short
#define int8_t          char
#define uint32_t        unsigned int
#define uint16_t        unsigned short
#define uint8_t         unsigned char


// Define the base addresses for peripherals
#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define SRAM_BASE       ((uint32_t) 0x20000000)

#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)

#define GPIOA_BASE      (PERIPH_BASE + 0x10800) // GPIOC base address is 0x40011000
#define GPIOC_BASE      (PERIPH_BASE + 0x11000) // GPIOC base address is 0x40011000
#define ADC1_BASE       (APB2PERIPH_BASE + 0x2400) //  ADC1 base address is 0x40012400
#define RCC_BASE        ( AHBPERIPH_BASE + 0x1000) //   RCC base address is 0x40021000

#define STACKINIT       0x20002000

#define GPIOA   ((GPIO_type *)  GPIOA_BASE)
#define GPIOC   ((GPIO_type *)  GPIOC_BASE)
#define ADC1            ((ADC_type   *)  ADC1_BASE)
#define RCC             ((RCC_type   *)   RCC_BASE)


/*
 * Register Addresses
 */
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
	uint32_t CFGR2;    /* RCC clock configuration register 2,        Address offset: 0x2C */
} RCC_type;

typedef struct
{
	uint32_t SR;        /* Address offset: 0x00 */
	uint32_t CR1;       /* Address offset: 0x04 */
	uint32_t CR2;       /* Address offset: 0x08 */
	uint32_t SMPR1;     /* Address offset: 0x0C */
	uint32_t SMPR2;     /* Address offset: 0x10 */
	uint32_t JOFR1;     /* Address offset: 0x14 */
	uint32_t JOFR2;     /* Address offset: 0x18 */
	uint32_t JOFR3;     /* Address offset: 0x1C */
	uint32_t JOFR4;     /* Address offset: 0x20 */
	uint32_t HTR;       /* Address offset: 0x24 */
	uint32_t LTR;       /* Address offset: 0x28 */
	uint32_t SQR1;      /* Address offset: 0x2C */
	uint32_t SQR2;      /* Address offset: 0x30 */
	uint32_t SQR3;      /* Address offset: 0x34 */
	uint32_t JSQR;      /* Address offset: 0x38 */
	uint32_t JDR1;      /* Address offset: 0x3C */
	uint32_t JDR2;      /* Address offset: 0x40 */
	uint32_t JDR3;      /* Address offset: 0x44 */
	uint32_t JDR4;      /* Address offset: 0x48 */
	uint32_t DR;        /* Address offset: 0x4C */
} ADC_type;



int32_t main(void);

/*************************************************
* Vector Table
*************************************************/

uint32_t (* const vector_table[])
__attribute__ ((section(".vectors"))) = {
	(uint32_t *) STACKINIT,         /* 0x000 Stack Pointer                   */
	(uint32_t *) main,              /* 0x004 Reset                           */
	0,                              /* 0x008 Non maskable interrupt          */
};


/*************************************************
* Main code starts from here
*************************************************/
int32_t main(void)
{

    unsigned int adc_data;
    int i;
	//  Register RCC->APB2ENR
    // Enabling Clock for GPIOC RCC Register
    //  15     14     13   12   11    10    09    08   07   06    05    04    03   02    01   00
    //  Res    USERT  Res  SP1  TIM1  ADC2  ADC1  Res  Res  IOPE  IOPD  IOPC IOPB  IOPA  Res  AFIO
    //         1EN    -    EN   EN    EN    EN              EN    EN    EN   EN    EN         EN
    RCC->APB2ENR |= (1 << 0); // Enable alternate function clock.
    RCC->APB2ENR |= (1 << 4); // Enable GPIOC CLK for PC13
    RCC->APB2ENR |= (1 << 9); // Enable clock for ADC1 clock.

    GPIOC->CRH |= 0x00200000; // Make GPIOC Pin13 output (PC13)

    // Setting PA7 as analog input
    GPIOA->CRL &= 0x0FFFFFFF; // PA7 Analog Mode : 00 (Input)
    						  //			Conf : 00 (Analog)
    GPIOA->CRL |= 0x00000000;

	
	//ADC1->CR1
	//   [31-24]  23      22    [21-20]   [19-16]      [15-13]        
	//   Res     AWDEN JAWDEN    Res     DUALMOD[3:0] DISCNUM[2:0]   
	//   [15-13]     12      11      10    9         8    7      6       5       [4-0]
	// DISCNUM[2:0] JDISCEN DISCEN  JAUTO AWDSGL    SCAN JEOCIE AWDIE   EOCIE   AWDCH[4:0]
	ADC1->CR1 = 0x00000000; // RESSET MODE

	// Single conversion
	ADC1->SQR1 = 0x00000000; // RESET VALUE

    // Selecting Channel 7 (PA7)
    // For changing channel following line is required.
    // ADC1->SQR3 = (channelnumber << 0); // Ch0 -> PA0 Ch1 -> PA1 ..... Ch7 -> PA7 Ch8 -> PB0 Ch9 -> PB1 
	ADC1->SQR3 = (7 << 0);  // Channel 7

	
	//ADC1->CR2
	//  [31-24]     23           22         21          20        [19-17]           16        
	//  Res         TSVREFE      SWSTART    JSWSTART    EXTTRIG   EXTSEL[2:0]       Res
	//  15        [14  13  12]  11      [10-9]  8   [7   6   5   4]   3         2       1       0
    //  JEXTTRIG  JEXTSEL[2:0]  ALIGN   Res     DMA     Res           RSTCAL    CAL     CONT    ADON
	
	ADC1->CR2 |= (7 << 17); // Select SWSTART as trigger
	ADC1->CR2 |= (1 << 20); // Enable external trigger
    ADC1->CR2 &= ~(1<<1);   // Single Conversion Mode
    ADC1->CR2 |= (1 << 0); // Enable A/D conversion
	
	// Waiting for ADC Power On Stablization 
	// This much delay may bot required
	for (i = 0; i < 50; ++i) __asm__("nop");
	// Waiting for stabilization

	// Calibration reset and start
	//    Optional for better accuracy.
	ADC1->CR2 |= (1 << 3);
	while((ADC1->CR2 & (1 << 3)));
	ADC1->CR2 |= (1 << 2);
	while((ADC1->CR2 & (1 << 2)));

    /* Single Conversion */
    ADC1->CR2 |=  (1<<22); // SWSTART/
	// ADC1->SR will become 0x12 after start of conversion
	// ADC1->SR bits : [31-5]   4       3       2       1       0
	//                  Res     STRT    JSTRT   JEOC    EOC     AWD
	
	if( (ADC1->SR & 0x00000010) == 0x00000010) //Before reading ADC1->SR = 0x12 if end of conversion is over
	{
	    // Reading ADC->SR will reset End of Conversion field
		while( (ADC1->SR & (1 <<1) ) );// Output will be  ADC1->SR = 0x02
		adc_data=ADC1->DR & 0x0000FFFF;
	}
	ADC1->SR = 0x0;
	/* Single Conversion Over */
	
	
	// While loop ADC reading with Software Start
    while(1)
	{
		ADC1->CR2 |=  (1<<22); // SWSTART/
		
		// ADC1->SR will become 0x12 after start of conversion
		if( (ADC1->SR & 0x00000010) == 0x00000010) //Before reading ADC1->SR = 0x12 if end of conversion is over
		{
			// Reading ADC->SR will reset End of Conversion field
			while( (ADC1->SR & (1 <<1) ) );// Output will be  ADC1->SR = 0x02
			adc_data=ADC1->DR & 0x0000FFFF;
		}

		if(adc_data < 0x800)
			GPIOC->BRR = (1 << 13);  //Switch ON LED
		else
			GPIOC->BSRR = (1 << 13); //Switch OFF LED

//      Convert in voltage and comparition 
//      0V = 0x000   :   3.3V=0xfff      
/*
		float voltage = (float)(adc_data-0.0)*3.3/4096.0; 
		if(voltage > 1.8)
			GPIOC->BRR = (1 << 13);
		else
			GPIOC->BSRR = (1 << 13);
*/			
		ADC1->SR = 0x0;
	}
    while(1);

	
	return 0;
}


