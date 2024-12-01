/******************************************************************************
* File: nrf24l01.c
* Original source: S. Brennen Ball, 2006-2007 @ http://blog.diyembedded.com/
* Modified: Domen Jurkovic @ https://www.damogranlabs.com
*****************************************************************************/

#include "nrf24l01.h"

// low-level private functions for library use only
void nrf24l01_transmit(void);
uint8_t nrf24l01_execute_command(uint8_t instruction, uint8_t *data, unsigned int len, bool copydata);
void nrf24l01_spi_send_read(uint8_t *data, unsigned int len, bool copydata);

/******************************************************************************
 * nRF init function
 * Arguments (except opt_rx_standby_mode) fill the actual register they are named after. 
 *  Registers that do not need to be initialized are not included here.
 * 
 * The argument opt_rx_active_mode is only used if the user is initializing the
 *  24L01 as a receiver.  If the argument is false, the receiver will remain in
 *  standby mode and not monitor for packets.  If the argument is true, the CE
 *  pin will be set and the 24L01 will monitor for packets.  In TX mode, the value
 *  of this argument is insignificant.
 * 
 * If the user wants to leave any 1-byte register in its default state, simply put
 *  as that register's argument nrf24l01_<reg>_DEFAULT_VAL, where <reg> is the register name.
 * 
 * If the user wants to leave any of the 5-byte registers RX_ADDR_P0, RX_ADDR_P1, or 
 *  TX_ADDR in its default state, simply put NULL in the argument for that address value.
******************************************************************************/
void nrf24l01_initialize(uint8_t config,
                         uint8_t opt_rx_active_mode,
                         uint8_t en_aa,
                         uint8_t en_rxaddr,
                         uint8_t setup_aw,
                         uint8_t setup_retr,
                         uint8_t rf_ch,
                         uint8_t rf_setup,
                         uint8_t *rx_addr_p0,
                         uint8_t *rx_addr_p1,
                         uint8_t rx_addr_p2,
                         uint8_t rx_addr_p3,
                         uint8_t rx_addr_p4,
                         uint8_t rx_addr_p5,
                         uint8_t *tx_addr,
                         uint8_t rx_pw_p0,
                         uint8_t rx_pw_p1,
                         uint8_t rx_pw_p2,
                         uint8_t rx_pw_p3,
                         uint8_t rx_pw_p4,
                         uint8_t rx_pw_p5)
{
    uint8_t data[5];

    data[0] = en_aa;
    nrf24l01_write_register(NRF24L01_EN_AA, data, 1);

    data[0] = en_rxaddr;
    nrf24l01_write_register(NRF24L01_EN_RXADDR, data, 1);

    data[0] = setup_aw;
    nrf24l01_write_register(NRF24L01_SETUP_AW, data, 1);

    data[0] = setup_retr;
    nrf24l01_write_register(NRF24L01_SETUP_RETR, data, 1);

    data[0] = rf_ch;
    nrf24l01_write_register(NRF24L01_RF_CH, data, 1);

    data[0] = rf_setup;
    nrf24l01_write_register(NRF24L01_RF_SETUP, data, 1);

    if (rx_addr_p0 != NULL)
        nrf24l01_set_rx_addr(rx_addr_p0, 5, 0);
    else
    {
        data[0] = NRF24L01_RX_ADDR_P0_B0_DEFAULT_VAL;
        data[1] = NRF24L01_RX_ADDR_P0_B1_DEFAULT_VAL;
        data[2] = NRF24L01_RX_ADDR_P0_B2_DEFAULT_VAL;
        data[3] = NRF24L01_RX_ADDR_P0_B3_DEFAULT_VAL;
        data[4] = NRF24L01_RX_ADDR_P0_B4_DEFAULT_VAL;

        nrf24l01_set_rx_addr(data, 5, 0);
    }

    if (rx_addr_p1 != NULL)
    {
        nrf24l01_set_rx_addr(rx_addr_p1, 5, 1);
    }
    else
    {
        data[0] = NRF24L01_RX_ADDR_P1_B0_DEFAULT_VAL;
        data[1] = NRF24L01_RX_ADDR_P1_B1_DEFAULT_VAL;
        data[2] = NRF24L01_RX_ADDR_P1_B2_DEFAULT_VAL;
        data[3] = NRF24L01_RX_ADDR_P1_B3_DEFAULT_VAL;
        data[4] = NRF24L01_RX_ADDR_P1_B4_DEFAULT_VAL;

        nrf24l01_set_rx_addr(data, 5, 1);
    }

    data[0] = rx_addr_p2;
    nrf24l01_set_rx_addr(data, 1, 2);
    data[0] = rx_addr_p3;
    nrf24l01_set_rx_addr(data, 1, 3);
    data[0] = rx_addr_p4;
    nrf24l01_set_rx_addr(data, 1, 4);
    data[0] = rx_addr_p5;
    nrf24l01_set_rx_addr(data, 1, 5);
    if (tx_addr != NULL)
    {
        nrf24l01_set_tx_addr(tx_addr, 5);
    }
    else
    {
        data[0] = NRF24L01_TX_ADDR_B0_DEFAULT_VAL;
        data[1] = NRF24L01_TX_ADDR_B1_DEFAULT_VAL;
        data[2] = NRF24L01_TX_ADDR_B2_DEFAULT_VAL;
        data[3] = NRF24L01_TX_ADDR_B3_DEFAULT_VAL;
        data[4] = NRF24L01_TX_ADDR_B4_DEFAULT_VAL;

        nrf24l01_set_tx_addr(data, 5);
    }

    data[0] = rx_pw_p0;
    nrf24l01_write_register(NRF24L01_RX_PW_P0, data, 1);
    data[0] = rx_pw_p1;
    nrf24l01_write_register(NRF24L01_RX_PW_P1, data, 1);
    data[0] = rx_pw_p2;
    nrf24l01_write_register(NRF24L01_RX_PW_P2, data, 1);
    data[0] = rx_pw_p3;
    nrf24l01_write_register(NRF24L01_RX_PW_P3, data, 1);
    data[0] = rx_pw_p4;
    nrf24l01_write_register(NRF24L01_RX_PW_P4, data, 1);
    data[0] = rx_pw_p5;
    nrf24l01_write_register(NRF24L01_RX_PW_P5, data, 1);

    if ((config & NRF24L01_CONFIG_PWR_UP) != 0)
    {
        nrf24l01_power_up_param(opt_rx_active_mode, config);
    }
    else
    {
        nrf24l01_power_down_param(config);
    }
}

