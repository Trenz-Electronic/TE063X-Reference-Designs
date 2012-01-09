//////////////////////////////////////////////////////////////////////////////
// Filename:          E:\DOKTORAT\Projekti\HSS3\EDK\MyProcessorIPLib/drivers/xps_fx2_v1_00_a/src/xps_fx2.c
// Version:           1.00.a
// Description:       xps_fx2 Driver Source File
// Date:              Thu Apr 10 23:54:23 2008 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "xps_fx2.h"

/************************** Function Definitions ***************************/


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
void XPS_FX2_SetCR(Xuint32 BaseAddress, Xuint32 Mask, Xuint32 Data)
{
  XIo_Out32(BaseAddress+XPS_FX2_CR_OFFSET, 
				(XIo_In32(BaseAddress+XPS_FX2_CR_OFFSET) | Mask) & 
				(Data | ~Mask));
}


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

void XPS_FX2_TXFIFO_Ready(Xuint32 BaseAddress, Xuint8 Enable)
{
	if (Enable)
  	XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_RX_FIFO_RDY_MASK, 0xFFFFFFFF);
  else
  	XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_RX_FIFO_RDY_MASK, 0);
}

/*
 * Writes a value to a XPS_FX2 CR register bit 31.
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

void XPS_FX2_TXFIFOrst(Xuint32 BaseAddress)
{
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_TX_FIFO_RST_MASK, 0xFFFFFFFF);
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_TX_FIFO_RST_MASK, 0);
}

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

void XPS_FX2_RXFIFOrst(Xuint32 BaseAddress)
{
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_RX_FIFO_RST_MASK, 0xFFFFFFFF);
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_RX_FIFO_RST_MASK, 0);
}


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

void XPS_FX2_SetUSB_FIFOadr(Xuint32 BaseAddress, enum PI_PipeNumber PipeNo)
{
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_USB_FIFOADR_MASK, (PipeNo<<XPS_FX2_CR_USB_FIFOADR_SHIFT));
}

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

void XPS_FX2_PktendTimeout(Xuint32 BaseAddress, Xuint16 Timeout)
{
  XPS_FX2_SetCR(BaseAddress, XPS_FX2_CR_PKTEND_TIMEOUT_MASK, (Timeout<<XPS_FX2_CR_PKTEND_TIMEOUT_SHIFT));
}

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

void XPS_FX2_SetTXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold)
{
	XPS_FX2_SetCR(BaseAddress, XPS_FX2_FTR_TX_FIFO_THRESHOLD_MASK, Threshold<<XPS_FX2_FTR_TX_FIFO_THRESHOLD_SHIFT);
}

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

void XPS_FX2_SetRXFifoThreshold(Xuint32 BaseAddress, Xuint16 Threshold)
{
	XPS_FX2_SetCR(BaseAddress, XPS_FX2_FTR_RX_FIFO_THRESHOLD_MASK, Threshold<<XPS_FX2_FTR_RX_FIFO_THRESHOLD_SHIFT);
}

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

Xuint16 XPS_FX2_GetTXFIFOcount(Xuint32 BaseAddress)
{
  return (Xuint16)((XIo_In32(BaseAddress+XPS_FX2_SR_OFFSET)&XPS_FX2_SR_TX_FIFO_THRESHOLD_MASK)>>XPS_FX2_SR_TX_FIFO_THRESHOLD_SHIFT);
}

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

Xuint16 XPS_FX2_GetRXFIFOcount(Xuint32 BaseAddress)
{
  return (Xuint16)((XIo_In32(BaseAddress+XPS_FX2_SR_OFFSET)&XPS_FX2_SR_RX_FIFO_THRESHOLD_MASK)>>XPS_FX2_SR_RX_FIFO_THRESHOLD_SHIFT);
}

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

void XPS_FX2_WriteTXFifo(Xuint32 BaseAddress, Xuint32 Data)
{
	XIo_Out32(BaseAddress+XPS_FX2_TX_OFFSET,Data);
}

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

Xuint32 XPS_FX2_ReadRXFifo(Xuint32 BaseAddress)
{
  return XIo_In32(BaseAddress+XPS_FX2_RX_OFFSET);
}

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
void XPS_FX2_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  XPS_FX2_mWriteReg(baseaddr, XPS_FX2_INTR_IPIER_OFFSET, 0x000000FF);

  /*
   * Enable all possible interrupt sources from device.
   */
  XPS_FX2_mWriteReg(baseaddr, XPS_FX2_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  XPS_FX2_mWriteReg(baseaddr, XPS_FX2_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for XPS_FX2 device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the XPS_FX2 device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void XPS_FX2_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = XPS_FX2_mReadReg(baseaddr, XPS_FX2_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = XPS_FX2_mReadReg(baseaddr, XPS_FX2_INTR_IPISR_OFFSET);
    XPS_FX2_mWriteReg(baseaddr, XPS_FX2_INTR_IPISR_OFFSET, IpStatus);
  }

}

