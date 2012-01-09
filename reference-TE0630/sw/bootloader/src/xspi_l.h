/* $Id: xspi_l.h,v 1.4.6.2 2008/02/28 11:08:19 svemula Exp $ */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2002-2008 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xspi_l.h
*
* This header file contains identifiers and low-level driver functions (or
* macros) that can be used to access the device.  High-level driver functions
* are defined in xspi.h.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b rpm  04/24/02 First release
* 1.11a wgr  03/22/07 Converted to new coding style.
* 1.11a  sv  02/22/08 Added the definition of LSB-MSB first option.
* </pre>
*
******************************************************************************/

#ifndef XSPI_L_H		/* prevent circular inclusions */
#define XSPI_L_H		/* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"
#include "xbasic_types.h"
#include "xstatus.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants define the register offsets for the registers of the
 * IPIF, there are some holes in the memory map for reserved addresses to allow
 * other registers to be added and still match the memory map of the interrupt
 * controller registers
 */
#define XSPI_DGIER_OFFSET    28 /* device global interrupt enable reg */
#define XSPI_IISR_OFFSET     32 /* IP interrupt status register */
#define XSPI_IIER_OFFSET     40 /* IP interrupt enable register */
#define XSPI_RESETR_OFFSET   64 /* reset register */


#define XSPI_RESET_MASK             0xA

/*
 * The following constant is used for the device global interrupt enable
 * register, to enable all interrupts for the device, this is the only bit
 * in the register
 */
#define XSPI_GINTR_ENABLE_MASK      0x80000000UL

/*
 * The following constants contain the masks to identify each internal IPIF
 * condition in the device registers of the IPIF, interrupts are assigned
 * in the register from LSB to the MSB
 */
#define XSPI_ERROR_MASK             1     /* LSB of the register */

/*
 * The following constants contain interrupt IDs which identify each internal
 * IPIF condition, this value must correlate with the mask constant for the
 * error
 */
#define XSPI_ERROR_INTERRUPT_ID     0    /* interrupt bit #, (LSB = 0) */
#define XSPI_NO_INTERRUPT_ID        128  /* no interrupts are pending */

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/******************************************************************************
*
* This macro resets the SPI device.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XSPI_RESET(u32 RegBaseAddress);
*
******************************************************************************/
#define XSPI_RESET(RegBaseAddress) \
    XIo_Out32(RegBaseAddress + XSPI_RESETR_OFFSET, XSPI_RESET_MASK)

/******************************************************************************
*
* This macro disables all interrupts for the device by writing to the Global
* interrupt enable register.  This register provides the ability to disable
* interrupts without any modifications to the interrupt enable register such
* that it is minimal effort to restore the interrupts to the previous enabled
* state.  The corresponding function, XSPI_GINTR_ENABLE, is provided to
* restore the interrupts to the previous enabled state.  This function is
* designed to be used in critical sections of device drivers such that it is
* not necessary to disable other device interrupts.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XSPI_GINTR_DISABLE(u32 RegBaseAddress);
*
******************************************************************************/
#define XSPI_GINTR_DISABLE(RegBaseAddress) \
	XIo_Out32((RegBaseAddress) + XSPI_DGIER_OFFSET, 0)

/******************************************************************************
*
* This macro writes to the global interrupt enable register to enable
* interrupts from the device.  This register provides the ability to enable
* interrupts without any modifications to the interrupt enable register such
* that it is minimal effort to restore the interrupts to the previous enabled
* state. This function does not enable individual interrupts as the interrupt
* enable register must be set appropriately.  This function is designed to be
* used in critical sections of device drivers such that it is not necessary to
* disable other device interrupts.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XSPI_GINTR_ENABLE(u32 RegBaseAddress);
*
******************************************************************************/
#define XSPI_GINTR_ENABLE(RegBaseAddress)           \
	XIo_Out32((RegBaseAddress) + XSPI_DGIER_OFFSET, \
	XSPI_GINTR_ENABLE_MASK)

