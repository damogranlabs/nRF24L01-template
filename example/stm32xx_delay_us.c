 /**
 ******************************************************************************
 * File Name          : stm32xx_hal_delay
 * Description        : This file provides code for the configuration
 *                      of microsecond delay.
 * @date    1-Nov-2018
 * @author  Domen Jurkovic, Damogran Labs
 * @source  http://damogranlabs.com/
 *          https://github.com/damogranlabs
 * @version v1.4
 *
 * @attention
 * This library doesn't generate precise us delay, so be careful - take a few us more/less.
 *
 *  1. initialize library with: delay_us_init();
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32xx_delay_us.h"

/* Private variables ------------------------------------------------------------------*/
volatile uint32_t us_multiplier;

/**
 * @brief Init microsecond delay
 * @note	for core clock frequency above 1MHz
 * @note	Doesn't generate precise us delay. For simple purposes.
 */
void delay_us_init(void) {
    us_multiplier = HAL_RCC_GetSysClockFreq() / 1000000; //For 1 us delay, we need to divide with 1M */
}

/**
 * @brief Delay for number of microsecond (aproximately)
 * @param	micros - number of microseconds (>= 1)
 */
void delay_us(volatile uint32_t micros){
	micros *= us_multiplier;
	
    /* Here, the suppression fails because the push/pop must enclose the whole function. */ 
    //substitute 8 cycles for each call of asm code below == //micros /= 8; 
    //__asm("mov r0,  #micros");
    __asm("mov r0, %0" : "=r" (micros));
    __asm("loop: sub r0, #8");
    __asm("   bne loop");
  
}
