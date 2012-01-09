//////////////////////////////////////////////////////////////////////////////
// Filename:          E:\DOKTORAT\Projekti\HSS4\EDK\HSS4_v0.1.0.0/drivers/xps_npi_dma_v1_00_a/src/xps_npi_dma.h
// Version:           1.00.a
// Description:       xps_npi_dma Driver Header File
// Date:              Thu Sep 11 16:59:40 2008 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////

#ifndef XPS_NPI_DMA_H
#define XPS_NPI_DMA_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

typedef struct
{
	Xuint32 BaseAddress;	//base address of the DMA device
	Xuint32 WrStartAddr;	//start address for writing
	Xuint32 RdStartAddr;	//start address for reading
	Xuint32 WrBytes;			//number of bytes for writing
	Xuint32 RdBytes;			//number of bytes for reading
	Xuint16 RdJumpBytes;	//read interleaving address jump (cols)
	Xuint16 RdRows;				//number of rows at reading
	Xuint8  WrBurstSize;	//write burst size (for detailed description see SetWrBurstSize)
	Xuint8  RdBurstSize;	//read burst size (for detailed description see SetRdBurstSize)
	Xuint8  WrLoop;				//continous writing flag
	Xuint8  RdLoop;				//continous reading flag
}NPI_DMA_TYPE;

/************************** Constant Definitions ***************************/
/**
 * NPI_DMA Registers Space Offsets
 * -- CR      : control register
 * -- WSA     : write start address register
 * -- WBR     : write bytes register
 * -- RSA     : read start address register
 * -- RBR     : read bytes register
 * -- RJR     : read jump register
 * -- SR      : status register
 * -- WCR     : write counter register
 * -- RCR     : read counter register
 */
 
#define XPS_NPI_DMA_CR_OFFSET  	(0x00000000)
#define XPS_NPI_DMA_WSA_OFFSET 	(0x00000004)
#define XPS_NPI_DMA_WBR_OFFSET 	(0x00000008)
#define XPS_NPI_DMA_RSA_OFFSET 	(0x0000000C)
#define XPS_NPI_DMA_RBR_OFFSET 	(0x00000010)
#define XPS_NPI_DMA_RJR_OFFSET 	(0x00000014)

#define XPS_NPI_DMA_SR_OFFSET  	(0x00000018)
#define XPS_NPI_DMA_WCR_OFFSET 	(0x0000001C)
#define XPS_NPI_DMA_RCR_OFFSET 	(0x00000020)

/**
 * NPI_DMA Registers Masks
 * -- CR_RST_MASK               : NPI_DMA master reset mask
 * -- CR_WR_FIFO_RST_MASK       : NPI_DMA Write FIFO reset mask
 * -- CR_RD_FIFO_RST_MASK       : NPI_DMA Read FIFO reset mask
 * -- CR_WR_LOOP_MASK           : NPI_DMA Write XFER loop mask (continuous write)
 * -- CR_RD_LOOP_MASK           : NPI_DMA Read XFER loop mask (continuous read)
 * -- CR_XFER_TEST_MASK         : NPI_DMA Write XFER test mode mask (32bit counter put to memory)
 * -- CR_XFER_WR_START_MASK     : NPI_DMA Write XFER start transfer mask
 * -- CR_XFER_RD_START_MASK     : NPI_DMA Read XFER start transfer mask
 * -- CR_WR_BLOCK_MASK          : NPI_DMA Write XFER burst packet size mask
 * -- CR_RD_BLOCK_MASK          : NPI_DMA Read XFER burst packet size mask
 * -- CR_USE_RD_JUMP_MASK       : NPI_DMA transpose read flag mask
 * -- SR_WR_DONE_MASK           : NPI_DMA Write XFER transfer done mask
 * -- SR_RD_DONE_MASK           : NPI_DMA Read XFER transfer done mask
 * -- SR_XFER_STATUS_MASK       : NPI_DMA Write XFER transfer status mask (not implemented)
 */
#define XPS_NPI_DMA_CR_RST_MASK                       (0x00000001UL)
#define XPS_NPI_DMA_CR_WR_FIFO_RST_MASK               (0x00000002UL)
#define XPS_NPI_DMA_CR_RD_FIFO_RST_MASK               (0x00000004UL)
#define XPS_NPI_DMA_CR_WR_LOOP_MASK                   (0x00000008UL)
#define XPS_NPI_DMA_CR_RD_LOOP_MASK                   (0x00000010UL)
#define XPS_NPI_DMA_CR_XFER_TEST_MASK                 (0x00000020UL)
#define XPS_NPI_DMA_CR_XFER_WRITE_MASK                (0x00000040UL)
#define XPS_NPI_DMA_CR_XFER_READ_MASK                 (0x00000080UL)
#define XPS_NPI_DMA_CR_WR_BLOCK_MASK                  (0x00000F00UL)
#define XPS_NPI_DMA_CR_WR_BLOCK_SHIFT                 8
#define XPS_NPI_DMA_CR_RD_BLOCK_MASK                  (0x0000F000UL)
#define XPS_NPI_DMA_CR_RD_BLOCK_SHIFT                 12
#define XPS_NPI_DMA_CR_USE_RD_JUMP_MASK               (0x00010000UL)

