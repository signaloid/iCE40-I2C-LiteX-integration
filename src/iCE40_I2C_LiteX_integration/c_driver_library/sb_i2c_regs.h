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


#ifndef __SB_I2C_REGS_H__
#define __SB_I2C_REGS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	This is a register map for the SB_I2C_1 IP block as defined in the
 *	Lattice "Advanced iCE40 I2C and SPI Hardened IP User Guide" [FPGA-TN-02011-1.7].
 *
 * 	Most if not all documentation comments are copied from that datasheet.
 */


typedef enum
{
	kSB_I2C_REGS_I2CCR1   = (0x8), /* 0b1000 | Control [Read/Write] */
	kSB_I2C_REGS_I2CCMDR  = (0x9), /* 0b1001 | Command [Read/Write] */
	kSB_I2C_REGS_I2CBRLSB = (0xa), /* 0b1010 | Clock Prescale register, LSB [Read/Write] */
	kSB_I2C_REGS_I2CBRMSB = (0xb), /* 0b1011 | Clock Prescale register, MSB [Read/Write] */
	kSB_I2C_REGS_I2CSR    = (0xc), /* 0b1100 | Status [Read] */
	kSB_I2C_REGS_I2CTXDR  = (0xd), /* 0b1101 | Transmit Data [Write] */
	kSB_I2C_REGS_I2CRXDR  = (0xe), /* 0b1110 | Receive Data [Read] */
	kSB_I2C_REGS_I2CGCDR  = (0xf), /* 0b1111 | General Call Information [Read] */
	kSB_I2C_REGS_I2CSADDR = (0x3), /* 0b0011 | Slave Address MSB [Read/Write] */
	kSB_I2C_REGS_I2CIRQEN = (0x7), /* 0b0111 | Interrupt Enable [Read/Write] */
	kSB_I2C_REGS_I2CIRQ   = (0x6), /* 0b0110 | Interrupt Status [Read/Write]* */
} SB_I2C_REGS_t;

/*
 * 	*_bp: bit position
 * 	*_bm: bit mask
 * 	*_gp: group position
 * 	*_gm: group mask
 * 	*_gc: group configuration
 */

/*
 * 	I2CCR1 - I2C Control Register 1
 *
 *	!Note: A write to this register will cause the I2C core to reset.
 */
typedef enum I2CCR1_enum
{
	/*
	 * 	SDA Output Delay (Tdel) Selection
	 */
	kI2CCR1_SDA_DEL_SEL_gp = 2,
	kI2CCR1_SDA_DEL_SEL_bm = 0b11,
	/* Min: 300ns | Max: 300ns + 2000/[wb_clk_i freq. in MHz] */
	kI2CCR1_SDA_DEL_SEL_300NS_gc = (0b00 << kI2CCR1_SDA_DEL_SEL_gp),
	/* Min: 150ns | Max: 150ns + 2000/[wb_clk_i freq. in MHz] */
	kI2CCR1_SDA_DEL_SEL_150NS_gc = (0b01 << kI2CCR1_SDA_DEL_SEL_gp),
	/* Min:  75ns | Max:  75ns + 2000/[wb_clk_i freq. in MHz] */
	kI2CCR1_SDA_DEL_SEL_75NS_gc = (0b10 << kI2CCR1_SDA_DEL_SEL_gp),
	/* Min:   0ns | Max:   0ns + 2000/[wb_clk_i freq. in MHz] */
	kI2CCR1_SDA_DEL_SEL_0NS_gc = (0b11 << kI2CCR1_SDA_DEL_SEL_gp),

	/*
	 * 	Wake-up from Standby/Sleep (by Slave Address matching) Enable Bit
	 * 	When this bit is enabled, the I2C core can send a wake-up signal to wake the device up from
	 * 	standby/sleep. The wake-up function is activated when the Slave Address is matched during
	 * 	standby/sleep mode.
	 */
	kI2CCR1_WKUPEN_bp = 5,
	kI2CCR1_WKUPEN_bm = (0b1 << kI2CCR1_WKUPEN_bp),

	/*
	 * 	Enable bit for General Call Response – Enables the general call response in slave mode.
	 * 	* 0: Disable
	 * 	* 1: Enable
	 * 	The General Call address is defined as 0000000 and works with either 7-bit or 10-bit addressing.
	 */
	kI2CCR1_GCEN_bp = 6,
	kI2CCR1_GCEN_bm = (0b1 << kI2CCR1_GCEN_bp),

	/*
	 *	I2C System Enable Bit – This bit enables the I2C core functions.
	 *	If I2CEN is cleared, the I2C core is disabled and forced into idle state.
	 */
	kI2CCR1_I2CEN_bp = 7,
	kI2CCR1_I2CEN_bm = (0b1 << kI2CCR1_I2CEN_bp),
} I2CCR1_t;