/******************************************************************************
 * Initializes the nRF24L01 to all default values except the PWR_UP and PRIM_RX bits.
 *  This function also disables the auto-ack feature on the chip (EN_AA register is 0).
 * 
 * If 'rx' is true, the device will be a receiver and transmitter otherwise.
 * 
 * 'payload_width' is the payload width for pipe 0.  All other pipes are left 
 *  in their default (disabled) state.
 * 
 * 'enable_auto_ack' controls the auto ack feature on pipe 0. If true, auto-ack will
 *  be enabled. If false, auto-ack is disabled.
******************************************************************************/
void nrf24l01_initialize_debug(bool rx, uint8_t p0_payload_width, bool enable_auto_ack)
{
    uint8_t config;
    uint8_t en_aa;

    config = NRF24L01_CONFIG_DEFAULT_VAL | NRF24L01_CONFIG_PWR_UP;

    if (enable_auto_ack != false)
    {
        en_aa = NRF24L01_EN_AA_ENAA_P0;
    }
    else
    {
        en_aa = NRF24L01_EN_AA_ENAA_NONE;
    }
    if (rx == true)
    {
        config = config | NRF24L01_CONFIG_PRIM_RX;
    }

    nrf24l01_initialize(config,
                        true,
                        en_aa,
                        NRF24L01_EN_RXADDR_DEFAULT_VAL,
                        NRF24L01_SETUP_AW_DEFAULT_VAL,
                        NRF24L01_SETUP_RETR_DEFAULT_VAL,
                        NRF24L01_RF_CH_DEFAULT_VAL,
                        NRF24L01_RF_SETUP_DEFAULT_VAL,
                        NULL,
                        NULL,
                        NRF24L01_RX_ADDR_P2_DEFAULT_VAL,
                        NRF24L01_RX_ADDR_P3_DEFAULT_VAL,
                        NRF24L01_RX_ADDR_P4_DEFAULT_VAL,
                        NRF24L01_RX_ADDR_P5_DEFAULT_VAL,
                        NULL,
                        p0_payload_width,
                        NRF24L01_RX_PW_P1_DEFAULT_VAL,
                        NRF24L01_RX_PW_P2_DEFAULT_VAL,
                        NRF24L01_RX_PW_P3_DEFAULT_VAL,
                        NRF24L01_RX_PW_P4_DEFAULT_VAL,
                        NRF24L01_RX_PW_P5_DEFAULT_VAL);
}

