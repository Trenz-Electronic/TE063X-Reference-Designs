//////////////////////////////////////////////////////////////////////////////
// Filename:          E:\DOKTORAT\Projekti\HSS4\EDK\HSS4_v0.1.0.0/drivers/xps_npi_dma_v1_00_a/src/xps_npi_dma.c
// Version:           1.00.a
// Description:       xps_npi_dma Driver Source File
// Date:              Thu Sep 11 16:59:40 2008 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "xps_npi_dma.h"

/************************** Function Definitions ***************************/

/*
 * Sets a bitwise value to XPS_NPI_DMA CR register using MASK.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   Mask is a bit mask.
 * @param   Value = 0 sets masked value to 0, otherwise to 1.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Value)
 *
 */
void XPS_NPI_DMA_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Data)
{
  XIo_Out32(BaseAddress, (XIo_In32(BaseAddress) | Mask) & (Data | ~Mask));
}

/*
 * Sets a write burst size value to XPS_NPI_DMA CRBS register.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   Size is a xfer block size.
 *
 * @return  None.
 *
 * @note     4 bytes when  block_size = X"0" C_NPI_DATA_WIDTH = 32 1 word xfer
						8 bytes when  block_size = X"0" C_NPI_DATA_WIDTH = 64 2 words xfer
						16 bytes when block_size = X"1" 4-word cache-
						32 bytes when block_size = X"2" 8-word cache-
						64 bytes when block_size = X"3" 16-word burst
						128 bytes when block_size = X"4" 32-word burst
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_SetWrBurstSize(Xuint32 BaseAddress, Xuint8 Size)
 *
 */
void XPS_NPI_DMA_SetWrBurstSize(Xuint32 BaseAddress, Xuint8 Size)
{
  XPS_NPI_DMA_SetCR(BaseAddress, XPS_NPI_DMA_CR_WR_BLOCK_MASK, Size << XPS_NPI_DMA_CR_WR_BLOCK_SHIFT);
}

/*
 * Sets a read burst size value to XPS_NPI_DMA CRBS register.
 *
 * @param   BaseAddress is the base address of the XPS_NPI_DMA device.
 * @param   Size is a xfer block size.
 *
 * @return  None.
 *
 * @note     4 bytes when  block_size = X"0" C_NPI_DATA_WIDTH = 32 1 word xfer
						8 bytes when  block_size = X"0" C_NPI_DATA_WIDTH = 64 2 words xfer
						16 bytes when block_size = X"1" 4-word cache-
						32 bytes when block_size = X"2" 8-word cache-
						64 bytes when block_size = X"3" 16-word burst
						128 bytes when block_size = X"4" 32-word burst
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_SetRdBurstSize(Xuint32 BaseAddress, Xuint8 Size)
 *
 */
void XPS_NPI_DMA_SetRdBurstSize(Xuint32 BaseAddress, Xuint8 Size)
{
  XPS_NPI_DMA_SetCR(BaseAddress, XPS_NPI_DMA_CR_RD_BLOCK_MASK, Size << XPS_NPI_DMA_CR_RD_BLOCK_SHIFT);
}

/*
 * Writes a value to a XPS_NPI_DMA CR.
 * This performs XPS_NPI_DMA reset.
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_Reset(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_Reset(NPI_DMA_TYPE * DMA)
{
	Xuint8 i;
	for (i=0; i< 0x18; i+=4)
		XIo_Out32(DMA->BaseAddress+i, 0); //clear all registers
	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_WR_FIFO_RST_MASK |
																			XPS_NPI_DMA_CR_RD_FIFO_RST_MASK	|
																			XPS_NPI_DMA_CR_RST_MASK, 0xFFFFFFFF);
  XIo_Out32(DMA->BaseAddress, 0); //reset CR
}

/*
 * Writes a value to a XPS_NPI_DMA CR.
 * This performs XPS_NPI_DMA Write FIFO reset.
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_WrFIFOrst(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_WrFIFOrst(NPI_DMA_TYPE * DMA)
{
  XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_WR_FIFO_RST_MASK, 0xFFFFFFFF);
  XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_WR_FIFO_RST_MASK, 0);
}

/*
 * Writes a value to a XPS_NPI_DMA CR.
 * This performs XPS_NPI_DMA Read FIFO reset.
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  None.
 *
 * @note    None.
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_RdFIFOrst(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_RdFIFOrst(NPI_DMA_TYPE * DMA)
{
  XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_RD_FIFO_RST_MASK, 0xFFFFFFFF);
  XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_RD_FIFO_RST_MASK, 0);
}

/*
 * Checks XPS_NPI_DMA SR for done bit
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  0(busy) or 1(done)
 *
 * @note    None.
 *
 * C-style signature:
 * 	u8 XPS_NPI_DMA_WrDone(NPI_DMA_TYPE * DMA)
 *
 */

