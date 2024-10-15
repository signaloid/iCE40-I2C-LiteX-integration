/*
 *	Copyright (c) 2024, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */


#include <generated/csr.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "sb_i2c_regs.h"

const uint16_t 	prescaler		= CONFIG_CLOCK_FREQUENCY / kSB_I2C_CONFIG_TARGET_I2C_FREQUENCY / 4 - 1;
const uint32_t 	cycles_per_i2c_cycle	= prescaler * 4;

bool sbrwi_status  = false;
bool sbstbi_status = false;

/**
 * 	@brief Sets the System Bus Control register.
 *
 */
void
sb_i2c_set_sbctrl(void)
{
	sb_i2c_sbctrl_write(
		0
		| sbrwi_status << CSR_SB_I2C_SBCTRL_SBRWI_OFFSET
		| sbstbi_status << CSR_SB_I2C_SBCTRL_SBSTBI_OFFSET
	);
}

/**
 * 	@brief Sets the System Bus Read/Write signal on the System Bus Control register.
 *
 * 	@param value is the value to set (0 or 1).
 */
void
sb_i2c_sbctrl_sbrwi_write(uint8_t value)
{
	sbrwi_status = value;
	sb_i2c_set_sbctrl();
}

/**
 * 	@brief Sets the System Bus Strobe signal on the System Bus Control register.
 *
 * 	@param value is the value to set (0 or 1).
 */
void
sb_i2c_sbctrl_sbstbi_write(uint8_t value)
{
	sbstbi_status = value;
	sb_i2c_set_sbctrl();
}

/**
 * 	@brief Sets the System Bus as a read command.
 */
void
sb_i2c_set_read_cmd(void)
{
	sb_i2c_sbctrl_sbrwi_write(0);
}

/**
 * 	@brief Sets the System Bus as a write command.
 *
 */
void
sb_i2c_set_write_cmd(void)
{
	sb_i2c_sbctrl_sbrwi_write(1);
}

/**
 * 	@brief Sets the System Bus as not ready.
 *
 */
void
sb_i2c_set_not_ready_cmd(void)
{
	sb_i2c_sbctrl_sbstbi_write(0);
}

/**
 * 	@brief Sets the System Bus as ready.
 *
 */
void
sb_i2c_set_ready_cmd(void)
{
	sb_i2c_sbctrl_sbstbi_write(1);
}

/**
 * 	@brief Gets the System Bus Acknowledgement value.
 *
 * 	@return true when the System Bus Acknowledgement is set, so the command was received.
 * 	@return false when the System Bus Acknowledgement is not set, so the command was not yet received.
 */
bool
sb_i2c_get_sb_ack(void)
{
	return sb_i2c_sbstatus_read() & (1 << CSR_SB_I2C_SBSTATUS_SBACKO_OFFSET);
}

/**
 * 	@brief Sets the System Bus Register Address.
 *
 * 	@param address is the register address to set.
 */
void
sb_i2c_set_reg_addr(SB_I2C_REGS_t address)
{
	sb_i2c_sbadri_write(address);
}

/**
 * 	@brief Sets the System Bus Data Input.
 *
 * 	@param data is the data to set.
 */
void
sb_i2c_set_data(uint8_t data)
{
	sb_i2c_sbdati_write(data);
}

/**
 * 	@brief Gets the System Bus Data Output.
 *
 * 	@return uint8_t the data.
 */
uint8_t
sb_i2c_get_data(void)
{
	return sb_i2c_sbdato_read();
}

/**
 * 	@brief Sets a System Bus Register.
 *
 * 	@param address is the register address to set.
 * 	@param data is the data to set.
 */