/******************************************************************************
*
* This function determines if interrupts are enabled at the global level by
* reading the global interrupt register. This register provides the ability to
* disable interrupts without any modifications to the interrupt enable register
* such that it is minimal effort to restore the interrupts to the previous
* enabled state.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return
*		- TRUE if global interrupts are enabled.
*		- FALSE if global interrupts are disabled.
*
* @note		C-Style signature:
*		int XSPI_IS_GINTR_ENABLED(u32 RegBaseAddress);
*
******************************************************************************/
#define XSPI_IS_GINTR_ENABLED(RegBaseAddress)             \
    (XIo_In32((RegBaseAddress) + XSPI_DGIER_OFFSET) ==    \
              XSPI_GINTR_ENABLE_MASK)

/******************************************************************************
*
*
* This function sets the Interrupt status register to the specified value.
* This register indicates the status of interrupt sources for the device.
* The status is independent of whether interrupts are enabled such that
* the status register may also be polled when interrupts are not enabled.
*
* Each bit of the register correlates to a specific interrupt source within the
* SPI device.  All bits of this register are latched. Setting a bit which is
* zero within this register causes an interrupt to be generated. The device
* global interrupt enable register and the device interrupt enable register must
* be set appropriately to allow an interrupt to be passed out of the device. The
* interrupt is cleared by writing to this register with the bits to be
* cleared set to a one and all others to zero.  This register implements a
* toggle on write functionality meaning any bits which are set in the value
* written cause the bits in the register to change to the opposite state.
*
* This function writes only the specified value to the register such that
* some status bits may be set and others cleared.  It is the caller's
* responsibility to get the value of the register prior to setting the value
* to prevent an destructive behavior.
*
* @param	RegBaseAddress is the base address of the SPI device.
* @param	Status contains the value to be written to the Interrupt
*		status register.
*
* @return	None.
*
* @note		C-Style signature:
*		void XSPI_WRITE_IISR(u32 RegBaseAddress, u32 Status);
*
******************************************************************************/
#define XSPI_WRITE_IISR(RegBaseAddress, Status) \
    XIo_Out32((RegBaseAddress) + XSPI_IISR_OFFSET, (Status))

/******************************************************************************
*
*
* This function gets the contents of the Interrupt Status Register.
* This register indicates the status of interrupt sources for the device.
* The status is independent of whether interrupts are enabled such
* that the status register may also be polled when interrupts are not enabled.
*
* Each bit of the register correlates to a specific interrupt source within the
* device.  All bits of this register are latched.  Writing a 1 to a bit within
* this register causes an interrupt to be generated if enabled in the interrupt
* enable register and the global interrupt enable is set.  Since the status is
* latched, each status bit must be acknowledged in order for the bit in the
* status register to be updated.  Each bit can be acknowledged by writing a
* 0 to the bit in the status register.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return	A status which contains the value read from the Interrupt
*		Status Register.
*
* @note		C-Style signature:
*		u32 XSPI_READ_IISR(u32 RegBaseAddress);
*
******************************************************************************/
#define XSPI_READ_IISR(RegBaseAddress) \
    XIo_In32((RegBaseAddress) + XSPI_IISR_OFFSET)

/******************************************************************************
*
* This function sets the contents of the Interrupt Enable Register . This
* register controls which interrupt sources of the SPI device are allowed to
* generate an interrupt. The global interrupt enable register and the device
* interrupt enable register must also be set appropriately for an interrupt to
* be passed out of the device.
*
* Each bit of the register correlates to a specific interrupt source within the
* device. Setting a bit in this register enables the interrupt source to
* generate an interrupt. Clearing a bit in this register disables interrupt
* generation for that interrupt source.
*
* This function writes only the specified value to the register such that
* some interrupt sources may be enabled and others disabled. It is the
* caller's responsibility to get the value of the interrupt enable register
* prior to setting the value to prevent a destructive behavior.
*
* @param	RegBaseAddress is the base address of the SPI device.
* @param	Enable contains the value to be written to the Interrupt Enable
*		Register.
*
* @return 	None
*
* @note		C-Style signature:
*		void XSPI_WRITE_IIER(u32 RegBaseAddress, u32 Enable);
*
******************************************************************************/
#define XSPI_WRITE_IIER(RegBaseAddress, Enable) \
    XIo_Out32((RegBaseAddress) + XSPI_IIER_OFFSET, (Enable))

