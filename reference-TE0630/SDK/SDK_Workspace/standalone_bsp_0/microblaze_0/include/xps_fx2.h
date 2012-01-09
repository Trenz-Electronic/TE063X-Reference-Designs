//////////////////////////////////////////////////////////////////////////////
// Filename:          E:\DOKTORAT\Projekti\HSS3\EDK\MyProcessorIPLib/drivers/xps_fx2_v1_00_a/src/xps_fx2.h
// Version:           1.00.a
// Description:       xps_fx2 Driver Header File
// Date:              Thu Apr 10 23:54:23 2008 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////

#ifndef XPS_FX2_H
#define XPS_FX2_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

/************************** Constant Definitions ***************************/

#define XPS_FX2_CR_OFFSET  (0x00000000)
#define XPS_FX2_FTR_OFFSET (0x00000004)
#define XPS_FX2_SR_OFFSET  (0x00000008)
#define XPS_FX2_TX_OFFSET  (0x0000000C)
#define XPS_FX2_RX_OFFSET  (0x00000010)

#define XPS_FX2_CR_TX_FIFO_RST_MASK  (0x00000001)
#define XPS_FX2_CR_RX_FIFO_RST_MASK  (0x00000002)
#define XPS_FX2_CR_RX_FIFO_RDY_MASK  (0x00000004)
#define XPS_FX2_CR_USB_FIFOADR_MASK  (0x00000030)
#define XPS_FX2_CR_USB_FIFOADR_SHIFT  4
#define XPS_FX2_CR_PKTEND_TIMEOUT_MASK  (0xFFFF0000)
#define XPS_FX2_CR_PKTEND_TIMEOUT_SHIFT  16

#define XPS_FX2_FTR_TX_FIFO_THRESHOLD_MASK  (0x00000FFF)
#define XPS_FX2_FTR_TX_FIFO_THRESHOLD_SHIFT  0
#define XPS_FX2_FTR_RX_FIFO_THRESHOLD_MASK  (0x0FFF0000)
#define XPS_FX2_FTR_RX_FIFO_THRESHOLD_SHIFT  16

#define XPS_FX2_SR_TX_FIFO_THRESHOLD_MASK  (0x00000FFF)
#define XPS_FX2_SR_TX_FIFO_THRESHOLD_SHIFT  0
#define XPS_FX2_SR_RX_FIFO_THRESHOLD_MASK  (0x0FFF0000)
#define XPS_FX2_SR_RX_FIFO_THRESHOLD_SHIFT  16

enum PI_PipeNumber
{
	PI_EP2	= 0,	
	PI_EP4	= 1,	
	PI_EP6	= 2,
	PI_EP8	= 3
};

/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 * -- SLV_REG4 : user logic slave module register 4
 */
#define XPS_FX2_USER_SLV_SPACE_OFFSET (0x00000000)
#define XPS_FX2_SLV_REG0_OFFSET (XPS_FX2_USER_SLV_SPACE_OFFSET + 0x00000000)
#define XPS_FX2_SLV_REG1_OFFSET (XPS_FX2_USER_SLV_SPACE_OFFSET + 0x00000004)
#define XPS_FX2_SLV_REG2_OFFSET (XPS_FX2_USER_SLV_SPACE_OFFSET + 0x00000008)
#define XPS_FX2_SLV_REG3_OFFSET (XPS_FX2_USER_SLV_SPACE_OFFSET + 0x0000000C)
#define XPS_FX2_SLV_REG4_OFFSET (XPS_FX2_USER_SLV_SPACE_OFFSET + 0x00000010)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define XPS_FX2_SOFT_RST_SPACE_OFFSET (0x00000100)
#define XPS_FX2_RST_REG_OFFSET (XPS_FX2_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**
 * Interrupt Controller Space Offsets
 * -- INTR_DISR  : device (peripheral) interrupt status register
 * -- INTR_DIPR  : device (peripheral) interrupt pending register
 * -- INTR_DIER  : device (peripheral) interrupt enable register
 * -- INTR_DIIR  : device (peripheral) interrupt id (priority encoder) register
 * -- INTR_DGIER : device (peripheral) global interrupt enable register
 * -- INTR_ISR   : ip (user logic) interrupt status register
 * -- INTR_IER   : ip (user logic) interrupt enable register
 */
