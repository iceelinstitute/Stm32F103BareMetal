#ifndef STM32F1REG_H
#define STM32F1REG_H



// Define some types for readibility
#define int32_t         int
#define int16_t         short
#define int8_t          char
#define uint32_t        unsigned int
#define uint16_t        unsigned short
#define uint8_t         unsigned char

// Define the base addresses for peripherals
#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define SYSTICK_BASE    ((uint32_t) 0xE000E010)

#define GPIOC_BASE      (PERIPH_BASE + 0x11000) // GPIOC base address is 0x40011000
#define GPIOD_BASE      (PERIPH_BASE + 0x11400) // GPIOD base address is 0x40011400
#define GPIOE_BASE      (PERIPH_BASE + 0x11800) // GPIOE base address is 0x40011800
#define RCC_BASE        (PERIPH_BASE + 0x21000) //   RCC base address is 0x40021000

#define STACKINIT       0x20002000
#define DELAY           70000

#define GPIOC   ((GPIO_type *)  GPIOC_BASE)
#define GPIOD   ((GPIO_type *)  GPIOD_BASE)
#define GPIOE   ((GPIO_type *)  GPIOE_BASE)
#define RCC     ((RCC_type *)     RCC_BASE)
#define SYSTICK ((STK_type *) SYSTICK_BASE)

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
	uint32_t CFGR2;    /* RCC clock configuration register2,         Address offset: 0x2C */
} RCC_type;

typedef struct
{
	uint32_t CSR;      /* SYSTICK control and status register,       Address offset: 0x00 */
	uint32_t RVR;      /* SYSTICK reload value register,             Address offset: 0x04 */
	uint32_t CVR;      /* SYSTICK current value register,            Address offset: 0x08 */
	uint32_t CALIB;    /* SYSTICK calibration value register,        Address offset: 0x0C */
} STK_type;

#endif