/******************************************************************************
*
*
* This function gets the Interrupt enable register contents.  This register
* controls which interrupt sources of the device are allowed to generate an
* interrupt.  The global interrupt enable register and the device interrupt
* enable register must also be set appropriately for an interrupt to be
* passed out of the SPI device.
*
* Each bit of the register correlates to a specific interrupt source within the
* SPI device. Setting a bit in this register enables the interrupt source to
* generate an interrupt.  Clearing a bit in this register disables interrupt
* generation for that interrupt source.
*
* @param	RegBaseAddress is the base address of the SPI device.
*
* @return	The contents read from the Interrupt Enable Register.
*
* @note		C-Style signature:
*		u32 XSPI_READ_IIER(u32 RegBaseAddress)
*
******************************************************************************/
#define XSPI_READ_IIER(RegBaseAddress) \
    XIo_In32((RegBaseAddress) + XSPI_IIER_OFFSET)

/************************** Function Prototypes ******************************/

/************************** Constant Definitions *****************************/

/*
 * Offset from the device base address (IPIF) to the IP registers.
 */
#define XSP_REGISTER_OFFSET      0x60

/*
 * Register offsets for the SPI. Each register except the CR & SSR is 8 bits,
 * so add 3 to the word-offset to get the LSB (in a big-endian system).
 */
#define XSP_CR_OFFSET   (XSP_REGISTER_OFFSET + 0x2)	/* 16-bit Control */
#define XSP_SR_OFFSET   (XSP_REGISTER_OFFSET + 0x4 + 3)	/* Status */
#define XSP_DTR_OFFSET  (XSP_REGISTER_OFFSET + 0x8 + 3)	/* Data transmit */
#define XSP_DRR_OFFSET  (XSP_REGISTER_OFFSET + 0xC + 3)	/* Data receive */
#define XSP_SSR_OFFSET  (XSP_REGISTER_OFFSET + 0x10) /* 32-bit slave select */
#define XSP_TFO_OFFSET  (XSP_REGISTER_OFFSET + 0x14 + 3) /* Tx FIFO occupancy */
#define XSP_RFO_OFFSET  (XSP_REGISTER_OFFSET + 0x18 + 3) /* Rx FIFO occupancy */

/*
 * SPI Control Register (CR) masks
 */
#define XSP_CR_LOOPBACK_MASK        0x1	/* Local loopback mode */
#define XSP_CR_ENABLE_MASK          0x2	/* System enable */
#define XSP_CR_MASTER_MODE_MASK     0x4	/* Enable master mode */
#define XSP_CR_CLK_POLARITY_MASK    0x8	/* Clock polarity high or low */
#define XSP_CR_CLK_PHASE_MASK      0x10	/* Clock phase 0 or 1 */
#define XSP_CR_TXFIFO_RESET_MASK   0x20	/* Reset transmit FIFO */
#define XSP_CR_RXFIFO_RESET_MASK   0x40	/* Reset receive FIFO */
#define XSP_CR_MANUAL_SS_MASK      0x80	/* Manual slave select assertion */
#define XSP_CR_TRANS_INHIBIT_MASK  0x100 /* Master transaction inhibit */

/*
 * LSB/MSB first data format select. The default data format is MSB first.
 * The LSB first data format is not available in all versions of the Xilinx Spi
 * Device whereas the MSB first data format is supported by all the versions of
 * the Xilinx Spi Devices. Please check the HW specification to see if this
 * feature is supported or not.
 */
#define XSP_CR_LSB_MSB_FIRST_MASK  0x200

/*
 * SPI Status Register (SR) masks
 */
#define XSP_SR_RX_EMPTY_MASK        0x1	/* Receive register/FIFO is empty */
#define XSP_SR_RX_FULL_MASK         0x2	/* Receive register/FIFO is full */
#define XSP_SR_TX_EMPTY_MASK        0x4	/* Transmit register/FIFO is empty */
#define XSP_SR_TX_FULL_MASK         0x8	/* Transmit register/FIFO is full */
#define XSP_SR_MODE_FAULT_MASK     0x10	/* Mode fault error */

/*
 * SPI Transmit FIFO Occupancy (TFO) mask. The binary value plus one yields
 * the occupancy.
 */
#define XSP_TFO_MASK        0x1F

/*
 * SPI Receive FIFO Occupancy (RFO) mask. The binary value plus one yields
 * the occupancy.
 */
