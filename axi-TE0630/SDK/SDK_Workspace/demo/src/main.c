/*
Copyright (C) 2012 Trenz Electronic

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#include "xparameters.h"
#include "version.h"
#include "fx2_iic.h"
#include "main.h"
#include "xiic.h"
#include "xintc.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "stdio.h"
//-----------------------------------------------------------------------------
#define IIC_DEVICE_ID		XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID			XPAR_INTC_0_IIC_0_VEC_ID
#define GPIO_DEVICE_ID  	XPAR_LEDS_DEVICE_ID
//-----------------------------------------------------------------------------
// Functions prototypes
//-----------------------------------------------------------------------------
int I2CWriteData(u8 *BufferPtr, u16 ByteCount);
int I2CReadData(u8 *BufferPtr, u16 ByteCount);
static int SetupInterruptSystem(XIic * IicInstPtr);
static void I2CStatusHandler(XIic *InstancePtr, int Event);
static void I2CSendHandler(XIic *InstancePtr);
static void I2CReceiveHandler(XIic *InstancePtr);
//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
XIic IicInstance;			/* The instance of the IIC device. */
XIntc InterruptController;	/* The instance of the Interrupt Controller */
XGpio Gpio; 				/* The Instance of the GPIO Driver */


u8 WriteBuffer[I2C_TRANSFER_COUNT];	/* Write buffer for writing a page. */
u8 ReadBuffer[I2C_TRANSFER_COUNT];	/* Read buffer for reading a page. */

volatile u8 TransmitComplete;
volatile u8 ReceiveComplete;

volatile u8 SlaveRead;
volatile u8 SlaveWrite;

//-----------------------------------------------------------------------------
int main(){
	int Status;

	XIic_Config *ConfigPtr;	// Pointer to configuration data

	// Initialize the IIC driver so that it is ready to use.
	ConfigPtr = XIic_LookupConfig(IIC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		xil_printf("Failed to initialize IIC\r\n");
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&IicInstance, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize IIC\r\n");
		return XST_FAILURE;
	}

	//Setup the Interrupt System.
	Status = SetupInterruptSystem(&IicInstance);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize IIC\r\n");
		return XST_FAILURE;
	}

	XIic_SlaveInclude();	// Include slave functions

	// Set the Transmit, Receive and Status Handlers.
	XIic_SetStatusHandler(&IicInstance, &IicInstance, (XIic_StatusHandler) I2CStatusHandler);
	XIic_SetSendHandler(&IicInstance, &IicInstance, (XIic_Handler) I2CSendHandler);
	XIic_SetRecvHandler(&IicInstance, &IicInstance, (XIic_Handler) I2CReceiveHandler);

	// Set the Address as a RESPOND type.
	Status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_RESPOND_TYPE, I2C_SLAVE_ADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize IIC\r\n");
		return XST_FAILURE;
	}

	Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize GPIO\r\n");
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&Gpio, 1, 0x00);	// All outputs

	xil_printf("\r\n--TE USB DEMO ver %d.%d--\r\n", MAJOR_VERSION,MINOR_VERSION);
	XGpio_DiscreteWrite(&Gpio, 1, 0x0F);	// Switch on LEDs

	while(1){	// Main loop
		I2CReadData(ReadBuffer, I2C_TRANSFER_COUNT);	// Read data from IIC
		switch(ReadBuffer[3]){	// Analyze command
			case CMD_NOP:
				break;
			case CMD_GETVERSION:
				WriteBuffer[0] = MAJOR_VERSION;
				WriteBuffer[1] = MINOR_VERSION;
				WriteBuffer[2] = RELEASE;
				WriteBuffer[3] = BUILD;
				xil_printf("*");
				break;
			case CMD_START_TX:
				xil_printf("starting TX test\r\n");
				break;
			case CMD_START_RX:
				xil_printf("starting RX test\r\n");
				break;
			case CMD_STOP:
				xil_printf("stop test\r\n");
				break;
			case CMD_PING:
				WriteBuffer[0] = 0x70;	//p
				WriteBuffer[1] = 0x6F;	//o
				WriteBuffer[2] = 0x6E;	//n
				WriteBuffer[3] = 0x67;	//g
				break;
		}
		I2CWriteData(WriteBuffer,I2C_TRANSFER_COUNT);	// Send response
		while(TransmitComplete) {};	// Wait while transfer completed
	}
}

