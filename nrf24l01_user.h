/******************************************************************************
* File: nrf24l01_user.h
* Original source: S. Ball, 2006-2007 @ http://blog.diyembedded.com/
* Modified: Domen Jurkovic @ https://www.damogranlabs.com
*****************************************************************************/
#ifndef __NRF24L01_USER_H
#define __NRF24L01_USER_H

#include <stdint.h>  // 'uint_x' type definition
#include <stdbool.h> // 'bool' type definition

/******************************************************************************
 * TODO Include: 
 * - CPU specific register definition header 
 * - SPI specific header
******************************************************************************/
#include "stm32f0xx_hal.h"     // register header file
#include "stm32f0xx_hal_spi.h" // SPI header file

/******************************************************************************
 * TODO SPI configuration:
 * User should init SPI driver and SPI pins somewhere in the code before nRF24 init function:
 *  - 8-bit, MSB-first
 *  - CPOL=LOW, CPHA=LOW (1 Edge)
 *  - no hardware NSS/CSN output (CSN pin is handled in software by this library
 * Also:
 * - add any SPI specific definitions, for example: rx/tx timeout value
 * - add SPI handler compatible to your SPI driver functions
******************************************************************************/
extern SPI_HandleTypeDef hspi1; // SPI handler
#define SPI_TIMEOUT_VALUE 100   //[ms] transfer function timeout considered as error

/******************************************************************************
 * TODO Delay function:
 * The user must define a function that delays for the specified number of 
 *  microseconds. This function needs to be as precise as possible, and the use 
 *  of a timer module within your microcontroller is highly recommended.
 * Init delay before nRF init function.
 * 
 * If your delay function has different name, uncomment #define below and rename 
 *  "custom_name_delay_us". You should also change the include file name below to 
 *  whatever the name of your  delay include file is.
******************************************************************************/
#include "stm32xx_delay_us.h" // include header for us delay function
//#define delay_us(microseconds)     // custom_name_delay_us(microseconds)

/******************************************************************************
 * TODO IO pin configuration:
 * User should configure pins somewhere before nRF24 init function
 * - CE: GPIO OUT, Push-Pull (chip enable - rx/tx mode)
 * - CSN: GPIO OUT, Push-Pull, PULL UP (chip select)
 * - IRQ: This is used so that the routines can poll for IRQ or create an ISR.
 *      - GPIO INPUT, PULL UP: Monitor pin state with (nrf24l01_irq_pin_active)  OR
 *      - GPIO INPUT, PULL UP: External interrupt on Fallin Edge
 *      Note: If interrupt pin as external interrupt is used, user should add any of 
 *            "interrupt check/clear functions" to ISR and optionally read/write data.
 *            For instance, read data with: "nrf24l01_read_rx_payload()".
 *            Remember to clear interrupt source after it is handled!
******************************************************************************/
#include "stm32f0xx_hal_gpio.h" // GPIO header file

/******************************************************************************
 * IO interface-related functions (change according to your hardware in nrf24l01_user.c file)
******************************************************************************/
bool nrf24l01_irq_pin_active(void); // Edit this function to read your chosen GPIO pin
void nrf24l01_clear_ce(void);       // Edit this function to set/clear your chosen GPIO pin
void nrf24l01_set_ce(void);         // Edit this function to set/clear your chosen GPIO pin
bool nrf24l01_ce_pin_active(void);  // Edit this function to read your chosen GPIO pin
void nrf24l01_clear_csn(void);      // Edit this function to set/clear your chosen GPIO pin
void nrf24l01_set_csn(void);        // Edit this function to set/clear your chosen GPIO pin
bool nrf24l01_csn_pin_active(void); // Edit this function to read your chosen GPIO pin

uint8_t spi_send_read_byte(uint8_t byte); // Edit this function to your SPI library

#endif /*__NRF24L01_USER_H */