/******************************************************************************
 * Initializes only the CONFIG register and pipe 0's payload width.
 *  The primary purpose of this function is to allow users with microcontrollers with
 *  extremely small program memories to still be able to init their 24L01. This code
 *  should have a smaller footprint than the above init functions. When using this method, 
 *  the 24L01 MUST have its default configuration loaded in all registers to work. 
 *  It is recommended that the device be reset or have its power cycled immediately 
 *  before this code is run.
 * 
 * In normal circumstances, the user should use nrf24l01_initialize() rather than this
 *  function, since this function does not set all of the register values.
******************************************************************************/
void nrf24l01_initialize_debug_lite(bool rx, uint8_t p0_payload_width)
{
    uint8_t config;

    config = NRF24L01_CONFIG_DEFAULT_VAL;

    if (rx != false)
    {
        config |= NRF24L01_CONFIG_PRIM_RX;
    }

    nrf24l01_write_register(NRF24L01_RX_PW_P0, &p0_payload_width, 1);
    nrf24l01_power_up_param(true, config);
}

/******************************************************************************
 * Powers up the 24L01 with all necessary delays.
 * This function takes the existing contents of the CONFIG register and sets the PWR_UP
 *  the argument rx_active_mode is only used if the user is setting up the 
 *  24L01 as a receiver. 
 * 
 * If the 'rx_active_mode' is false, the receiver will remain in 
 *  standby mode and not monitor for packets. If the argument is true, the CE
 *  pin will be set and the 24L01 will monitor for packets.
 * 
 * In TX mode, the value of this argument is insignificant. 
 * 
 * If the read value of the CONFIG register already has the PWR_UP bit set,
 *  this function exits in order to not make an unecessary register write.
******************************************************************************/
void nrf24l01_power_up(bool rx_active_mode)
{
    uint8_t config;

    nrf24l01_read_register(NRF24L01_CONFIG, &config, 1);

    if ((config & NRF24L01_CONFIG_PWR_UP) != 0)
    {
        return;
    }

    config |= NRF24L01_CONFIG_PWR_UP;
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);
    delay_us(1500);

    if ((config & NRF24L01_CONFIG_PRIM_RX) == 0)
    {
        nrf24l01_clear_ce();
    }
    else
    {
        if (rx_active_mode != false)
        {
            nrf24l01_set_ce();
        }
        else
        {
            nrf24l01_clear_ce();
        }
    }
}
/******************************************************************************
 * Powers up the 24L01 with all necessary delays.
 * This function allows the user to set the contents of the CONFIG register, but the function
 * sets the PWR_UP bit in the CONFIG register, so the user does not need to.
 * 
 * 'rx_active_mode' is only used if the user is setting up the 24L01 as a receiver.
 *  If the argument is false, the receiver will remain in standby mode and not monitor for packets.
 *  If the argument is true, the CE pin will be set and the 24L01 will monitor for packets.  
 * 
 * In TX mode, the value of this argument is insignificant.
******************************************************************************/
void nrf24l01_power_up_param(bool rx_active_mode, uint8_t config)
{
    config |= NRF24L01_CONFIG_PWR_UP;

    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);
    delay_us(1500);

    if ((config & NRF24L01_CONFIG_PRIM_RX) == 0)
    {
        nrf24l01_clear_ce();
    }
    else
    {
        if (rx_active_mode != false)
        {
            nrf24l01_set_ce();
        }
        else
        {
            nrf24l01_clear_ce();
        }
    }
}

/******************************************************************************
 * Powers down the 24L01
 * This function takes the existing contents of the CONFIG register and simply
 *  clears the PWR_UP bit in the CONFIG register.
 * 
 * If the read value of the CONFIG register already has the PWR_UP bit cleared, this 
 *  function exits in order to not make an unecessary register write.
******************************************************************************/
void nrf24l01_power_down()
{
    uint8_t config;

    nrf24l01_read_register(NRF24L01_CONFIG, &config, 1);

    if ((config & NRF24L01_CONFIG_PWR_UP) == 0)
    {
        return;
    }

    config &= (~NRF24L01_CONFIG_PWR_UP);
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);

    nrf24l01_clear_ce();
}

/******************************************************************************
 * Powers down the 24L01
 * This function allows the user to set the contents of the CONFIG register, but the function
 *  clears the PWR_UP bit in the CONFIG register, so the user does not need to.
******************************************************************************/
void nrf24l01_power_down_param(uint8_t config)
{
    config &= (~NRF24L01_CONFIG_PWR_UP);
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);

    nrf24l01_clear_ce();
}

