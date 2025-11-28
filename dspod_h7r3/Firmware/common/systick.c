/*
 * systick.c - dspod h7r3 systick setup, encoder and button polling 
 * E. Brombaugh 11-14-2025
 */

#include "systick.h"
#include "debounce.h"

/*
 * Button Connections for dspod h7r3 & module
 *
 * btn1  PB7	NC
 * btn2  PB8	TAP
 * btn3  PB9	ENC E
 * btn4  PC13	ENC B
 * btn5  PC14	ENC A
 *
 */
 
/* hookups for buttons */
#define ENC_A_PORT GPIOC
#define ENC_A_PIN GPIO_PIN_14
#define ENC_B_PORT GPIOC
#define ENC_B_PIN GPIO_PIN_13
#define ENC_E_PORT GPIOB
#define ENC_E_PIN GPIO_PIN_9
#define TAP_PORT GPIOB
#define TAP_PIN GPIO_PIN_8

int16_t enc_val;
debounce_state btn_dbs[NUM_DBS];
uint8_t btn_fe[NUM_DBS], btn_re[NUM_DBS];

const GPIO_TypeDef *btn_ports[NUM_DBS] =
{
	ENC_A_PORT,
	ENC_B_PORT,
	ENC_E_PORT,
	TAP_PORT,
};

const uint16_t btn_pins[NUM_DBS] = 
{
	ENC_A_PIN,
	ENC_B_PIN,
	ENC_E_PIN,
	TAP_PIN,
};

const uint8_t btn_db_lens[NUM_DBS] =
{
	2,
	2,
	15,
	15,
};

const GPIO_PinState btn_state_act[NUM_DBS] =
{
	GPIO_PIN_RESET,	// most buttons active low
	GPIO_PIN_RESET,
	GPIO_PIN_RESET,
	GPIO_PIN_RESET,
};


void systick_init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO Clock for encoder phases */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/* Enable PC14,13 for input for encoder phases */
	GPIO_InitStructure.Pin =  ENC_A_PIN | ENC_B_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(ENC_A_PORT, &GPIO_InitStructure);

	/* Enable GPIO Clock for encoder and tap buttons */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Enable PB9,8 for input for encoder and tap buttons */
	GPIO_InitStructure.Pin =  ENC_E_PIN | TAP_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(ENC_E_PORT, &GPIO_InitStructure);

	/* set up debounce objects for buttons & encoder */
	for(i=0;i<NUM_DBS;i++)
	{
		init_debounce(&btn_dbs[i], btn_db_lens[i]);
		btn_fe[i] = 0;
		btn_re[i] = 0;
	}
    enc_val = 0;
}

/*
 * check status of buttons
 */
uint8_t systick_get_button(uint8_t btn)
{
	return btn_dbs[btn].state;
}

/*
 * check for falling edge of buttons
 */
uint8_t systick_button_fe(uint8_t btn)
{
	uint8_t result = btn_fe[btn];
	btn_fe[btn] = 0;
	return result;
}

/*
 * check for rising edge of buttons
 */
uint8_t systick_button_re(uint8_t btn)
{
	uint8_t result = btn_re[btn];
	btn_re[btn] = 0;
	return result;
}

/*
 * get state of encoder shaft
 */
int16_t systick_get_enc(void)
{
	int16_t result = enc_val;
	enc_val = 0;
	return result;
}

/*
 * wrapper for H7R3 encoder + button
 */
uint8_t encoder_poll(int16_t *rtn_enc_val, uint8_t *rtn_enc_btn)
{
	uint8_t result = 0;
	
	*rtn_enc_val = 0;
	*rtn_enc_btn = 0;
	
	if(enc_val != 0)
	{
		result++;
		*rtn_enc_val = enc_val;
		enc_val = 0;
	}
	
	if(btn_fe[ENC_E])
	{
		result++;
		*rtn_enc_btn = 1;
		btn_fe[ENC_E] = 0;
	}
	
	return result;
}

/*
 * SysTick IRQ handler runs at 1000Hz, checks for buttons, encoder, etc.
 */
void SysTick_Handler(void)
{
	uint8_t i;
	
	/* Needed by HAL! */
	HAL_IncTick();
    
	/* gather inputs and run all debouncers */
	for(i=0;i<NUM_DBS;i++)
	{
		debounce(&btn_dbs[i], (HAL_GPIO_ReadPin((GPIO_TypeDef *)btn_ports[i], btn_pins[i]) ==
				btn_state_act[i]));
		btn_fe[i] |= btn_dbs[i].fe;
		btn_re[i] |= btn_dbs[i].re;
	}
	
	/* decode encoder phases */
	int16_t enc_del = 0;
	/* equivalent to linux "half-step" method (2 transitions/step) */
    if(btn_dbs[ENC_B].re)
	{
        if(btn_dbs[ENC_A].state == 1)
            enc_del++;
        else
            enc_del--;
    }
    else if(btn_dbs[ENC_B].fe)
    {
        if(btn_dbs[ENC_A].state == 0)
            enc_del++;
        else
            enc_del--;
    }
	enc_val += enc_del;
}
