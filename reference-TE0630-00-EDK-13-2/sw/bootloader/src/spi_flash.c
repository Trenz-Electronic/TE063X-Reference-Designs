#include "spi_flash.h"
#include "xparameters.h"
#include "xspi_l.h"

#define SPI_NONE_SELECT		0xFF
#define SPI_SELECT_0		0xFE
#define	READ				0x03 // Read Instruction
#define	DP					0xB9 // Deep Power-down

//***************************************************************************//
void SPI_Send (Xuint8 *pdata, Xuint32 uiNumBytes) {
	while (uiNumBytes>0) {
		XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, *pdata);
		while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
		XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
		pdata++;
		uiNumBytes--;
	}
}

void SPI_Receive (Xuint8 *pdata, Xuint32 uiNumBytes) {
	while (uiNumBytes>0) {
		XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, 0);
		while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
		*pdata = XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
		pdata++;
		uiNumBytes--;
	}
}

void SPI_CommandREAD (Xuint8 bySector, Xuint8 byPage, Xuint8 byByte) {
	// Initialize SPI Control Register
	XSpi_mSetControlReg (XPAR_SPI_FLASH_BASEADDR, XSP_CR_MANUAL_SS_MASK | XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK);
	
	// Deselect SPI devices
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_NONE_SELECT);
	
	// Select SPI device 0
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_SELECT_0);	
	
	// Send READ command
	XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, READ);
	// Wait transfer to finish
	while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
	// Empty receive queue
	XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
	// Send sector
	XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, bySector);
	// Wait transfer to finish
	while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
	// Empty receive queue
	XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
	// Send page
	XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, byPage);
	// Wait transfer to finish
	while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
	// Empty receive queue
	XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
	// Send address
	XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, byByte);
	// Wait transfer to finish
	while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
	// Empty receive queue
	XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);
}

void SPI_DeselectDevices (void) {
	// Deselect SPI devices
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_NONE_SELECT);
}

void SPI_DeepPowerDown (void) {
	// Initialize SPI Control Register
	XSpi_mSetControlReg (XPAR_SPI_FLASH_BASEADDR, XSP_CR_MANUAL_SS_MASK | XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK);

	// Deselect SPI devices
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_NONE_SELECT);
	
	// Select SPI device 0
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_SELECT_0);	
	
	// Send DP command
	XSpi_mSendByte (XPAR_SPI_FLASH_BASEADDR, DP);
	// Wait transfer to finish
	while ((XSpi_mGetStatusReg(XPAR_SPI_FLASH_BASEADDR) & XSP_SR_RX_EMPTY_MASK)!=0);
	// Empty receive queue
	XSpi_mRecvByte(XPAR_SPI_FLASH_BASEADDR);

	// Deselect SPI devices to end command
	XSpi_mSetSlaveSelectReg (XPAR_SPI_FLASH_BASEADDR,SPI_NONE_SELECT);
}