void
sb_i2c_set_register(SB_I2C_REGS_t address, uint8_t data)
{
	/*
	 * 	Set the System Bus Register Address, and the data
	 */
	sb_i2c_set_reg_addr(address);
	sb_i2c_set_data(data);
	sb_i2c_set_write_cmd();

	/*
	 * 	Indicate that the System Bus has a ready command
	 */
	sb_i2c_set_ready_cmd();

	/*
	 * 	Wait for the System Bus Acknowledgement, so the command was received
	 */
	while (!sb_i2c_get_sb_ack());

	/*
	 * 	Reset System Bus signals
	 */
	sb_i2c_set_not_ready_cmd();
	sb_i2c_set_read_cmd();
}

/**
 * 	@brief Gets a System Bus Register.
 *
 * 	@param address is the register address to get.
 * 	@return uint8_t the data.
 */
uint8_t
sb_i2c_get_register(SB_I2C_REGS_t address)
{
	/*
	 * 	Set the System Bus Register Address, and indicate that the System Bus is a read command
	 */
	sb_i2c_set_reg_addr(address);
	sb_i2c_set_read_cmd();

	/*
	 * 	Indicate that the System Bus has a ready command
	 */
	sb_i2c_set_ready_cmd();

	/*
	 * 	Wait for the System Bus Acknowledgement, so the command was received
	 */
	while (!sb_i2c_get_sb_ack());

	/*
	 * 	Get the data
	 */
	uint8_t data = sb_i2c_get_data();

	/*
	 * 	Reset System Bus signals
	 */
	sb_i2c_set_not_ready_cmd();

	/*
	 * 	Return the data
	 */
	return data;
}

/**
 * 	@brief Tries to reset the I2C bus.
 */
void
i2c_reset(void)
{
	/*
	 * 	Try to release the I2C bus
	 */
	i2c_end();

	/*
	 * 	Send a dummy command to 0x00
	 */
	i2c_begin(0x00, false);
	i2c_end();

	/*
	 * 	Reinitialize the I2C Hard IP
	 */
	i2c_init();

	/*
	 * 	Release the I2C bus
	 */
	i2c_end();

	/*
	 * 	Send a dummy command to 0x00
	 */
	i2c_begin(0x00, false);
	i2c_end();
}

/**
 * 	@brief Waits for the I2C bus to be ready.
 */
void
sb_i2c_wait_for_trrdy(void)
{
	for (uint8_t timeout = 0; timeout < kSB_I2C_CONFIG_TRRDY_TIMEOUT; timeout++)
	{
		if ((sb_i2c_get_register(kSB_I2C_REGS_I2CSR) & kI2CSR_TRRDY_bm) != 0)
		{
			return;
		}
	}

	/*
	 * 	Waiting has timed out, so try to reset the I2C bus
	 */
	i2c_reset();
}

/**
 * 	@brief Waits for the I2C bus to be in the Master receiving / Slave transmitting mode.
 */
void
sb_i2c_wait_for_srw(void)
{
	for (uint8_t timeout = 0; timeout < kSB_I2C_CONFIG_SRW_TIMEOUT; timeout++)
	{
		if ((sb_i2c_get_register(kSB_I2C_REGS_I2CSR) & kI2CSR_SRW_bm) != 0)
		{
			return;
		}
	}

	/*
	 * 	Waiting has timed out, so try to reset the I2C bus
	 */
	i2c_reset();
}

/**
 * 	@brief Waits for the given number of I2C cycles.
 *
 * 	@param cycles is the number of cycles to wait for.
 */
void
i2c_wait_for_i2c_cycles(uint32_t cycles)
{
	while (cycles--)
	{
		for (uint32_t i = 0; i < cycles_per_i2c_cycle; i++)
		{
			asm("nop");
		}
	}
}

/**
 * 	@brief Sends a command to the I2C bus, and then clears the command to avoid sending it again.
 *
 * 	@param command is the command to send.
 */