/******************************************************************************
 * Sets up the 24L01 as a receiver with all necessary delays.
 * This function takes the existing contents of the CONFIG register and sets the PRIM_RX 
 * bit in the CONFIG register. 
 * 
 * If the 'rx_active_mode' is false, the receiver will remain in standby mode and not monitor for packets.  
 * If the argument is true, the CE pin will be set and the 24L01 will monitor for packets.
 * 
 * If the read value of the CONFIG register already has the PRIM_RX bit set, this function 
 * exits in order to not make an unecessary register write.
******************************************************************************/
void nrf24l01_set_as_rx(bool rx_active_mode)
{
    uint8_t config;
    //uint8_t status = nrf24l01_read_register(0, &config, 1);

    if ((config & NRF24L01_CONFIG_PRIM_RX) != 0)
    {
        return;
    }

    config |= NRF24L01_CONFIG_PRIM_RX;
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);

    if (rx_active_mode != false)
    {
        nrf24l01_set_ce();
    }
    else
    {
        nrf24l01_clear_ce();
    }
}

/******************************************************************************
 * Sets up the 24L01 as a receiver with all necessary delays.
 * This function allows the user to set the contents of the CONFIG register, but the function
 *  sets the PRIM_RX bit in the CONFIG register, so the user does not need to.
 * 
 * If the 'rx_active_mode' is false, the receiver will remain in standby mode and not monitor for packets.  
 * If the argument is true, the CE pin will be set and the 24L01 will monitor for packets.
******************************************************************************/
void nrf24l01_set_as_rx_param(bool rx_active_mode, uint8_t config)
{
    config |= NRF24L01_CONFIG_PRIM_RX;
    if ((config & NRF24L01_CONFIG_PWR_UP) != 0)
    {
        nrf24l01_power_up_param(rx_active_mode, config);
    }
    else
    {
        nrf24l01_power_down_param(config);
    }
}

/******************************************************************************
 * Takes a 24L01 that is already in RX standby mode and puts it in active RX mode
******************************************************************************/
void nrf24l01_rx_standby_to_active()
{
    nrf24l01_set_ce();
}

/******************************************************************************
 * Takes a 24L01 that is already in active RX mode and puts it in RX standy mode
******************************************************************************/
void nrf24l01_rx_active_to_standby()
{
    nrf24l01_clear_ce();
}

/******************************************************************************
 * Sets up the 24L01 as a transmitter
 * This function takes the existing contents of the CONFIG register and simply
 *  clears the PRIM_RX bit in the CONFIG register.
 * 
 * If the read value of the CONFIG register already has the PRIM_RX bit cleared, this function
 *  exits in order to not make an unecessary register write.
******************************************************************************/
void nrf24l01_set_as_tx()
{
    uint8_t config;

    nrf24l01_read_register(NRF24L01_CONFIG, &config, 1);

    if ((config & NRF24L01_CONFIG_PRIM_RX) == 0)
    {
        return;
    }

    config &= (~NRF24L01_CONFIG_PRIM_RX);
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);

    nrf24l01_clear_ce();
}

/******************************************************************************
 * Sets up the 24L01 as a transmitter
 * This function allows the user to set the contents of the CONFIG register, but the function
 *  clears the PRIM_RX bit in the CONFIG register, so the user does not need to.
******************************************************************************/
void nrf24l01_set_as_tx_param(uint8_t config)
{
    config &= ~(NRF24L01_CONFIG_PRIM_RX);
    if ((config & NRF24L01_CONFIG_PWR_UP) != 0)
    {
        nrf24l01_power_up_param(false, config);
    }
    else
    {
        nrf24l01_power_down_param(config);
    }
}

/******************************************************************************
 * Executes the W_REGISTER SPI operation
 * 'regnumber' indicates the register number assigned by the nrf24l01 specification.
 *  For regnumber values, see section titled "register definitions" in nrf24l01.h.
 * 
 * 'data' should be of size 1 for all register writes except for RX_ADDR_P0, RX_ADDR_P1, and TX_ADDR. 
 *  The size of data should be set according to the user-specified size of the address
 *  length for the register the address is being sent to.
 * 
 * 'len' is always the size of uint8_t * data. 
 *  For example, if data is declared as data[6], len should equal 6.
 * 
 * Returns the value of the STATUS register.
******************************************************************************/
uint8_t nrf24l01_write_register(uint8_t regnumber, uint8_t *data, unsigned int len)
{
    return nrf24l01_execute_command(NRF24L01_W_REGISTER | (regnumber & NRF24L01_W_REGISTER_DATA), data, len, false);
}

