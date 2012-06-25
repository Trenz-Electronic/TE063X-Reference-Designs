/*****************************************************************************
* Filename:          C:\Projects\Trenz\repos\TE063X-Reference-Designs\axi-TE0630/drivers/axi_fx2_v1_00_a/src/axi_fx2.h
* Version:           1.00.a
* Description:       axi_fx2 Driver Header File
* Date:              Mon Jun 25 15:20:38 2012 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef AXI_FX2_H
#define AXI_FX2_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 * -- SLV_REG4 : user logic slave module register 4
 * -- SLV_REG5 : user logic slave module register 5
 * -- SLV_REG6 : user logic slave module register 6
 * -- SLV_REG7 : user logic slave module register 7
 */
#define AXI_FX2_USER_SLV_SPACE_OFFSET (0x00000000)
#define AXI_FX2_SLV_REG0_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000000)
#define AXI_FX2_SLV_REG1_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000004)
#define AXI_FX2_SLV_REG2_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000008)
#define AXI_FX2_SLV_REG3_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x0000000C)
#define AXI_FX2_SLV_REG4_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000010)
#define AXI_FX2_SLV_REG5_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000014)
#define AXI_FX2_SLV_REG6_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x00000018)
#define AXI_FX2_SLV_REG7_OFFSET (AXI_FX2_USER_SLV_SPACE_OFFSET + 0x0000001C)

/**
 * Software Reset Space Register Offsets
 * -- RST : software reset register
 */
#define AXI_FX2_SOFT_RST_SPACE_OFFSET (0x00000100)
#define AXI_FX2_RST_REG_OFFSET (AXI_FX2_SOFT_RST_SPACE_OFFSET + 0x00000000)

/**
 * Software Reset Masks
 * -- SOFT_RESET : software reset
 */
#define SOFT_RESET (0x0000000A)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a AXI_FX2 register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the AXI_FX2 device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void AXI_FX2_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define AXI_FX2_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a AXI_FX2 register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the AXI_FX2 device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 AXI_FX2_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define AXI_FX2_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from AXI_FX2 user logic slave registers.
 *
 * @param   BaseAddress is the base address of the AXI_FX2 device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void AXI_FX2_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 AXI_FX2_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define AXI_FX2_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg4(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG4_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg5(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG5_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg6(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG6_OFFSET) + (RegOffset), (Xuint32)(Value))
#define AXI_FX2_mWriteSlaveReg7(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (AXI_FX2_SLV_REG7_OFFSET) + (RegOffset), (Xuint32)(Value))

#define AXI_FX2_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG0_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg1(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG1_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg2(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG2_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg3(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG3_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg4(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG4_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg5(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG5_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg6(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG6_OFFSET) + (RegOffset))
#define AXI_FX2_mReadSlaveReg7(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (AXI_FX2_SLV_REG7_OFFSET) + (RegOffset))

/**
 *
 * Reset AXI_FX2 via software.
 *
 * @param   BaseAddress is the base address of the AXI_FX2 device.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void AXI_FX2_mReset(Xuint32 BaseAddress)
 *
 */
#define AXI_FX2_mReset(BaseAddress) \
 	Xil_Out32((BaseAddress)+(AXI_FX2_RST_REG_OFFSET), SOFT_RESET)

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the AXI_FX2 instance to be worked on.
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
XStatus AXI_FX2_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 8


#endif /** AXI_FX2_H */
