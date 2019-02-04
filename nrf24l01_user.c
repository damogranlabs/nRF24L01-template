/******************************************************************************
* File: nrf24l01_user.c
* Original source: S. Brennen Ball, 2006-2007 @ http://blog.diyembedded.com/
* Modified: Domen Jurkovic @ https://www.damogranlabs.com
*****************************************************************************/
#include "nrf24l01.h"
#include "nrf24l01_user.h"

/******************************************************************************
 * TODO: Add IRQ pin read function
 * Function must return true if IRQ pin is low, false otherwise
 * 
******************************************************************************/
bool nrf24l01_irq_pin_active()
{
    if (HAL_GPIO_ReadPin(NRF24L01_IRQ_GPIO_Port, NRF24L01_IRQ_Pin) == GPIO_PIN_RESET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * TODO: Add CE pin write function, state = LOW
******************************************************************************/
void nrf24l01_clear_ce()
{
    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin, GPIO_PIN_RESET); // change write function
}

/******************************************************************************
 * TODO: Add CE pin write function, state = HIGH
******************************************************************************/
void nrf24l01_set_ce()
{
    HAL_GPIO_WritePin(NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin, GPIO_PIN_SET); // change write function
}

/******************************************************************************
 * TODO: Add CE pin state read function
 * Function must return true if CE is HIGH, false if not.
******************************************************************************/
bool nrf24l01_ce_pin_active()
{
    if (HAL_GPIO_ReadPin(NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin) == GPIO_PIN_SET)
    { // change read function
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * TODO: Add CSN pin write function, state = LOW
******************************************************************************/
void nrf24l01_clear_csn()
{
    HAL_GPIO_WritePin(NRF24L01_CSN_GPIO_Port, NRF24L01_CSN_Pin, GPIO_PIN_RESET); // change write function
}

/******************************************************************************
 * TODO: Add CSN pin write function, state = HIGH
******************************************************************************/
void nrf24l01_set_csn()
{
    HAL_GPIO_WritePin(NRF24L01_CSN_GPIO_Port, NRF24L01_CSN_Pin, GPIO_PIN_SET); // change write function
}

/******************************************************************************
 * TODO: Add CSN pin read function
 * Function must return true if CSN is HIGH, false if not.
******************************************************************************/
bool nrf24l01_csn_pin_active()
{
    if (HAL_GPIO_ReadPin(NRF24L01_CSN_GPIO_Port, NRF24L01_CSN_Pin) == GPIO_PIN_SET)
    { // change read function
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************
 * TODO: This function should take the argument uint8_t 'byte' and send it through
 *  the SPI port to the 24L01. Then, it should wait until the 24L01 has returned
 *  its response over SPI. 
 *  Received byte should be the return value of the function.
******************************************************************************/
uint8_t spi_send_read_byte(uint8_t byte)
{
    uint8_t data_in;

    // implement SPI transmit & receive function (1 byte)
    if (HAL_SPI_TransmitReceive(&hspi1, &byte, &data_in, 1, SPI_TIMEOUT_VALUE) != HAL_OK)
    {
        // report & handle error if needed!
    }

    return data_in;
}
