/*
 * systick.c - H7 Audio prototype systick setup, encoder and button polling 
 * E. Brombaugh 07-07-2019
 */

#include "systick.h"
#include "debounce.h"
#include "led.h"

/*
 * Button Connections
 *
 * BTN_0  PC6
 *
 */
 
/* hookups for button */
#define ENC_A_PORT GPIOC
#define ENC_A_PIN GPIO_PIN_6
#define ENC_B_PORT GPIOC
#define ENC_B_PIN GPIO_PIN_7
#define ENC_E_PORT GPIOD
#define ENC_E_PIN GPIO_PIN_15

int16_t enc_val;
debounce_state btn_dbs[NUM_DBS];
uint8_t btn_fe[NUM_DBS], btn_re[NUM_DBS];
uint32_t pulse_cnt[1];

const GPIO_TypeDef *btn_ports[NUM_DBS] =
{
	ENC_A_PORT,
	ENC_B_PORT,
	ENC_E_PORT,
};

const uint16_t btn_pins[NUM_DBS] = 
{
	ENC_A_PIN,
	ENC_B_PIN,
	ENC_E_PIN,
};

const uint8_t btn_db_lens[NUM_DBS] =
{
	2,
	2,
	15,
};

const GPIO_PinState btn_state_act[NUM_DBS] =
{
	GPIO_PIN_RESET,	// most buttons active low
	GPIO_PIN_RESET,
	GPIO_PIN_RESET,
};


void systick_init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO Clock for encoder phases */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/* Enable PC6,7 for input for encoder phases */
	GPIO_InitStructure.Pin =  ENC_A_PIN | ENC_B_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(ENC_A_PORT, &GPIO_InitStructure);

	/* Enable GPIO Clock for encoder button */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/* Enable PD15 for input for encoder button */
	GPIO_InitStructure.Pin =  ENC_E_PIN;
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
	
	/* init LED pulse counters */
	pulse_cnt[0] = 0;
	
	/* No need to start the 1ms system tick since HAL does that for us */
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
 * pulse an LED
 */
void systick_pulse_led(uint8_t led, uint32_t duration)
{
	led = 0;
	LEDOn(led);
	pulse_cnt[led] = duration;
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
		debounce(&btn_dbs[i], (HAL_GPIO_ReadPin((GPIO_TypeDef *)btn_ports[i], btn_pins[i]) !=
				btn_state_act[i]));
		btn_fe[i] |= btn_dbs[i].fe;
		btn_re[i] |= btn_dbs[i].re;
	}
	
	/* decode encoder phases */
	if(btn_dbs[ENC_A].re)
	{
        if(btn_dbs[ENC_B].state == 0)
            enc_val++;
        else
            enc_val--;
    }
	
	/* handle LED pulsing */
	for(i=0;i<1;i++)
	{
		if(pulse_cnt[i] > 0)
		{
			pulse_cnt[i]--;
			
			if(pulse_cnt[i] == 0)
				LEDOff(i);
		}
	}
}
