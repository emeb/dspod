/*
 * riscv_top.h - hardware definitions for riscv_top design
 * 06-04-26 E. Brombaugh
 */

#ifndef __riscv_top__
#define __riscv_top__

#include <stdint.h>

// 32-bit parallel out
#define gp_out (*(volatile uint32_t *)0x20000000)

// bit fields within gp_out
#define GP_OUT_ALGO_MASK 255
#define GP_OUT_ALGO_POS 0
#define GP_OUT_ALGO (GP_OUT_ALGO_MASK << GP_OUT_ALGO_POS)
#define GP_OUT_LED_BLUE_MASK 1
#define GP_OUT_LED_BLUE_POS 17
#define GP_OUT_LED_BLUE (GP_OUT_LED_BLUE_MASK << GP_OUT_LED_BLUE_POS)
#define GP_OUT_LED_GREEN_MASK 1
#define GP_OUT_LED_GREEN_POS 18
#define GP_OUT_LED_GREEN (GP_OUT_LED_GREEN_MASK << GP_OUT_LED_GREEN_POS)
#define GP_OUT_LED_RED_MASK 1
#define GP_OUT_LED_RED_POS 19
#define GP_OUT_LED_RED (GP_OUT_LED_RED_MASK << GP_OUT_LED_RED_POS)
#define GP_OUT_LED_MASK 0x7
#define GP_OUT_LED_POS 17
#define GP_OUT_LED (GP_OUT_LED_MASK << GP_OUT_LED_POS)
#define GP_OUT_ENCVCLR_MASK 1
#define GP_OUT_ENCVCLR_POS 27
#define GP_OUT_ENCVCLR (GP_OUT_ENCVCLR_MASK << GP_OUT_ENCVCLR_POS)
#define GP_OUT_GATEOUT_MASK 1
#define GP_OUT_GATEOUT_POS 28
#define GP_OUT_GATEOUT (GP_OUT_GATEOUT_MASK << GP_OUT_GATEOUT_POS)
#define GP_OUT_LCD_BL_MASK 1
#define GP_OUT_LCD_BL_POS 29
#define GP_OUT_LCD_BL (GP_OUT_LCD_BL_MASK << GP_OUT_LCD_BL_POS)
#define GP_OUT_LCD_DC_MASK 1
#define GP_OUT_LCD_DC_POS 30
#define GP_OUT_LCD_DC (GP_OUT_LCD_DC_MASK << GP_OUT_LCD_DC_POS)
#define GP_OUT_LCD_RST_MASK 1
#define GP_OUT_LCD_RST_POS 31
#define GP_OUT_LCD_RST (GP_OUT_LCD_RST_MASK << GP_OUT_LCD_RST_POS)

// 32-bit parallel in
#define gp_in  (*(volatile uint32_t *)0x20000004)

// bit fields within gp_in
#define GP_IN_BUTTON_RAW_MASK 0x1ff
#define GP_IN_BUTTON_RAW_POS 0
#define GP_IN_BUTTON_RAW (GP_IN_BUTTON_RAW_MASK << GP_IN_BUTTON_RAW_SHF)
#define GP_IN_BUTTON_DEBOUNCE_MASK 0x1f
#define GP_IN_BUTTON_DEBOUNCE_POS 9
#define GP_IN_BUTTON_DEBOUNCE (GP_IN_BUTTON_DEBOUNCE_MASK << GP_IN_BUTTON_DEBOUNCE_SHF)
#define GP_IN_BUTTON_ENCVAL_MASK 0xff
#define GP_IN_BUTTON_ENCVAL_POS 14
#define GP_IN_BUTTON_ENCVAL (GP_IN_BUTTON_ENCVAL_MASK << GP_IN_BUTTON_ENCVAL_POS)
#define GP_IN_HW_PROG_MASK 0x1
#define GP_IN_HW_PROG_POS 31

// 32-bit clock counter
#define clkcnt_reg (*(volatile uint32_t *)0x50000000)

