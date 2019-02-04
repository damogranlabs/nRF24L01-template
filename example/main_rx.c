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
  uint8_t rxData[PAYLOAD_SIZE]; // data received through NRF24L01

  /*
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  ...
  */

  // NRF24L01+ init
  // delay_us_init();
  nrf24l01_power_up(true);
  //initialize the 24L01 to the debug configuration as TX, 1 data byte, and auto-ack disabled
  nrf24l01_initialize_debug(true, PAYLOAD_SIZE, false);
  nrf24l01_aa_disable(NRF24L01_PIPE_NUM);

  f_addr[0] = f_addr[1] = f_addr[2] = f_addr[3] = f_addr[4] = NRF24L01_TX_ADDR_B0_DEFAULT_VAL;
  nrf24l01_set_rx_addr(f_addr, NRF24L01_ADDRESS_SIZE, NRF24L01_PIPE_NUM);

  nrf24l01_clear_flush();

  while (1)
  {
    /* ... */

    //wait until a packet has been received
    if ((nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()))
    {
      nrf24l01_read_rx_payload(rxData, PAYLOAD_SIZE); //read the packet into data
      nrf24l01_irq_clear_all();                       //clear all interrupts in the 24L01

      // handleReceivedData(rxData);
    }

    // HAL_Delay(100);

    /* ... */
  }
}

/* ... */