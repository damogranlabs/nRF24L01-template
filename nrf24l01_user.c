/******************************************************************************
* File: nrf24l01_user.c
* Original source: S. Brennen Ball, 2006-2007 @ http://blog.diyembedded.com/
* Modified: Domen Jurkovic @ https://www.damogranlabs.com
*****************************************************************************/
#include "nrf24l01.h"

/******************************************************************************
 * TODO IO pin configuration:
 * User should configure GPIO pins before nRF24 init function.
 * - CE: GPIO OUT, Push-Pull, high state by default.
 * - CSN: GPIO OUT, Push-Pull, PULL UP, low state by default.
 * - IRQ: This is used so that the routines can poll for IRQ or create an ISR.
 *      - GPIO INPUT, PULL UP: Monitor pin state with (nrf24l01_irq_pin_active)  OR
 *      - GPIO INPUT, PULL UP: External interrupt on Fallin Edge
 *      Note: If interrupt pin as external interrupt is used, user should add any of
 *            "interrupt check/clear functions" to ISR and optionally read/write data.
 *            For instance, read data with: "nrf24l01_read_rx_payload()".
 *            Remember to clear interrupt source after it is handled!
 ******************************************************************************/
// #include "TODO_your_GPIO_lib.h"

/******************************************************************************
 * SPI configuration:
 * User should init SPI driver and SPI pins somewhere in the code before nRF24 init function:
 *  - 8-bit, MSB-first
 *  - CPOL=LOW, CPHA=LOW (1 Edge)
 *  - no hardware NSS/CSN output (CSN pin is handled in software by this library
 ******************************************************************************/
// #include "TODO_your_spi_lib.h"

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
// #include "TODO_your_delay_us_lib"
// #define delay_us(microseconds)   custom_name_delay_us(microseconds)

/******************************************************************************
 * TODO: Return `true` if IRQ pin is low, `false` otherwise.
 ******************************************************************************/
bool nrf24l01_irq_pin_active()
{
}

/******************************************************************************
 * TODO: Write low state on CE pin.
 ******************************************************************************/
void nrf24l01_clear_ce()
{
}

/******************************************************************************
 * TODO: Write high state on CE pin.
 ******************************************************************************/
void nrf24l01_set_ce()
{
}

/******************************************************************************
 * TODO: Return `true` if current CE pin state is high, `false` otherwise.
 ******************************************************************************/
bool nrf24l01_ce_pin_active()
{
}

/******************************************************************************
 * TODO: Write low state on CSN pin.
 ******************************************************************************/
void nrf24l01_clear_csn()
{
}

/******************************************************************************
 * TODO: Write high state on CSN pin.
 ******************************************************************************/
void nrf24l01_set_csn()
{
}

/******************************************************************************
 * TODO: Return `true` if current CSB pin state is high, `false` otherwise.
 ******************************************************************************/
bool nrf24l01_csn_pin_active()
{
}

/******************************************************************************
 * TODO: Send given `byte` over chosen SPI interface, wait and return received byte.
 ******************************************************************************/
uint8_t spi_send_read_byte(uint8_t byte)
{
}