/******************************************************************************
* This function reads a buffer of bytes  when the IIC Master on the bus writes
* data to the slave device.
*
* @param	BufferPtr contains the address of the data buffer to be filled.
* @param	ByteCount contains the number of bytes in the buffer to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int I2CReadData(u8 *BufferPtr, u16 ByteCount)
{
	int Status;

	ReceiveComplete = 1;	//Set the defaults.

	Status = XIic_Start(&IicInstance);	// Start the IIC device.
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XIic_IntrGlobalEnable(IicInstance.BaseAddress);	// Set the Global Interrupt Enable.

	while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {
		if (SlaveRead) {
			//XIic_SlaveRecv(&IicInstance, ReadBuffer, I2C_TRANSFER_COUNT);
			XIic_SlaveRecv(&IicInstance, BufferPtr, I2C_TRANSFER_COUNT);
			SlaveRead = 0;
		}
	}

	XIic_IntrGlobalDisable(IicInstance.BaseAddress);	// Disable the Global Interrupt Enable.

	Status = XIic_Stop(&IicInstance);	// Stop the IIC device.
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/******************************************************************************
* This function writes a buffer of bytes to the IIC bus when the IIC master
* initiates a read operation.
*
* @param	ByteCount contains the number of bytes in the buffer to be
*		written.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int I2CWriteData(u8 *BufferPtr, u16 ByteCount)
{
	int Status;

	TransmitComplete = 1;	// Set the defaults.

	Status = XIic_Start(&IicInstance);	// Start the IIC device.
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XIic_IntrGlobalEnable(IicInstance.BaseAddress);	// Set the Global Interrupt Enable.

	XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF);

	while ((TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == FALSE)) {
		if (SlaveWrite) {
			XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0x00);
			XIic_SlaveSend(&IicInstance, BufferPtr, ByteCount);
			SlaveWrite = 0;
			break;
		}
	}

	XIic_IntrGlobalDisable(IicInstance.BaseAddress);	// Disable the Global Interrupt Enable bit.

	Status = XIic_Stop(&IicInstance); // Stop the IIC device.
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/******************************************************************************
* This Status handler is called asynchronously from an interrupt context and
* indicates the events that have occurred.
*
* @param	InstancePtr is not used, but contains a pointer to the IIC
* 		device driver instance which the handler is being called for.
* @param	Event indicates whether it is a request for a write or read.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void I2CStatusHandler(XIic *InstancePtr, int Event)
{
	if (Event == XII_MASTER_WRITE_EVENT) {	// Its a Write request from Master.
		SlaveRead = 1;
	} else {	// Its a Read request from the master.
		SlaveWrite = 1;
	}
}

/******************************************************************************
* This Send handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been sent.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
*		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void I2CSendHandler(XIic *InstancePtr)
{
	TransmitComplete = 0;
}

/******************************************************************************
* This Receive handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been Received.
*
* @param	InstancePtr is a pointer to the IIC driver instance for which
* 		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void I2CReceiveHandler(XIic *InstancePtr)
{
	ReceiveComplete = 0;
}

/****************************************************************************/
/**
* This function setups the interrupt system so interrupts can occur for the
* IIC. The function is application-specific since the actual system may or
* may not have an interrupt controller. The IIC device could be directly
* connected to a processor without an interrupt controller. The user should
* modify this function to fit the application.
*
* @param	IicInstPtr contains a pointer to the instance of the IIC  which
*		is going to be connected to the interrupt controller.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static int SetupInterruptSystem(XIic * IicInstPtr)
{
	int Status;

	if (InterruptController.IsStarted == XIL_COMPONENT_IS_STARTED) {
		return XST_SUCCESS;
	}

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 */
	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above
	 * performs the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, IIC_INTR_ID,
				   (XInterruptHandler) XIic_InterruptHandler,
				   IicInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Enable the interrupts for the IIC device.
	XIntc_Enable(&InterruptController, IIC_INTR_ID);

	Xil_ExceptionInit();	// Initialize the exception table.

	// Register the interrupt controller handler with the exception table.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				 (Xil_ExceptionHandler) XIntc_InterruptHandler,
				 &InterruptController);

	Xil_ExceptionEnable();	// Enable non-critical exceptions.

	return XST_SUCCESS;
}

