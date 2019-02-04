/* ...*/
#include <stdio.h>
#include "nrf24l01.h"

#define NRF24L01_ADDRESS_SIZE 5 // // 5 bytes for RX/TX address
#define PAYLOAD_SIZE 4          // send four bytes at a time
#define NRF24L01_PIPE_NUM 0     // pipeline number

#define NRF24L01_RETRY 3
#define NRF24Ll01_TIMEOUT 20 // [ms]
#define NRF24L01_RX_DELAY 50 // [ms]

/* ...*/

int main(void)
{
  uint8_t f_addr[NRF24L01_ADDRESS_SIZE]; // nrf24l01 with 5-byte address width
  uint8_t *data[PAYLOAD_SIZE];

  /*
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  ...
  */

  // NRF24L01+ init
  // delay_us_init();
  nrf24l01_power_down();
  //initialize the 24L01 to the debug configuration as TX, 4 data byte, and auto-ack disabled
  nrf24l01_initialize_debug(false, PAYLOAD_SIZE, false);
  nrf24l01_aa_disable(NRF24L01_PIPE_NUM);

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
    /* ... */
    send_data(data)
    /* ... */
  }
}

// send data over nRF24L01+ with timeout, try NRF24L01_RETRY times.
// retval: OK - false, ERROR - true
bool send_data(uint8_t *data)
{
  uint32_t timeout;
  uint8_t retry = 0;
  bool state = true;

  for (retry = 0; retry < NRF24L01_RETRY; retry++)
  {
    timeout = HAL_GetTick() + NRF24Ll01_TIMEOUT;
    state = true;

    nrf24l01_write_tx_payload(data, PAYLOAD_SIZE, true); //transmit received characters over RF
    while (!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active()))
    { //wait until a packet has been transmitted
      if (HAL_GetTick() > timeout)
      { // check for timeout
        // log_error(E_OTHER, E_FL_SEND); // report error
        state = false;
        break;
      }
    }

    nrf24l01_irq_clear_all();    //clear all interrupts in the 24L01
    delay_us(NRF24L01_RX_DELAY); // delay because NRF receivers can handle only 1 payload at a time
  }

  return state;
}

/* ... */