/******************************************************************************
 * Executes the R_REGISTER SPI operation
 * 'regnumber' indicates the register number assigned by the nrf24l01 specification.
 *  For regnumber values, see section titled "register definitions" in nrf24l01.h.
 * 
 * 'data' should be of size 1 for all register writes except for RX_ADDR_P0, RX_ADDR_P1 and TX_ADDR.
 *  The size of data should be set according to the user-specified size of the address 
 *  length for the register the address is being read from.
 * 
 * 'len' is always the size of uint8_t * data. 
 *  For example, if data is declared as data[6], len = 6.
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_read_register(uint8_t regnumber, uint8_t *data, unsigned int len)
{
    return nrf24l01_execute_command(regnumber & NRF24L01_R_REGISTER_DATA, data, len, true);
}

/******************************************************************************
 * Executes the W_TX_PAYLOAD operation
 * 'data' is the actual payload to be sent to the nrf24l01.
 * 
 * 'len' is the length of the payload being sent (this should be sized
 *  according to the payload length specified by the receiving nrf24l01).
 * 
 * If 'transmit' is true, the nrf24l01 immediately transmits the data in the payload.
 * If false, the user must use the nrf24l01_transmit() function to send the payload.
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_write_tx_payload(uint8_t *data, unsigned int len, bool transmit)
{
    uint8_t status;

    status = nrf24l01_execute_command(NRF24L01_W_TX_PAYLOAD, data, len, false);
    if (transmit == true)
    {
        nrf24l01_transmit();
    }

    return status;
}

/******************************************************************************
 * Executes the R_RX_PAYLOAD instruction
 * 'data' is the actual payload that has been received by the nrf24l01.
 *  The user must size data according to the payload width specified to the nrf24l01.
 *  This variable is filled by this function, so individual byte values need not be initialized by the user.
 * 
 * 'len' is the length of the payload being clocked out of the nrf24l01 (this should be
 *  sized according to the payload length specified to the nrf24l01).
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_read_rx_payload(uint8_t *data, unsigned int len)
{
    uint8_t status;

    nrf24l01_clear_ce();
    status = nrf24l01_execute_command(NRF24L01_R_RX_PAYLOAD, data, len, true);
    nrf24l01_set_ce();

    return status;
}

/******************************************************************************
 * Executes the FLUSH_TX SPI operation
 * This funciton empties the contents of the TX FIFO
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_flush_tx()
{
    return nrf24l01_execute_command(NRF24L01_FLUSH_TX, NULL, 0, true);
}

/******************************************************************************
 * Executes the FLUSH_RX SPI operation
 * This funciton empties the contents of the RX FIFO
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_flush_rx()
{
    return nrf24l01_execute_command(NRF24L01_FLUSH_RX, NULL, 0, true);
}

/******************************************************************************
 * Executes the REUSE_TX_PL SPI operation
 * This funciton allows the user to constantly send a packet repeatedly when issued.
 * 
 * Returns the value of the STATUS register
*******************************************************************************/
uint8_t nrf24l01_reuse_tx_pl()
{
    return nrf24l01_execute_command(NRF24L01_REUSE_TX_PL, NULL, 0, true);
}

/******************************************************************************
 * Executes the FLUSH_TX SPI operation
 * This funciton does nothing
 * 
 * Returns the value of the STATUS register
******************************************************************************/
uint8_t nrf24l01_nop()
{
    return nrf24l01_execute_command(NRF24L01_NOP, NULL, 0, true);
}

/******************************************************************************
 * Transmits the current tx payload
******************************************************************************/
void nrf24l01_transmit()
{
    nrf24l01_set_ce();
    delay_us(10);
    nrf24l01_clear_ce();
}

/******************************************************************************
 * Sets the TX address in the TX_ADDR register
 * Address is the actual address to be used.  It should be sized according to the 
 *  'tx_addr' length specified to the nrf24l01.
 * 
 * 'len' is the length of the address. Its value should be specified according to the 
 *  'tx_addr' length specified to the nrf24l01.
******************************************************************************/
void nrf24l01_set_tx_addr(uint8_t *address, unsigned int len)
{
    nrf24l01_write_register(NRF24L01_TX_ADDR, address, len);
}

/******************************************************************************
 * Sets the RX address in the RX_ADDR register that is offset by 'rxpipenum'
 * 'address' is the actual address to be used. It should be sized according to 
 *  the 'rx_addr' length that is being filled.
 * 
 * 'len' is the length of the address. Its value should be specified according to 
 *  the 'rx_addr' length specified to the nrf24l01.
 * 
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *   If an invalid address (greater than five) is supplied, the function does nothing.
******************************************************************************/
void nrf24l01_set_rx_addr(uint8_t *address, unsigned int len, uint8_t rxpipenum)
{
    if (rxpipenum > 5)
    {
        return;
    }

    nrf24l01_write_register(NRF24L01_RX_ADDR_P0 + rxpipenum, address, len);
}