/*
 * 	I2CCMDR - I2C Command Register
 */
typedef enum I2CCMDR_enum
{

	/*
	 * 	Read Command with Buffer Disable – Read from Slave in master mode with the double buffering
	 * 	disabled for easier control over single byte data communication scenario.
	 * 	* 0: Read with buffer enabled as default
	 * 	* 1: Read with buffer disabled
	 */
	kI2CCMDR_RBUFDIS_bp = 1,
	kI2CCMDR_RBUFDIS_bm = (0b1 << kI2CCMDR_RBUFDIS_bp),

	/*
	 * 	Clock Stretching Disable – Disables the clock stretching if desired by the user for both master and
	 * 	slave mode. Then overflow error flag must be monitored.
	 * 	* 0: Enable Clock Stretching
	 * 	* 1: Disable Clock Stretching
	 */
	kI2CCMDR_CKSDIS_bp = 2,
	kI2CCMDR_CKSDIS_bm = (0b1 << kI2CCMDR_CKSDIS_bp),

	/*
	 * 	ACK/NACK Control – This bit controls the ACK/NACK response to the master.
	 * 	* 0: Send ACK
	 * 	* 1: Send NACK
	 */
	kI2CCMDR_ACK_bp = 3,
	kI2CCMDR_ACK_bm = (0b1 << kI2CCMDR_ACK_bp),

	/*
	 * 	Write Command – Write to Slave in master mode.
	 */
	kI2CCMDR_WR_bp = 4,
	kI2CCMDR_WR_bm = (0b1 << kI2CCMDR_WR_bp),

	/*
	 *	Indicate Read from slave (Master operation)
	 */
	kI2CCMDR_RD_bp = 5,
	kI2CCMDR_RD_bm = (0b1 << kI2CCMDR_RD_bp),


	/*
	 * 	Generate STOP condition (Master operation)
	 */
	kI2CCMDR_STO_bp = 6,
	kI2CCMDR_STO_bm = (0b1 << kI2CCMDR_STO_bp),

	/*
	 *	Generate START (or Repeated START) condition (Master operation)
	 */
	kI2CCMDR_STA_bp = 7,
	kI2CCMDR_STA_bm = (0b1 << kI2CCMDR_STA_bp),

} I2CCMDR_t;

/*
 *	I2C_PRESCALE[9:0]
 *
 * 	I2C Clock Pre-scale value. A write operation to I2CBRMSB[1:0] will cause an I2C core reset. The System Bus clock
 * 	frequency is divided by (I2C_PRESCALE*4) to produce the Master I2C clock frequency supported by the I2C bus
 *	(50 kHz, 100 kHz, 400 kHz).
 */

/*
 *	I2CBRLSB - I2C Clock Prescale Register, LSB
 */
typedef enum I2CBRLSB_enum
{
	kI2CBRLSB_bp = 0,
	kI2CBRLSB_bm = 0b11111111,
} I2CBRLSB_t;

/*
 *	I2CBRMSB - I2C Clock Prescale Register, MSB
 */
typedef enum I2CBRMSB_enum
{
	kI2CBRMSB_bp = 0,
	kI2CBRMSB_bm = 0b11,
} I2CBRMSB_t;

/*
 *	I2CSR - I2C Status Register
 */