u8 XPS_NPI_DMA_WrDone(NPI_DMA_TYPE * DMA)
{
  if (XPS_NPI_DMA_SR_WR_DONE_MASK == (XPS_NPI_DMA_SR_WR_DONE_MASK & XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_SR_OFFSET)))  //checking busy
		return 1;
	else
		return 0; 
}

/*
 * Checks XPS_NPI_DMA SR for done bit
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  0(busy) or 1(done)
 *
 * @note    None.
 *
 * C-style signature:
 * 	u8 XPS_NPI_DMA_RdDone(NPI_DMA_TYPE * DMA)
 *
 */

u8 XPS_NPI_DMA_RdDone(NPI_DMA_TYPE * DMA)
{
  if (XPS_NPI_DMA_SR_RD_DONE_MASK == (XPS_NPI_DMA_SR_RD_DONE_MASK & XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_SR_OFFSET)))  //checking busy
		return 1;
	else
		return 0; 
}

/*
 *
 * Function to start a data write to DDR
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  none
 *
 * @note   
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_WriteRAM(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_WriteRAM(NPI_DMA_TYPE * DMA)
{
	if (DMA->WrLoop) //clear transfer mask
  	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_WR_LOOP_MASK | XPS_NPI_DMA_CR_XFER_WRITE_MASK, XPS_NPI_DMA_CR_WR_LOOP_MASK);
  else
  	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_WR_LOOP_MASK | XPS_NPI_DMA_CR_XFER_WRITE_MASK, 0);
  	
	XIo_Out32(DMA->BaseAddress + XPS_NPI_DMA_WSA_OFFSET, DMA->WrStartAddr); //set transfer initial address
	XIo_Out32(DMA->BaseAddress + XPS_NPI_DMA_WBR_OFFSET, DMA->WrBytes); //set transfer bytes
	XPS_NPI_DMA_SetWrBurstSize(DMA->BaseAddress, DMA->WrBurstSize); 	//set burst size	
	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_XFER_WRITE_MASK, 0xFFFFFFFF);  //start transfer
}

/**
 *
 * Function to setup tranpose data read from DDR
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 * @param   Enable is a flag to enable(1) or disable(0) transpose.
 *
 * @return 	none
 *
 * @note    4 bytes when  C_NPI_DATA_WIDTH = 32 1 word xfer
 *					8 bytes when  C_NPI_DATA_WIDTH = 64 2 words xfer
 *					set DMA->RdBurstSize=0!!!
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_ReadRAM(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_TransposeRead(NPI_DMA_TYPE * DMA, u8 Enable)
{
	if (Enable) {
		XIo_Out16(DMA->BaseAddress+XPS_NPI_DMA_RJR_OFFSET, DMA->RdJumpBytes); //set jump address
		XIo_Out16(DMA->BaseAddress+XPS_NPI_DMA_RJR_OFFSET+2, DMA->RdRows-1); //set rows
		XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_USE_RD_JUMP_MASK, 0xFFFFFFFF);  //enable flag
	} else {
		XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_USE_RD_JUMP_MASK, 0);  //disable flag
	}
}
/**
 *
 * Function to start a data read from DDR
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return 	none
 *
 *
 * C-style signature:
 * 	void XPS_NPI_DMA_ReadRAM(NPI_DMA_TYPE * DMA)
 *
 */