#define XPS_NPI_DMA_SR_WR_DONE_MASK                 (0x00000001UL)
#define XPS_NPI_DMA_SR_RD_DONE_MASK                 (0x00000002UL)
#define XPS_NPI_DMA_SR_XFER_STATUS_MASK               (0x000000F0UL)
#define XPS_NPI_DMA_SR_XFER_STATUS_SHIFT               4

//function prototypes

void XPS_NPI_DMA_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Data); //internal function
void XPS_NPI_DMA_SetWrBurstSize(Xuint32 BaseAddress, Xuint8 Size); //internal function

void XPS_NPI_DMA_Reset(NPI_DMA_TYPE * DMA);
void XPS_NPI_DMA_WrFIFOrst(NPI_DMA_TYPE * DMA);
void XPS_NPI_DMA_RdFIFOrst(NPI_DMA_TYPE * DMA);
u8 XPS_NPI_DMA_WrDone(NPI_DMA_TYPE * DMA);
u8 XPS_NPI_DMA_RdDone(NPI_DMA_TYPE * DMA);
void XPS_NPI_DMA_WriteRAM(NPI_DMA_TYPE * DMA);
void XPS_NPI_DMA_TransposeRead(NPI_DMA_TYPE * DMA, u8 Enable);
void XPS_NPI_DMA_ReadRAM(NPI_DMA_TYPE * DMA);
Xuint32 XPS_NPI_DMA_WriteRAM_Blocking(NPI_DMA_TYPE * DMA);
Xuint32 XPS_NPI_DMA_ReadRAM_Blocking(NPI_DMA_TYPE * DMA);

/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 * -- SLV_REG2 : user logic slave module register 2
 * -- SLV_REG3 : user logic slave module register 3
 * -- SLV_REG4 : user logic slave module register 4
 * -- SLV_REG5 : user logic slave module register 5
 */
//#define XPS_NPI_DMA_USER_SLV_SPACE_OFFSET (0x00000000)
//#define XPS_NPI_DMA_SLV_REG0_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x00000000)
//#define XPS_NPI_DMA_SLV_REG1_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x00000004)
//#define XPS_NPI_DMA_SLV_REG2_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x00000008)
//#define XPS_NPI_DMA_SLV_REG3_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x0000000C)
//#define XPS_NPI_DMA_SLV_REG4_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x00000010)
//#define XPS_NPI_DMA_SLV_REG5_OFFSET (XPS_NPI_DMA_USER_SLV_SPACE_OFFSET + 0x00000014)

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
#define XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET (0x00000100)
#define XPS_NPI_DMA_INTR_DISR_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define XPS_NPI_DMA_INTR_DIPR_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define XPS_NPI_DMA_INTR_DIER_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define XPS_NPI_DMA_INTR_DIIR_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define XPS_NPI_DMA_INTR_DGIER_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define XPS_NPI_DMA_INTR_IPISR_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define XPS_NPI_DMA_INTR_IPIER_OFFSET (XPS_NPI_DMA_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

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
 * Write a value to a XPS_NPI_DMA register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void XPS_NPI_DMA_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define XPS_NPI_DMA_mWriteReg(BaseAddress, RegOffset, Data) \
 	XIo_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a XPS_NPI_DMA register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 XPS_NPI_DMA_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define XPS_NPI_DMA_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from XPS_NPI_DMA user logic slave registers.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void XPS_NPI_DMA_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 XPS_NPI_DMA_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
//#define XPS_NPI_DMA_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
//#define XPS_NPI_DMA_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))
//#define XPS_NPI_DMA_mWriteSlaveReg2(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG2_OFFSET) + (RegOffset), (Xuint32)(Value))
//#define XPS_NPI_DMA_mWriteSlaveReg3(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG3_OFFSET) + (RegOffset), (Xuint32)(Value))
//#define XPS_NPI_DMA_mWriteSlaveReg4(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG4_OFFSET) + (RegOffset), (Xuint32)(Value))
//#define XPS_NPI_DMA_mWriteSlaveReg5(BaseAddress, RegOffset, Value) \
// 	XIo_Out32((BaseAddress) + (XPS_NPI_DMA_SLV_REG5_OFFSET) + (RegOffset), (Xuint32)(Value))
//
//#define XPS_NPI_DMA_mReadSlaveReg0(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG0_OFFSET) + (RegOffset))
//#define XPS_NPI_DMA_mReadSlaveReg1(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG1_OFFSET) + (RegOffset))
//#define XPS_NPI_DMA_mReadSlaveReg2(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG2_OFFSET) + (RegOffset))
//#define XPS_NPI_DMA_mReadSlaveReg3(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG3_OFFSET) + (RegOffset))
//#define XPS_NPI_DMA_mReadSlaveReg4(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG4_OFFSET) + (RegOffset))
//#define XPS_NPI_DMA_mReadSlaveReg5(BaseAddress, RegOffset) \
// 	XIo_In32((BaseAddress) + (XPS_NPI_DMA_SLV_REG5_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Enable all possible interrupts from XPS_NPI_DMA device.
 *
 * @param   baseaddr_p is the base address of the XPS_NPI_DMA device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_NPI_DMA_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the XPS_NPI_DMA device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_NPI_DMA_Intr_DefaultHandler(void * baseaddr_p);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the XPS_NPI_DMA instance to be worked on.
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
XStatus XPS_NPI_DMA_SelfTest(void * baseaddr_p);

#endif // XPS_NPI_DMA_H
