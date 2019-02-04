# nRF24L01+ libarary
Based on [this project, tutorials and files](http://blog.diyembedded.com/search/label/nrf24l01) \(diyembedded.com\).    
Modified by: [Damogran Labs: https://damogranlabs.com/](https://damogranlabs.com/2019/02/nrf24l01+-stm32-library).  
Date: 4.2.2019  

## About
This is a simple template for nRF24L01+ RF transceivers, which can be easily ported to any CPU. A quick example is available for STM32 HAL-based library.  
  
There are two pairs of files:
* *nRF24l01.c* and *nRF24l01.h*. This are non target-specific files
* *nRF24l01_user.c* and *nRF24l01_user.h*. This are user files, which must be modified in accordance to a chosen CPU and target pinout, spi driver, code, ...

## Usage
* Add/include all 4 files into your project
* Define GPIOs:
  * NRF24L01_CE - Output Push, HIGH by default
  * NRF24L01_CSN - Output Push, LOW by default
  * NRF24L01_IRQ - Input mode, Pull-up enabled
* SPI: (8-bits, MSB-first, clock polarity: LOW, clock phase: 1 edge, software toggling slave select pin)
  * NRF24L01_SCK - SPI1_SCK	
  * NRF42L01_MISO - SPI1_MISO	
  * NRF24L01_MOSI - SPI1_MOSI	
* Define 'protocol':
  * Address, size
  * Payload size (fixed)
  * Pipeline number
  * ...
* Open *nrf24l01_user.c/.h* and resolve *TODO* fields (delay, gpio, spi transfer, ...)
  
Note: the examples given are just a bare-bone init and RX/TX samples. There are many examples with all the explanation on original author web page.