void
sb_i2c_send_command(uint8_t command)
{
	/*
	 * 	Set the command
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CCMDR, command);

	/*
	 * 	Clear the command, to avoid sending it again
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CCMDR, 0x00);
}

void
i2c_init(void)
{
	/*
	 * 	Release the I2C bus
	 */
	i2c_end();

	/*
	 * 	Set the I2C control register
	 */
	sb_i2c_set_register(
		kSB_I2C_REGS_I2CCR1,
		0
		| kI2CCR1_SDA_DEL_SEL_300NS_gc
		| kI2CCR1_I2CEN_bm
	);

	/*
	 * 	Set Clock Prescaler
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CBRLSB, prescaler & 0xFF);
	sb_i2c_set_register(kSB_I2C_REGS_I2CBRMSB, prescaler >> 8);
}

void
i2c_begin(uint8_t address, bool is_read_cmd)
{
	/*
	 * 	Set the I2C slave address, and the read/write mode
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CTXDR, address << 1 | (is_read_cmd ? 0b1 : 0b0));

	/*
	 * 	Send the slave address and mode
	 */
	sb_i2c_send_command(
		0
		| kI2CCMDR_CKSDIS_bm
		| kI2CCMDR_WR_bm
		| kI2CCMDR_STA_bm
	);

	/*
	 * 	Check if it is a read command
	 */
	if (is_read_cmd)
	{
		/*
		 * 	Wait for the System Bus to be in the Master receiving / Slave transmitting mode
		 */
		sb_i2c_wait_for_srw();

		/*
		 * 	Set the I2C bus for slave writing
		 */
		sb_i2c_set_register(kSB_I2C_REGS_I2CCMDR,
			0
			| kI2CCMDR_CKSDIS_bm
			| kI2CCMDR_RD_bm
		);
	}
	else
	{
		/*
		 * 	Wait for the System Bus to be ready
		 */
		sb_i2c_wait_for_trrdy();
	}
}

void
i2c_write(uint8_t data)
{
	/*
	 * 	Set the I2C data
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CTXDR, data);

	/*
	 * 	Send the data
	 */
	sb_i2c_send_command(
		0
		| kI2CCMDR_CKSDIS_bm
		| kI2CCMDR_WR_bm
	);

	/*
	 * 	Wait for the System Bus to be ready
	 */
	sb_i2c_wait_for_trrdy();
}

uint8_t
i2c_read(bool is_last_read)
{
	/*
	 * 	Check if it is the last read
	 */
	if (!is_last_read)
	{
		/*
		 * 	Wait for the System Bus to be ready
		 */
		sb_i2c_wait_for_trrdy();

		/*
		 * 	Return the I2C data
		 */
		return sb_i2c_get_register(kSB_I2C_REGS_I2CRXDR);
	}

	/*
	 * 	Send a stop and ack I2C command
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CCMDR,
		0
		| kI2CCMDR_CKSDIS_bm
		| kI2CCMDR_ACK_bm
		| kI2CCMDR_RD_bm
		| kI2CCMDR_STO_bm
	);

	/*
	 * 	Wait for the System Bus to be ready
	 */
	sb_i2c_wait_for_trrdy();

	/*
	 * 	Return the I2C data
	 */
	return sb_i2c_get_register(kSB_I2C_REGS_I2CRXDR);
}

void
i2c_end(void)
{
	/*
	 * 	Send a stop I2C command
	 */
	sb_i2c_set_register(kSB_I2C_REGS_I2CCMDR,
		0
		| kI2CCMDR_CKSDIS_bm
		| kI2CCMDR_STO_bm
	);
}

bool
i2c_scan(uint8_t address)
{
	/*
	 * 	Send a dummy write command to the given address
	 */
	i2c_begin(address, false);

	/*
	 * 	Send a dummy zero
	 */
	i2c_write(0x00);

	/*
	 * 	Check if the slave acknowledged the command
	 */
	bool ack = !(sb_i2c_get_register(kSB_I2C_REGS_I2CSR) & kI2CSR_RARC_bm);

	/*
	 * 	Release the I2C bus
	 */
	i2c_end();

	/*
	 * 	Return true if the slave acknowledged the command
	 */
	return ack;
}