void XPS_NPI_DMA_ReadRAM(NPI_DMA_TYPE * DMA)
{
	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_XFER_READ_MASK, 0);  //clear transfer mask
	XIo_Out32(DMA->BaseAddress + XPS_NPI_DMA_RSA_OFFSET, DMA->RdStartAddr); //set transfer initial address
	XIo_Out32(DMA->BaseAddress + XPS_NPI_DMA_RBR_OFFSET, DMA->RdBytes); //set transfer bytes
	XPS_NPI_DMA_SetRdBurstSize(DMA->BaseAddress, DMA->RdBurstSize); 	//set burst size
	XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_XFER_READ_MASK, 0xFFFFFFFF);  //start transfer
}

/*
 *
 * Function to data write to DDR
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  Number of transferred bytes.
 *
 * @note
 *
 * C-style signature:
 * 	Xuint32 XPS_NPI_DMA_WriteRAM_Blocking(NPI_DMA_TYPE * DMA)
 *
 */

Xuint32 XPS_NPI_DMA_WriteRAM_Blocking(NPI_DMA_TYPE * DMA)
{
  Xuint32 status, timeout=0;
  if (1==XPS_NPI_DMA_WrDone(DMA))  //checking busy
  {
  	XPS_NPI_DMA_WriteRAM(DMA);
		do {
			timeout++;
/*			if (timeout>1000000){
				xil_printf("NPI DMA timeout\r\n");
				break;
			}*/
		} while (0==XPS_NPI_DMA_WrDone(DMA)); //waiting for done = 1
		status = XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_WCR_OFFSET);
		XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_XFER_WRITE_MASK, 0); //end transfer
		//status = XPS_NPI_DMA_SR_XFER_STATUS_MASK & XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_SR_OFFSET);
    //status = status>>4;
    return status;
  }
  else
  {
		return 0;//ERROR: XFER module is busy
	}
}


/**
 *
 * Function to read data from DDR
 *
 * @param   DMA is the base address of the XPS_NPI_DMA structure.
 *
 * @return  Number of transferred bytes.
 *
 * @note    4 bytes when  C_NPI_DATA_WIDTH = 32 1 word xfer
						8 bytes when  C_NPI_DATA_WIDTH = 64 2 words xfer
 *
 * C-style signature:
 * 	Xuint32 XPS_NPI_DMA_ReadRAM_Blocking(NPI_DMA_TYPE * DMA)
 *
 */

Xuint32 XPS_NPI_DMA_ReadRAM_Blocking(NPI_DMA_TYPE * DMA)
{
  Xuint32 status, timeout=0;
  if (1==XPS_NPI_DMA_RdDone(DMA))  //checking busy
  {
		XPS_NPI_DMA_ReadRAM(DMA);
		do {
			timeout++;
/*			if (timeout>1000000){
				xil_printf("NPI DMA timeout\r\n");
				break;
			}*/
		} while (0==XPS_NPI_DMA_RdDone(DMA)); //waiting for done = 1
		status = XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_RCR_OFFSET);
		XPS_NPI_DMA_SetCR(DMA->BaseAddress, XPS_NPI_DMA_CR_XFER_READ_MASK, 0); //end transfer
		//status = XPS_NPI_DMA_SR_XFER_STATUS_MASK & XIo_In32(DMA->BaseAddress + XPS_NPI_DMA_SR_OFFSET);
    //status = status>>4;
    return status;
  }
  else
  {
		return 0;//ERROR: XFER module is busy
	}
}

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
void XPS_NPI_DMA_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  XPS_NPI_DMA_mWriteReg(baseaddr, XPS_NPI_DMA_INTR_IPIER_OFFSET, 0x000000FF);

  /*
   * Enable all possible interrupt sources from device.
   */
  XPS_NPI_DMA_mWriteReg(baseaddr, XPS_NPI_DMA_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  XPS_NPI_DMA_mWriteReg(baseaddr, XPS_NPI_DMA_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for XPS_NPI_DMA device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the XPS_NPI_DMA device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_NPI_DMA_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = XPS_NPI_DMA_mReadReg(baseaddr, XPS_NPI_DMA_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = XPS_NPI_DMA_mReadReg(baseaddr, XPS_NPI_DMA_INTR_IPISR_OFFSET);
    XPS_NPI_DMA_mWriteReg(baseaddr, XPS_NPI_DMA_INTR_IPISR_OFFSET, IpStatus);
  }

}