#define XPS_FX2_INTR_CNTRL_SPACE_OFFSET (0x00000200)
#define XPS_FX2_INTR_DISR_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define XPS_FX2_INTR_DIPR_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define XPS_FX2_INTR_DIER_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define XPS_FX2_INTR_DIIR_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define XPS_FX2_INTR_DGIER_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define XPS_FX2_INTR_IPISR_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define XPS_FX2_INTR_IPIER_OFFSET (XPS_FX2_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

/**
 * Interrupt Controller Masks
 * -- INTR_TERR_MASK : transaction error
 * -- INTR_DPTO_MASK : data phase time-out
 * -- INTR_IPIR_MASK : ip interrupt requeset
 * -- INTR_RFDL_MASK : read packet fifo deadlock interrupt request
 * -- INTR_WFDL_MASK : write packet fifo deadlock interrupt request
 * -- INTR_IID_MASK  : interrupt id
 * -- INTR_GIE_MASK  : global interrupt enable
 * -- INTR_NOPEND    : the DIPR has no pending interrupts
 */
#define INTR_TERR_MASK (0x00000001UL)
#define INTR_DPTO_MASK (0x00000002UL)
#define INTR_IPIR_MASK (0x00000004UL)
#define INTR_RFDL_MASK (0x00000020UL)
#define INTR_WFDL_MASK (0x00000040UL)
#define INTR_IID_MASK (0x000000FFUL)
#define INTR_GIE_MASK (0x80000000UL)
#define INTR_NOPEND (0x80)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a XPS_FX2 register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void XPS_FX2_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define XPS_FX2_mWriteReg(BaseAddress, RegOffset, Data) \
 	XIo_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a XPS_FX2 register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 XPS_FX2_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define XPS_FX2_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from XPS_FX2 user logic slave registers.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void XPS_FX2_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 XPS_FX2_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define XPS_FX2_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (XPS_FX2_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define XPS_FX2_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (XPS_FX2_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define XPS_FX2_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (XPS_FX2_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define XPS_FX2_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (XPS_FX2_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))
#define XPS_FX2_mWriteSlaveReg4(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (XPS_FX2_SLV_REG4_OFFSET) + (RegOffset), (Xuint32)(Value))

#define XPS_FX2_mReadSlaveReg0(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (XPS_FX2_SLV_REG0_OFFSET) + (RegOffset))
#define XPS_FX2_mReadSlaveReg1(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (XPS_FX2_SLV_REG1_OFFSET) + (RegOffset))
#define XPS_FX2_mReadSlaveReg2(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (XPS_FX2_SLV_REG2_OFFSET) + (RegOffset))
#define XPS_FX2_mReadSlaveReg3(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (XPS_FX2_SLV_REG3_OFFSET) + (RegOffset))
#define XPS_FX2_mReadSlaveReg4(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (XPS_FX2_SLV_REG4_OFFSET) + (RegOffset))

/**
 *
 * Reset XPS_FX2 via software.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void XPS_FX2_mReset(Xuint32 BaseAddress)
 *
 */
#define XPS_FX2_mReset(BaseAddress) \
 	XIo_Out32((BaseAddress)+(XPS_FX2_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/

/*
 * Sets a bitwise value to OPBFX2 CR register using MASK.
 *
 * @param   BaseAddress is the base address of the OPBFX2 device.
 * @param   Mask is a bit mask.
 * @param   Data sets bits in masked value only.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Data)
 *
 */
void XPS_FX2_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Data);

/*
 * Writes a value to a XPS_FX2 CR register bit 30.
 * This performs XPS_FX2 FIFO reset.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_TXFIFOrst(Xuint32 BaseAddress)
 *
 */

void XPS_FX2_TXFIFOrst(Xuint32 BaseAddress);

/*
 * Writes a value to a XPS_FX2 CR register bit 30.
 * This performs XPS_FX2 FIFO reset.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_RXFIFOrst(Xuint32 BaseAddress)
 *
 */

void XPS_FX2_RXFIFOrst(Xuint32 BaseAddress);

/*
 * Writes a value to a XPS_FX2 CR register bit 29.
 * This performs XPS_FX2 external FIFO_RDY signal enable.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Enable is the flag for enabling (1) or disabling (0) extzernal FIFO interface.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_TXFIFO_Ready(Xuint32 BaseAddress, Xuint8 Enable)
 *
 */

void XPS_FX2_TXFIFO_Ready(Xuint32 BaseAddress, Xuint8 Enable);

/*
 * Writes a value to a XPS_FX2 CR register bit 26-27.
 * This sets FX2 slave fifo address.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Address is a FX2 slave FIFO selection address
 *          0=fifo02, 1=fifo04, 2=fifo06, 3=fifo08
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_SetUSB_FIFOadr(Xuint32 BaseAddress, enum PI_PipeNumber PipeNo)
 *
 */

void XPS_FX2_SetUSB_FIFOadr(Xuint32 BaseAddress, enum PI_PipeNumber PipeNo);

/*
 * Writes a value to a XPS_FX2 CR register bit 0-15.
 * This sets XPS_FX2 PKTEND timeout value.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Timeout is the timeout counter value before asserting PKTEND.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_PktendTimeout(Xuint32 BaseAddress, Xuint16 Timeout)
 *
 */

void XPS_FX2_PktendTimeout(Xuint32 BaseAddress, Xuint16 Timeout);

/*
 * Writes a value to a XPS_FX2 CR register.
 * This sets XPS_FX2 FIFO programmable empty threshold.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Threshold is 9 bit FIFO threshold
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_SetTXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold)
 *
 */

void XPS_FX2_SetTXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold);

/*
 * Writes a value to a XPS_FX2 CR register.
 * This sets XPS_FX2 FIFO programmable empty threshold.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Threshold is 9 bit FIFO threshold
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_SetTXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold)
 *
 */

void XPS_FX2_SetRXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold);

/*
 * Writes a value to a XPS_FX2 CR register bit 30.
 * This performs XPS_FX2 FIFO reset.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  9 bit FIFO occupancy counter
 *
 * @note    None.
 *
 * C-style signature:
 * 	Xuint16 XPS_FX2_GetTXFIFOcount(Xuint32 BaseAddress)
 *
 */

Xuint16 XPS_FX2_GetTXFIFOcount(Xuint32 BaseAddress);

/*
 * Writes a value to a XPS_FX2 CR register bit 30.
 * This performs XPS_FX2 FIFO reset.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  9 bit FIFO occupancy counter
 *
 * @note    None.
 *
 * C-style signature:
 * 	Xuint16 XPS_FX2_GetTXFIFOcount(Xuint32 BaseAddress)
 *
 */

Xuint16 XPS_FX2_GetRXFIFOcount(Xuint32 BaseAddress);

/*
 * Writes a value to a XPS_FX2 TX register.
 * This sets puts 32bits of data to TX_FIFO
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 * @param   Data is a data to be written
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_FX2_WriteTXFifo(Xuint32 BaseAddress, Xuint32 Data)
 *
 */

void XPS_FX2_WriteTXFifo(Xuint32 BaseAddress, Xuint32 Data);

/*
 * Writes a value to a XPS_FX2 RX register.
 * This performs 32bit read from XPS_FX2 RX fifo.
 *
 * @param   BaseAddress is the base address of the XPS_FX2 device.
 *
 * @return  32bit RX_FIFO data
 *
 * @note    None.
 *
 * C-style signature:
 * 	Xuint32 XPS_FX2_ReadRXFifo(Xuint32 BaseAddress)
 *
 */

Xuint32 XPS_FX2_ReadRXFifo(Xuint32 BaseAddress);

/**
 *
 * Enable all possible interrupts from XPS_FX2 device.
 *
 * @param   baseaddr_p is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_FX2_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_FX2_Intr_DefaultHandler(void * baseaddr_p);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the XPS_FX2 instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus XPS_FX2_SelfTest(void * baseaddr_p);

#endif // XPS_FX2_H