/******************************************************************************
 * Sets the RX payload width on the pipe offset by 'rxpipenum'
 * 'payloadwidth' is the length of the payload for the pipe referenced in 'rxpipenum'.
 *  It must be less than or equal to 32.  If an invalid payload width is specified, 
 *  the function does nothing.
 * 
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *  If an invalid address (greater than five) is supplied, the function does nothing.
******************************************************************************/
void nrf24l01_set_rx_pw(uint8_t payloadwidth, uint8_t rxpipenum)
{
    if ((rxpipenum > 5) || (payloadwidth > 32))
    {
        return;
    }

    nrf24l01_write_register(NRF24L01_RX_PW_P0 + rxpipenum, &payloadwidth, 1);
}

/******************************************************************************
 * Gets the RX payload width on the pipe offset by 'rxpipenum'
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *  If an invalid address (greater than five) is supplied, the function does nothing.
******************************************************************************/
uint8_t nrf24l01_get_rx_pw(uint8_t rxpipenum)
{
    uint8_t data;
    if (rxpipenum > 5)
    {
        return 0;
    }

    nrf24l01_read_register(NRF24L01_RX_PW_P0 + rxpipenum, &data, 1);
    return data;
}

/******************************************************************************
 * Returns the value of the CONFIG register
******************************************************************************/
uint8_t nrf24l01_get_config()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_CONFIG, &data, 1);

    return data;
}

/******************************************************************************
 * Sets the value of the CONFIG register
******************************************************************************/
void nrf24l01_set_config(uint8_t config)
{
    nrf24l01_write_register(NRF24L01_CONFIG, &config, 1);
}

/******************************************************************************
 * Returns the current RF channel in RF_CH register
******************************************************************************/
uint8_t nrf24l01_get_rf_ch()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_RF_CH, &data, 1);

    return data;
}

/******************************************************************************
 * uint8_t channel is the channel to be changed to.
******************************************************************************/
void nrf24l01_set_rf_ch(uint8_t channel)
{
    uint8_t data;

    data = channel & ~NRF24L01_RF_CH_RESERVED;

    nrf24l01_write_register(NRF24L01_RF_CH, &data, 1);
}

/******************************************************************************
 * Returns the value of the OBSERVE_TX register
******************************************************************************/
uint8_t nrf24l01_get_observe_tx()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_OBSERVE_TX, &data, 1);

    return data;
}

/******************************************************************************
 * Returns the current PLOS_CNT value in OBSERVE_TX register
******************************************************************************/
uint8_t nrf24l01_get_plos_cnt()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_OBSERVE_TX, &data, 1);

    return ((data & NRF24L01_OBSERVE_TX_PLOS_CNT) >> 4);
}

/******************************************************************************
 * Clears the PLOS_CNT field of the OBSERVE_TX register
 * This function makes a read of the current value of RF_CH and
 *  simply writes it back to the register, clearing PLOS_CNT
******************************************************************************/
void nrf24l01_clear_plos_cnt()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_RF_CH, &data, 1);
    nrf24l01_write_register(NRF24L01_RF_CH, &data, 1);
}

/******************************************************************************
 * Clears the PLOS_CNT field of the OBSERVE_TX register
 * This function allows the user to set the RF_CH register by using the argument 
 *  in the function during the PLOS_CNT clearing process
******************************************************************************/
void nrf24l01_clear_plos_cnt_param(uint8_t rf_ch)
{
    nrf24l01_write_register(NRF24L01_RF_CH, &rf_ch, 1);
}

/******************************************************************************
 * Returns the current ARC_CNT value in OBSERVE_TX register
******************************************************************************/
uint8_t nrf24l01_get_arc_cnt()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_OBSERVE_TX, &data, 1);

    return (data & NRF24L01_OBSERVE_TX_ARC_CNT);
}

/******************************************************************************
 * Returns true if auto-ack is enabled on the pipe that is offset by rxpipenum
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.  
 *   If an invalid address (greater than five) is supplied, the function returns false.
******************************************************************************/
bool nrf24l01_aa_enabled(uint8_t rxpipenum)
{
    uint8_t data;

    if (rxpipenum > 5)
    {
        return false;
    }

    nrf24l01_read_register(NRF24L01_EN_AA, &data, 1);

    return (data & (0x01 << rxpipenum));
}

/******************************************************************************
 * Enables auto-ack is enabled on the pipe that is offset by rxpipenum
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *   If an invalid address (greater than five) is supplied, the function returns false.
******************************************************************************/
void nrf24l01_aa_enable(uint8_t rxpipenum)
{
    uint8_t data;

    if (rxpipenum > 5)
    {
        return;
    }

    nrf24l01_read_register(NRF24L01_EN_AA, &data, 1);
    if ((data & (0x01 << rxpipenum)) != 0)
    {
        return;
    }

    data |= 0x01 << rxpipenum;
    nrf24l01_write_register(NRF24L01_EN_AA, &data, 1);
}

