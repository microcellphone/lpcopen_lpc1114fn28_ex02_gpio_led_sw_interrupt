/*
===============================================================================
 Name        : lpcopen_lpc1114fn28_ex01_gpio_led_sw.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "led.h"
#include "sw.h"

// TODO: insert other definitions and declarations here

uint8_t SW_Status = 0;

void PIOINT0_IRQHandler(void)
{
	uint32_t status = Chip_GPIO_GetMaskedInts(LPC_GPIO, 0);

    if (status&(1<<SW3_BIT))  {
    	/* Clear interrupt */
    	Chip_GPIO_ClearInts(LPC_GPIO, SW3_PORT, (1 << SW3_BIT));
    }

    if (status&(1<<SW4_BIT))  {
    	/* Clear interrupt */
    	Chip_GPIO_ClearInts(LPC_GPIO, SW4_PORT, (1 << SW4_BIT));
    }
}

void PIOINT1_IRQHandler(void)
{
	uint32_t status = Chip_GPIO_GetMaskedInts(LPC_GPIO, 1);

    if (status&(1<<SW1_BIT))  {
    	/* Clear interrupt */
    	Chip_GPIO_ClearInts(LPC_GPIO, SW1_PORT, (1 << SW1_BIT));
    	SW_Status = 1;
    }

    if (status&(1<<SW2_BIT))  {
    	/* Clear interrupt */
    	Chip_GPIO_ClearInts(LPC_GPIO, SW2_PORT, (1 << SW2_BIT));
    }
}


int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here
    bool led;

    SysTick_Config(SystemCoreClock/1000 - 1);

    IOCON_Config_Request();
    LED1_Config_Request();
    SW1_Config_Request();
    led = 0;

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
    	if(SW_Status == 0)	  led = true;
    	else {
    		led = false;
    		SW_Status = 0;
    	}
    	LED_Set_Request(LED1, led);
    	Delay (1000) ;
    	i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