typedef enum I2CSR_enum
{
	/*
	 * 	Hardware General Call Received – This flag indicate that a hardware general call is received from the
	 *	slave port. It will cause an interrupt to System Bus Host if SCI set up allowed.
	 *	* 0: NO Hardware General Call Received in Slave Mode
	 *	* 1: Hardware General Call Received in Slave Mode
	 */
	kI2CSR_HGC_bp = 0,
	kI2CSR_HGC_bm = (0b1 << kI2CSR_HGC_bp),

	/*
	 *	Transmitter/Receiver Overrun or NACK Received Bit – This flag indicate that a Transmit or Receive
	 *	Overrun Errors happened depend on the mode (master or slave) and SRW bit, or a no-acknowledges
	 *	response is received after transmitting a byte. If RARC bit is high, it is a NACK bit, otherwise, it is
	 *	overrun bit. It will cause an interrupt to System Bus Host if SCI set up allowed.
	 *	* 0: Transmitter or Receiver Normal or Acknowledge Received for Transmitting
	 *	* 1: Transmitter or Receiver Overrun or No-Acknowledge Received for Transmitting
	 */
	kI2CSR_TROE_bp = 1,
	kI2CSR_TROE_bm = (0b1 << kI2CSR_TROE_bp),

	/*
	 *	Transmitter or Receiver Ready Bit – This flag indicate that a Transmit Register ready to receive data or
	 *	Receiver Register if ready for read depend on the mode (master or slave) and SRW bit. It will cause an
	 *	interrupt to System Bus Host if SCI set up allowed.
	 *	* 0: Transmitter or Receiver is not ready
	 *	* 1: Transmitter or Receiver is ready
	 */
	kI2CSR_TRRDY_bp = 2,
	kI2CSR_TRRDY_bm = (0b1 << kI2CSR_TRRDY_bp),

	/*
	 *	Arbitration Lost – This bit will go high if master has lost its arbitration in Master mode, It will
	 *	cause an interrupt to System Bus Host if SCI set up allowed.
	 *	* 0: Normal
	 *	* 1: Arbitration Lost
	 */
	kI2CSR_ARBL_bp = 3,
	kI2CSR_ARBL_bm = (0b1 << kI2CSR_ARBL_bp),

	/*
	 *	Slave RW
	 *	* 0: Master transmitting / Slave receiving
	 *	* 1: Master receiving / Slave transmitting
	 */
	kI2CSR_SRW_bp = 4,
	kI2CSR_SRW_bm = (0b1 << kI2CSR_SRW_bp),

	/*
	 *	Received Acknowledge – This flag represents acknowledge response from the addressed slave during
	 *	master write or from receiving master during master read.
	 *	* 0: No acknowledge received
	 *	* 1: Acknowledge received
	 */
	kI2CSR_RARC_bp = 5,
	kI2CSR_RARC_bm = (0b1 << kI2CSR_RARC_bp),

	/*
	 *	Bus Busy – This bit indicates the bus is involved in transaction. This will be set at start condition
	 *	and cleared at stop. Therefore, only when this bit is high, should all other status bits be treated as
	 *	valid indicators for a valid transfer.
	 */
	kI2CSR_BUSY_bp = 6,
	kI2CSR_BUSY_bm = (0b1 << kI2CSR_BUSY_bp),

	/*
	 *	Transmitting In Progress – This bit indicates that current data byte is being transferred for both
	 *	master and slave mode. Note that the TIP flag will suffer half SCL cycle latency right after the start
	 *	condition because of the signal synchronization. Note also that this bit could be high after
	 *	configuration wake-up and before the first valid I2C transfer start (when BUSY is low), and it is not
	 *	indicating byte in transfer, but an invalid indicator.
	 *	* 0: Byte transfer completed
	 *	* 1: Byte transfer in progress
	 */
	kI2CSR_TIP_bp = 7,
	kI2CSR_TIP_bm = (0b1 << kI2CSR_TIP_bp),
} I2CSR_t;

/*
 *	I2CTXDR - I2C Transmitting Data Register
 *
 *	I2C Transmit Data – This register holds the byte that will be transmitted on the I2C
 *	bus during the Write Data phase. Bit 0 is the LSB and will be transmitted last. When
 *	transmitting the slave address, Bit 0 represents the Read/Write bit.
 */
typedef enum I2CTXDR_enum
{
	kI2CTXDR_bp = 0,
	kI2CTXDR_bm = 0b11111111,
} I2CTXDR_t;

/*
 *	I2CRXDR - I2C Receiving Data Register
 *
 *	I2C Receive Data – This register holds the byte captured from the I2C bus during the
 *	Read Data phase. Bit 0 is LSB and was received last.
 */
typedef enum I2CRXDR_enum
{
	kI2CRXDR_bp = 0,
	kI2CRXDR_bm = 0b11111111,
} I2CRXDR_t;

/*
 *	I2CGCDR - I2C General Call Data Register
 *
 *	I2C General Call Data – This register holds the second (command) byte of the
 *	General Call transaction on the I2C bus.
 */
typedef enum I2CGCDR_enum
{
	kI2CGCDR_bp = 0,
	kI2CGCDR_bm = 0b11111111,
} I2CGCDR_t;

/*
 *	I2CSADDR - I2C Slave Address MSB Register
 */