/******************************************************************************
 * Disables auto-ack is enabled on the pipe that is offset by rxpipenum
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *   If an invalid address (greater than five) is supplied, the function returns false.
******************************************************************************/
void nrf24l01_aa_disable(uint8_t rxpipenum)
{
    uint8_t data;

    if (rxpipenum > 5)
    {
        return;
    }

    nrf24l01_read_register(NRF24L01_EN_AA, &data, 1);
    if ((data & (0x01 << rxpipenum)) == 0)
        return;

    data &= ~(0x01 << rxpipenum);
    nrf24l01_write_register(NRF24L01_EN_AA, &data, 1);
}

/******************************************************************************
 * Returns true if the pipe is enabled that is offset by rxpipenum
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *  If an invalid address (greater than five) is supplied, the function returns false.
******************************************************************************/
bool nrf24l01_rx_pipe_enabled(uint8_t rxpipenum)
{
    uint8_t data;

    if ((rxpipenum > 5))
    {
        return false;
    }

    nrf24l01_read_register(NRF24L01_EN_RXADDR, &data, 1);
    return (data & (0x01 << rxpipenum));
}

/******************************************************************************
 * Enables the pipe that is offset by 'rxpipenum'
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *  If an invalid address (greater than five) is supplied, the function does nothing.
******************************************************************************/
void nrf24l01_rx_pipe_enable(uint8_t rxpipenum)
{
    uint8_t data;

    if (rxpipenum > 5)
    {
        return;
    }

    nrf24l01_read_register(NRF24L01_EN_RXADDR, &data, 1);
    if ((data & (0x01 << rxpipenum)) != 0)
    {
        return;
    }

    data |= 0x01 << rxpipenum;
    nrf24l01_write_register(NRF24L01_EN_RXADDR, &data, 1);
}

/******************************************************************************
 * Disables the pipe that is offset by 'rxpipenum'
 * 'rxpipenum' is the pipe number (zero to five) whose address is being specified.
 *  If an invalid address (greater than five) is supplied, the function does nothing.
******************************************************************************/
void nrf24l01_rx_pipe_disable(uint8_t rxpipenum)
{
    uint8_t data;

    if (rxpipenum > 5)
    {
        return;
    }

    nrf24l01_read_register(NRF24L01_EN_RXADDR, &data, 1);
    if ((data & (0x01 << rxpipenum)) == 0)
    {
        return;
    }

    data &= ~(0x01 << rxpipenum);
    nrf24l01_write_register(NRF24L01_EN_RXADDR, &data, 1);
}

/******************************************************************************
 * Returns the status of the CD register (true if carrier detect [CD] is active, false if not).
******************************************************************************/
bool nrf24l01_cd_active()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_CD, &data, 1);

    return data;
}

/******************************************************************************
 * Returns the value of the FIFO_STATUS register
******************************************************************************/
uint8_t nrf24l01_get_fifo_status()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return data;
}

/******************************************************************************
 * Return the value of the status register
******************************************************************************/
uint8_t nrf24l01_get_status()
{
    return nrf24l01_nop();
}

/******************************************************************************
 * Returns true if TX_REUSE bit in FIFO_STATUS register is set, false otherwise
******************************************************************************/
bool nrf24l01_fifo_tx_reuse()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return (bool)(data & NRF24L01_FIFO_STATUS_TX_REUSE);
}

/******************************************************************************
 * Returns true if TX_FULL bit in FIFO_STATUS register is set, false otherwise
******************************************************************************/
bool nrf24l01_fifo_tx_full()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return (bool)(data & NRF24L01_FIFO_STATUS_TX_FULL);
}

/******************************************************************************
 * Returns true if TX_EMPTY bit in FIFO_STATUS register is set, false otherwise
******************************************************************************/
bool nrf24l01_fifo_tx_empty()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return (bool)(data & NRF24L01_FIFO_STATUS_TX_EMPTY);
}

/******************************************************************************
 * Returns true if RX_FULL bit in FIFO_STATUS register is set, false otherwise
******************************************************************************/
bool nrf24l01_fifo_rx_full()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return (bool)(data & NRF24L01_FIFO_STATUS_RX_FULL);
}

/******************************************************************************
 * Returns true if RX_EMPTYE bit in FIFO_STATUS register is set, false otherwise
******************************************************************************/
bool nrf24l01_fifo_rx_empty()
{
    uint8_t data;

    nrf24l01_read_register(NRF24L01_FIFO_STATUS, &data, 1);

    return (bool)(data & NRF24L01_FIFO_STATUS_RX_EMPTY);
}

