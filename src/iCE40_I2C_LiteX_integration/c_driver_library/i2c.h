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


#ifndef __I2C_H
#define __I2C_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SB_I2C_CONFIG_enum
{
	/*
	 * 	Target I2C Bus frequency
	 */
	kSB_I2C_CONFIG_TARGET_I2C_FREQUENCY = 400000,

	/*
	 * 	Hard IP Timeouts
	 */
	kSB_I2C_CONFIG_TRRDY_TIMEOUT = INT8_MAX,
	kSB_I2C_CONFIG_SRW_TIMEOUT   = INT8_MAX,
} SB_I2C_CONFIG;

/**
 * 	@brief Initializes the I2C Hard IP.
 */
void i2c_init(void);

/**
 * 	@brief Initiates an I2C transaction, for slave with given address, as a read or write command.
 *
 * 	@param address is the slave address
 * 	@param is_read_cmd sets the read or write command
 */
void i2c_begin(uint8_t address, bool is_read_cmd);

/**
 * 	@brief Writes a byte to the I2C bus.
 *
 * 	@param data is the byte to write
 */
void i2c_write(uint8_t data);

/**
 * 	@brief Reads a byte from the I2C bus.
 *
 * 	@param is_last_read sets if this is the last read
 * 	@return uint8_t the read byte
 */
uint8_t i2c_read(bool is_last_read);

/**
 * 	@brief Ends an I2C transaction, and releases the I2C bus.
 */
void i2c_end(void);

/**
 * 	@brief Scans for a slave with given address.
 *
 * 	@param address is the slave address
 * 	@return true if the slave was found (ACK received)
 * 	@return false if the slave was not found (no ACK received)
 */
bool i2c_scan(uint8_t address);

#ifdef __cplusplus
}
#endif

#endif