// ADC
#define ADC_BASE 0x60000000

typedef struct
{
	volatile uint16_t CHL0;
	uint16_t reserved0;
	volatile uint16_t CHL1;
	uint16_t reserved1;
	volatile uint16_t CHL2;
	uint16_t reserved2;
	volatile uint16_t CHL3;
	uint16_t reserved3;
} ADC_TypeDef;

#define ADC ((ADC_TypeDef *) ADC_BASE)
#define ADC_get_chl(x) (*((uint16_t *)ADC + (x * 2)))

// System Control
#define sysctrl_reg (*(volatile uint32_t *)0x70000000)
#define SYSCTRL_CPURST 0x1
#define SYSCTRL_ROMREMAP 0x2

// level detect
#define LVL_BASE 0x80000000

typedef struct
{
	volatile uint16_t L_RX;
	uint16_t reserved0;
	volatile uint16_t R_RX;
	uint16_t reserved1;
	volatile uint16_t L_TX;
	uint16_t reserved2;
	volatile uint16_t R_TX;
	uint16_t reserved3;
} LVL_TypeDef;

#define LVL ((LVL_TypeDef *) LVL_BASE)

// Midiverb microcode store
#define MVB_BASE 0x90000000

// ACIA serial
#define acia_ctlstat (*(volatile uint8_t *)0x30000000)
#define acia_data (*(volatile uint8_t *)0x30000004)

// SPI cores @ BUS_ADDR74 = 0b0000 and 0b0010
#define SPI0_BASE 0x40000000
#define SPI1_BASE 0x40000080

typedef struct
{
	uint32_t reserved0[8];
	volatile uint8_t SPICR0;
	uint8_t reserved1[3];
	volatile uint8_t SPICR1;
	uint8_t reserved2[3];
	volatile uint8_t SPICR2;
	uint8_t reserved3[3];
	volatile uint8_t SPIBR;
	uint8_t reserved4[3];
	volatile uint8_t SPISR;
	uint8_t reserved5[3];
	volatile uint8_t SPITXDR;
	uint8_t reserved6[3];
	volatile uint8_t SPIRXDR;
	uint8_t reserved7[3];
	volatile uint8_t SPICSR;
	uint8_t reserved8[3];
} SPI_TypeDef;

#define SPI0 ((SPI_TypeDef *) SPI0_BASE)
#define SPI1 ((SPI_TypeDef *) SPI1_BASE)

// I2C cores @ BUS_ADDR74 = 0b0001 and 0b0011
#define I2C0_BASE 0x40000040
#define I2C1_BASE 0x400000C0

typedef struct
{
	uint32_t reserved0;			// 0
	uint32_t reserved1;			// 1
	uint32_t reserved2;			// 2
	volatile uint8_t I2CSADDR;	// 3
	uint8_t reserved3[3];
	uint32_t reserved4;			// 4
	uint32_t reserved5;			// 5
	volatile uint8_t I2CIRQ;	// 6
	uint8_t reserved6[3];
	volatile uint8_t I2CIRQEN;	// 7
	uint8_t reserved7[3];
	volatile uint8_t I2CCR1;	// 8
	uint8_t reserved8[3];
	volatile uint8_t I2CCMDR;	// 9
	uint8_t reserved9[3];
	volatile uint8_t I2CBRLSB;	// A
	uint8_t reservedA[3];
	volatile uint8_t I2CBRMSB;	// B
	uint8_t reservedB[3];
	volatile uint8_t I2CSR;		// C
	uint8_t reservedC[3];
	volatile uint8_t I2CTXDR;	// D
	uint8_t reservedD[3];
	volatile uint8_t I2CRXDR;	// E
	uint8_t reservedE[3];
	volatile uint8_t I2CGCDR;	// F
	uint8_t reservedF[3];
} I2C_TypeDef;

#define I2C0 ((I2C_TypeDef *) I2C0_BASE)
#define I2C1 ((I2C_TypeDef *) I2C1_BASE)

#endif