/******************************************************************************
 * Returns true if RX_DR interrupt is active, false otherwise
******************************************************************************/
bool nrf24l01_irq_rx_dr_active()
{
    return (nrf24l01_get_status() & NRF24L01_STATUS_RX_DR);
}

/******************************************************************************
 * Returns true if TX_DS interrupt is active, false otherwise
******************************************************************************/
bool nrf24l01_irq_tx_ds_active()
{
    return (nrf24l01_get_status() & NRF24L01_STATUS_TX_DS);
}

/******************************************************************************
 * Returns true if MAX_RT interrupt is active, false otherwise
******************************************************************************/
bool nrf24l01_irq_max_rt_active()
{
    return (nrf24l01_get_status() & NRF24L01_STATUS_MAX_RT);
}

/******************************************************************************
 * Clear all interrupts in the status register
******************************************************************************/
void nrf24l01_irq_clear_all()
{
    uint8_t data = NRF24L01_STATUS_RX_DR | NRF24L01_STATUS_TX_DS | NRF24L01_STATUS_MAX_RT;

    nrf24l01_write_register(NRF24L01_STATUS, &data, 1);
}

/******************************************************************************
 * Clears only the RX_DR interrupt
******************************************************************************/
void nrf24l01_irq_clear_rx_dr()
{
    uint8_t data = NRF24L01_STATUS_RX_DR;

    nrf24l01_write_register(NRF24L01_STATUS, &data, 1);
}

/******************************************************************************
 * Clears only the TX_DS interrupt
******************************************************************************/
void nrf24l01_irq_clear_tx_ds()
{
    uint8_t data = NRF24L01_STATUS_TX_DS;

    nrf24l01_write_register(NRF24L01_STATUS, &data, 1);
}

/******************************************************************************
 * Clears only the MAX_RT interrupt
******************************************************************************/
void nrf24l01_irq_clear_max_rt()
{
    uint8_t data = NRF24L01_STATUS_MAX_RT;

    nrf24l01_write_register(NRF24L01_STATUS, &data, 1);
}

/******************************************************************************
 * Returns the current pipe in the 24L01's STATUS register
******************************************************************************/
uint8_t nrf24l01_get_rx_pipe()
{
    return nrf24l01_get_rx_pipe_from_status(nrf24l01_get_status());
}

uint8_t nrf24l01_get_rx_pipe_from_status(uint8_t status)
{
    return ((status & 0xE) >> 1);
}

/******************************************************************************
 * Flush both fifos and clear interrupts
******************************************************************************/
void nrf24l01_clear_flush()
{
    nrf24l01_flush_rx();
    nrf24l01_flush_tx();
    nrf24l01_irq_clear_all();
}

/******************************************************************************
 * Get all register values.
 * 'data' must be at least 35 bytes long
******************************************************************************/
void nrf24l01_get_all_registers(uint8_t *data)
{
    unsigned int outer;
    unsigned int inner;
    unsigned int dataloc = 0;
    uint8_t buffer[5];

    for (outer = 0; outer <= 0x17; outer++)
    {
        nrf24l01_read_register(outer, buffer, 5);

        for (inner = 0; inner < 5; inner++)
        {
            if (inner >= 1 && (outer != 0x0A && outer != 0x0B && outer != 0x10))
            {
                break;
            }
            data[dataloc] = buffer[inner];
            dataloc++;
        }
    }
}

/******************************************************************************
 * Low-level spi send function for library use
 * User should not call this function directly, but rather use one of the 8 SPI data instructions
******************************************************************************/
uint8_t nrf24l01_execute_command(uint8_t instruction, uint8_t *data, unsigned int len, bool copydata)
{
    uint8_t status;

    nrf24l01_clear_csn();

    status = instruction;
    nrf24l01_spi_send_read(&status, 1, true);
    nrf24l01_spi_send_read(data, len, copydata);

    nrf24l01_set_csn();

    return status;
}

/******************************************************************************
 * Low-level spi send function for library use
 * User should not call this function directly, but rather use one of the 8 SPI data instructions
******************************************************************************/
void nrf24l01_spi_send_read(uint8_t *data, unsigned int len, bool copydata)
{
    unsigned int count;
    uint8_t tempbyte;

    for (count = 0; count < len; count++)
    {
        if (copydata != false)
        {
            data[count] = spi_send_read_byte(data[count]);
        }
        else
        {
            tempbyte = data[count];
            spi_send_read_byte(tempbyte);
        }
    }
}