typedef enum I2CSADDR_enum
{
	kI2CSADDR_7BIT_ADDR_bp = 0,
	kI2CSADDR_7BIT_ADDR_bm = 0b11111,

	kI2CSADDR_10BIT_ADDR_bp = 0,
	kI2CSADDR_10BIT_ADDR_bm = 0b11111111,
} I2CSADDR_t;

/*
 *	I2CIRQEN - I2C Interrupt Control Register
 */
typedef enum I2CIRQEN_enum
{
	/*
	 *	Interrupt Enable for Hardware General Call Received
	 */
	kI2CIRQEN_IRQHGCEN_bp = 0,
	kI2CIRQEN_IRQHGCEN_bm = (0b1 << kI2CIRQEN_IRQHGCEN_bp),

	/*
	 *	Interrupt Enable for Transmitter/Receiver Overrun or NACK Received
	 */
	kI2CIRQEN_IRQTROEEN_bp = 1,
	kI2CIRQEN_IRQTROEEN_bm = (0b1 << kI2CIRQEN_IRQTROEEN_bp),

	/*
	 *	Interrupt Enable for Transmitter or Receiver Ready
	 */
	kI2CIRQEN_IRQTRRDYEN_bp = 2,
	kI2CIRQEN_IRQTRRDYEN_bm = (0b1 << kI2CIRQEN_IRQTRRDYEN_bp),

	/*
	 *	Interrupt Enable for Arbitration Lost
	 */
	kI2CIRQEN_IRQARBLEN_bp = 3,
	kI2CIRQEN_IRQARBLEN_bm = (0b1 << kI2CIRQEN_IRQARBLEN_bp),

	/*
	 *	Force Interrupt Request On – Force the Interrupt Flag set to improve testability
	 */
	kI2CIRQEN_IRQINTFRC_bp = 6,
	kI2CIRQEN_IRQINTFRC_bm = (0b1 << kI2CIRQEN_IRQINTFRC_bp),

	/*
	 *	Auto Interrupt Clear Enable – Enable the interrupt flag auto clear when the I2CIRQ has been read.
	 */
	kI2CIRQEN_IRQINTCLREN_bp = 7,
	kI2CIRQEN_IRQINTCLREN_bm = (0b1 << kI2CIRQEN_IRQINTCLREN_bp),
} I2CIRQEN_t;

/*
 *	I2CIRQ - I2C Interrupt Status Register
 */
typedef enum I2CIRQ_enum
{
	/*
	 *	Interrupt Status for Hardware General Call Received. When enabled, indicates HGC was asserted.
	 *	Write a ‘1’ to this bit to clear the interrupt.
	 *	* 0: No interrupt
	 *	* 1: General Call Received in slave mode Interrupt
	 */
	kI2CIRQ_IRQHGC_bp = 0,
	kI2CIRQ_IRQHGC_bm = (0b1 << kI2CIRQ_IRQHGC_bp),

	/*
	 *	Interrupt Status for Transmitter/Receiver Overrun or NACK received. When enabled, indicates TROE
	 *	was asserted. Write a ‘1’ to this bit to clear the interrupt.
	 *	* 0: No interrupt
	 *	* 1: Transmitter or Receiver Overrun or NACK received Interrupt
	 */
	kI2CIRQ_IRQTROE_bp = 1,
	kI2CIRQ_IRQTROE_bm = (0b1 << kI2CIRQ_IRQTROE_bp),

	/*
	 *	Interrupt Status for Transmitter or Receiver Ready. When enabled, indicates TRRDY was asserted.
	 *	Write a ‘1’ to this bit to clear the interrupt.
	 *	* 0: No interrupt
	 *	* 1: Transmitter or Receiver Ready Interrupt
	 */
	kI2CIRQ_IRQTRRDY_bp = 2,
	kI2CIRQ_IRQTRRDY_bm = (0b1 << kI2CIRQ_IRQTRRDY_bp),

	/*
	 *	Interrupt Status for Arbitration Lost. When enabled, indicates ARBL was asserted. Write a ‘1’ to this
	 *	bit to clear the interrupt.
	 *	* 0: No interrupt
	 *	* 1: Arbitration Lost Interrupt
	 */
	kI2CIRQ_IRQARBL_bp = 3,
	kI2CIRQ_IRQARBL_bm = (0b1 << kI2CIRQ_IRQARBL_bp),
} I2CIRQ_t;

#ifdef __cplusplus
}
#endif

#endif