#define XSP_RFO_MASK        0x1F


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/****************************************************************************/
/**
*
* Set the contents of the control register. Use the XSP_CR_* constants defined
* above to create the bit-mask to be written to the register.
*
* @param	BaseAddress is the base address of the device.
* @param	Mask is the 16-bit value to write to the control register.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSpi_mSetControlReg(u32 BaseAddress, u16 Mask);
*
*****************************************************************************/
#define XSpi_mSetControlReg(BaseAddress, Mask) \
	XIo_Out16((BaseAddress) + XSP_CR_OFFSET, (Mask))


/****************************************************************************/
/**
*
* Get the contents of the control register. Use the XSP_CR_* constants defined
* above to interpret the bit-mask returned.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	A 16-bit value representing the contents of the control
*		register.
*
* @note		C-Style signature:
* 		u16 XSpi_mGetControlReg(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mGetControlReg(BaseAddress) \
	XIo_In16((BaseAddress) + XSP_CR_OFFSET)


/****************************************************************************/
/**
*
* Get the contents of the status register. Use the XSP_SR_* constants defined
* above to interpret the bit-mask returned.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	An 8-bit value representing the contents of the status register.
*
* @note		C-Style signature:
* 		u8 XSpi_mGetStatusReg(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mGetStatusReg(BaseAddress) \
	XIo_In8((BaseAddress) + XSP_SR_OFFSET)


/****************************************************************************/
/**
*
* Set the contents of the slave select register. Each bit in the mask
* corresponds to a slave select line. Only one slave should be selected at
* any one time.
*
* @param	BaseAddress is the  base address of the device.
* @param	Mask is the 32-bit value to write to the slave select register.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSpi_mSetSlaveSelectReg(u32 BaseAddress, u32 Mask);
*
*****************************************************************************/
#define XSpi_mSetSlaveSelectReg(BaseAddress, Mask) \
	XIo_Out32((BaseAddress) + XSP_SSR_OFFSET, (Mask))


/****************************************************************************/
/**
*
* Get the contents of the slave select register. Each bit in the mask
* corresponds to a slave select line. Only one slave should be selected at
* any one time.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	The 32-bit value in the slave select register.
*
* @note		C-Style signature:
* 		u32 XSpi_mGetSlaveSelectReg(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mGetSlaveSelectReg(BaseAddress) \
	XIo_In32((BaseAddress) + XSP_SSR_OFFSET)

/****************************************************************************/
/**
*
* Enable the device and uninhibit master transactions. Preserves the current
* contents of the control register.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSpi_mEnable(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mEnable(BaseAddress) \
{ \
	u16 Control; \
	Control = XSpi_mGetControlReg((BaseAddress)); \
	Control |= XSP_CR_ENABLE_MASK; \
	Control &= ~XSP_CR_TRANS_INHIBIT_MASK; \
	XSpi_mSetControlReg((BaseAddress), Control); \
}

/****************************************************************************/
/**
*
* Disable the device. Preserves the current contents of the control register.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
* 		void XSpi_mDisable(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mDisable(BaseAddress) \
	XSpi_mSetControlReg((BaseAddress), \
	XSpi_mGetControlReg((BaseAddress)) & ~XSP_CR_ENABLE_MASK)


/****************************************************************************/
/**
*
* Send one byte to the currently selected slave. The byte that is received
* from the slave is saved in the receive FIFO/register.
*
* @param	BaseAddress is the  base address of the device.
* @param	Data is the data to be sent.
*
* @return	None.
*
* @note		C-Style signature:
* 		XSpi_mSendByte(u32 BaseAddress, u8 Data);
*
*****************************************************************************/
#define XSpi_mSendByte(BaseAddress, Data) \
	XIo_Out8((BaseAddress) + XSP_DTR_OFFSET, (Data))


/****************************************************************************/
/**
*
* Receive one byte from the device's receive FIFO/register. It is assumed
* that the byte is already available.
*
* @param	BaseAddress is the  base address of the device.
*
* @return	The byte retrieved from the receive FIFO/register.
*
* @note		C-Style signature:
* 		u8 XSpi_mRecvByte(u32 BaseAddress);
*
*****************************************************************************/
#define XSpi_mRecvByte(BaseAddress) \
	XIo_In8((BaseAddress) + XSP_DRR_OFFSET)

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif /* end of protection macro */

