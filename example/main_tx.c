#include "nrf24l01.h"
#include <stdio.h>

#define NRF24L01_ADDRESS_SIZE 5 // // 5 bytes for RX/TX address
#define PAYLOAD_SIZE 4          // send four bytes at a time
#define NRF24L01_PIPE_NUM 0     // pipeline number

#define NRF24Ll01_TIMEOUT 20 // [ms]
#define NRF24L01_RX_DELAY 50 // [ms]

int main(void)
{
  uint8_t f_addr[NRF24L01_ADDRESS_SIZE]; // nrf24l01 with 5-byte address width
  uint8_t data[PAYLOAD_SIZE];

  /* Configure (see *nrf24l01_user.c*):
    - GPIO pins
    - Microseconds delay
    - SPI interface
  */
  nrf24l01_power_down();
  nrf24l01_initialize_debug(false, PAYLOAD_SIZE, false);

  f_addr[0] = f_addr[1] = f_addr[2] = f_addr[3] = f_addr[4] = NRF24L01_TX_ADDR_B0_DEFAULT_VAL;
  nrf24l01_set_tx_addr(f_addr, NRF24L01_ADDRESS_SIZE);

  nrf24l01_clear_flush();

  data[0] = 0;
  data[1] = 1;
  data[2] = 2;
  data[3] = 3;
  data[4] = 4;

  while (1)
  {
    // IRQ pin used as normal GPIO, not interrupt-driven pin.
    nrf24l01_write_tx_payload(&data, PAYLOAD_SIZE, true); // transmit received characters over RF
    while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active()))
      ;

    // delay
  }
}