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
#include "xil_exception.h"
#include "version.h"
#include "fx2_iic.h"
#include "main.h"
#include "xiic.h"
#include "xiic_l.h"
#include "xintc.h"
#include "xgpio.h"
#include "stdio.h"
#include "xllfifo.h"
#include "math.h"
//-----------------------------------------------------------------------------
#define IIC_DEVICE_ID		XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID			XPAR_INTC_0_IIC_0_VEC_ID
#define FIFO_INTR_ID		XPAR_INTC_0_LLFIFO_0_VEC_ID
#define GPIO_DEVICE_ID  	XPAR_LEDS_DEVICE_ID

#define min(a, b) (((a) < (b)) ? (a) : (b))
//-----------------------------------------------------------------------------
// Functions prototypes
//-----------------------------------------------------------------------------
int I2CWriteData(u8 *BufferPtr, u16 ByteCount);
//int I2CReadData(u8 *BufferPtr, u16 ByteCount);
int I2CReadData(XIic* InstancePtr, u8 *BufferPtr, u16 ByteCount);
static int SetupInterruptSystem(XIic * IicInstPtr, XLlFifo * LLFifoInstPtr);
static void I2CStatusHandler(XIic *InstancePtr, int Event);
static void I2CSendHandler(XIic *InstancePtr);
static void I2CReceiveHandler(XIic *InstancePtr);
static void FIFOReceiveHandler(XLlFifo *InstancePtr);
int rx_test(XLlFifo* InstancePtr);
int tx_test(XLlFifo* InstancePtr);

u8 state = STATE_IDLE;
volatile u32 total_cnt,err_cnt;
//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
XIic IicInstance;			// The instance of the IIC device.
XIntc InterruptController;	// The instance of the Interrupt Controller
XGpio Gpio; 				// The Instance of the GPIO Driver
XLlFifo LLFifo;				// The instance of the LL FIFO
volatile u32 *Fx2_core	= (u32*)XPAR_AXI_FX2_0_BASEADDR;	// FX2 core base

// Buffers to access from interrupt context
u8 iic_receive_buffer[I2C_TRANSFER_COUNT];
u8 iic_transmit_buffer[I2C_TRANSFER_COUNT];
volatile u8 iic_receive_valid = 0;
volatile u8 iic_transmit_valid = 0;

u8 iic_int_buffer[I2C_TRANSFER_COUNT];	// Read buffer for internal use
u8 iic_read_count = 0;					// Read buffer counter
u8 I2CBuffer[I2C_TRANSFER_COUNT];	// Read/Write buffer for IIC operations
//u8 I2CWriteBuffer[I2C_TRANSFER_COUNT];	// Write buffer for IIC operations

//volatile u8 TransmitComplete = 1;
//volatile u8 ReceiveComplete = 1;
//volatile u8 ReceiveComplete = 0;

volatile u8 SlaveRead;
volatile u8 SlaveWrite;

u8 get_rx_test_status(void){
	// Wait for test to finish
	return (err_cnt == 0)? 1 : 0;
}
//-----------------------------------------------------------------------------
int main(){
	u32 Status;

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
	Status = SetupInterruptSystem(&IicInstance, &LLFifo);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize LL FIFO\r\n");
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

	XIic_WriteReg(IicInstance.BaseAddress, XIIC_RFD_REG_OFFSET,	I2C_TRANSFER_COUNT - 1);
	XIic_Start(&IicInstance);
	XIic_IntrGlobalEnable(IicInstance.BaseAddress);

	Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Failed to initialize GPIO\r\n");
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&Gpio, 1, 0x00);	// All outputs

	// Initialize FIFO
	XLlFifo_Initialize(&LLFifo, XPAR_AXI_FIFO_0_BASEADDR);
	XLlFifo_Reset(&LLFifo);
	XLlFifo_IntEnable(&LLFifo, XLLF_INT_RC_MASK);

	xil_printf("\r\n-- TE USB DEMO ver %d.%d --\r\n", MAJOR_VERSION,MINOR_VERSION);
	XGpio_DiscreteWrite(&Gpio, 1, 0x0F);	// Switch on LEDs

	while(1){	// Main loop
		if(iic_receive_valid == 1){
			iic_receive_valid = 0;
			switch(iic_receive_buffer[3]){
				case CMD_NOP:
					xil_printf("NOP\r\n");
				break;
				case CMD_GETVERSION:
					xil_printf("GETVERSION\r\n");
				break;
				case CMD_START_TX:
					xil_printf("Start TX\r\n");
				break;
				case CMD_START_RX:
					xil_printf("Start RX\r\n");
				break;
				case CMD_STOP:
					xil_printf("Stop\r\n");
					xil_printf("Received %d (%d errors)\r\n",total_cnt,err_cnt);
					iic_transmit_buffer[0] = 0;
					iic_transmit_buffer[1] = 0;
					iic_transmit_buffer[2] = 0;
					iic_transmit_buffer[3] = get_rx_test_status();
					XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF); // Activate FX2 interrupt
				break;
				case CMD_PING:
					xil_printf("Ping\r\n");
				break;
				default:
					xil_printf("Unknown command 0x%02X\n\r",iic_receive_buffer[3]);
			}
		}
	}
}

/******************************************************************************
* This function reads received USB data and check it
*
* @param	XLlFifo reference
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int rx_test(XLlFifo* RxInstance){
	u32 frame_len;
	u8 buffer[256];
	while (1){
	while (XLlFifo_RxOccupancy(RxInstance)) {
		frame_len = XLlFifo_RxGetLen(RxInstance);
		while (frame_len) {
			unsigned bytes = min(sizeof(buffer), frame_len);
			XLlFifo_Read(RxInstance, buffer, bytes);

			//xil_printf("*");
			// ********
			// do something with buffer here
			// ********
			frame_len -= bytes;
		}
	}
	}
	return XST_SUCCESS;
}

/******************************************************************************
* This function write data sequence to USB FIFO
*
* @param	XLlFifo reference
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int tx_test(XLlFifo* TxInstance){
	u32 frame_left, frame_len;
	u8 buffer[256];

 	frame_left = frame_len;
 	while (frame_left) {
 		unsigned bytes = min(sizeof(buffer), frame_left);
 		XLlFifo_Write(TxInstance, buffer, bytes);
 		// ********
 		// do something here to refill buffer
 		// ********
 		frame_left -= bytes;
 	}
 	XLlFifo_TxSetLen(TxInstance, frame_len);

 	return XST_SUCCESS;
}
/******************************************************************************
* This function reads a buffer of bytes  when the IIC Master on the bus writes
* data to the slave device.
*
* @param	IIC Instance pointer
* @param	BufferPtr contains the address of the data buffer to be filled.
* @param	ByteCount contains the number of bytes in the buffer to be read.
*
* @return	Count of received data bytes
*
* @note		None
*
******************************************************************************/
/*
int I2CReadData(XIic* InstancePtr, u8 *BufferPtr, u16 ByteCount)
{

	if((ReceiveComplete) || (XIic_IsIicBusy(InstancePtr) == TRUE)){
		if (SlaveRead) {
			XIic_SlaveRecv(InstancePtr, BufferPtr, ByteCount);
			SlaveRead = 0;
		}
		return 0;
	}
	else {
		ReceiveComplete = 1;
		return I2C_TRANSFER_COUNT;
	}

}
*/
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
/*
int I2CWriteData(u8 *BufferPtr, u16 ByteCount)
{
	int Status;

	TransmitComplete = 1;	// Set the defaults.

	Status = XIic_Start(&IicInstance);	// Start the IIC device.
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XIic_IntrGlobalEnable(IicInstance.BaseAddress);	// Set the Global Interrupt Enable.

	XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF); // Activate FX2 interrupt

	while ((TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == FALSE)) {
		if (SlaveWrite) {
			XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0x00); // Remove interrupt
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
*/
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
		XIic_SlaveRecv(InstancePtr, iic_receive_buffer, I2C_TRANSFER_COUNT);
	} else {	// Its a Read request from the master.
		SlaveWrite = 1;
		// Write prepared buffer
		XIic_WriteReg(InstancePtr->BaseAddress, XIIC_GPO_REG_OFFSET, 0x00); // Remove interrupt
		XIic_SlaveSend(InstancePtr, iic_transmit_buffer, I2C_TRANSFER_COUNT);
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
	//TransmitComplete = 0;
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
	//ReceiveComplete = 0;

	iic_receive_valid = 1;
	switch(iic_receive_buffer[3]){	// Analyze command
		case CMD_NOP:
		break;
		case CMD_GETVERSION:
			iic_transmit_buffer[0] = MAJOR_VERSION;
			iic_transmit_buffer[1] = MINOR_VERSION;
			iic_transmit_buffer[2] = RELEASE;
			iic_transmit_buffer[3] = BUILD;
			XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF); // Activate FX2 interrupt
		break;
		case CMD_START_TX:
		break;
		case CMD_START_RX:
			total_cnt = 0;	// Clear counter
			err_cnt = 0;	// Clear err_cnt
		break;
		case CMD_STOP:
		break;
		case CMD_PING:
			iic_transmit_buffer[0] = 0x70;	//p
			iic_transmit_buffer[1] = 0x6F;	//o
			iic_transmit_buffer[2] = 0x6E;	//n
			iic_transmit_buffer[3] = 0x67;	//g
			XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF); // Activate FX2 interrupt
		break;
		default:
			//xil_printf("Unknown packet\r\n");
			//xil_printf("%02X %02X %02X %02X\r\n",iic_receive_buffer[ 0],iic_receive_buffer[ 1],iic_receive_buffer[ 2],iic_receive_buffer[ 3]);
			//xil_printf("%02X %02X %02X %02X\r\n",iic_receive_buffer[ 4],iic_receive_buffer[ 5],iic_receive_buffer[ 6],iic_receive_buffer[ 7]);
			//xil_printf("%02X %02X %02X %02X\r\n",iic_receive_buffer[ 8],iic_receive_buffer[ 9],iic_receive_buffer[10],iic_receive_buffer[11]);
		break;
	}
	XIic_WriteReg(IicInstance.BaseAddress, XIIC_GPO_REG_OFFSET, 0xFF); // Activate FX2 interrupt
}

/******************************************************************************
* This Receive handler is called asynchronously from an interrupt
* context and indicates that data in the specified buffer has been Received.
*
* @param	InstancePtr is a pointer to the LLFIFO driver instance for which
* 		the handler is being called for.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void FIFOReceiveHandler(XLlFifo *InstancePtr){
	u32 frame_len, fifo_status;
	u8 buffer[2048];
	//int i;
	xil_printf("&");
	fifo_status = XLlFifo_Status(InstancePtr);	// Read status
	//if(fifo_status & XLLF_INT_RC_MASK){
	//	xil_printf("R");
	//}
	//else{
	//	xil_printf("0x%08x ",fifo_status);
	//}

	XLlFifo_IntClear(InstancePtr, XLLF_INT_ALL_MASK);	// Clear all

	while (XLlFifo_RxOccupancy(InstancePtr)) {
		frame_len = XLlFifo_RxGetLen(InstancePtr);
		while (frame_len) {
			unsigned bytes = min(sizeof(buffer), frame_len);
			XLlFifo_Read(InstancePtr, buffer, bytes);
			//xil_printf("&");

			//xil_printf("received %d bytes\n\r",bytes);
			//xil_printf("%d\n\r",frame_len);
			//for(i = 0;i<bytes;i+=4)
			//	xil_printf("0x%02X 0x%02X 0x%02X 0x%02X\n\r",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3]);

			//for(i = 0;i<bytes;i+=4){
			//	rx_int = (buffer[i]|(buffer[i+1]<<8)|(buffer[i+2]<<16)|(buffer[i+3]<<24));
				//xil_printf("0x%04X\n\r",rx_int);
				//xil_printf("0x%04X\n\r",total_cnt);
				//

			//	if(total_cnt != rx_int){
					//xil_printf("-");
					//xil_printf("0x%04X != 0x%04X\n\r",total_cnt,rx_int);
			//		err_cnt++;
			//	}
			//	total_cnt++;
			//}
			// ********
			// do something with buffer here
			// ********
			frame_len -= bytes;
			total_cnt++;
		}
	}
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
static int SetupInterruptSystem(XIic * IicInstPtr, XLlFifo * LLFifoInstPtr)
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

	Status = XIntc_Connect(&InterruptController, FIFO_INTR_ID,
				   (XInterruptHandler) FIFOReceiveHandler,
				   LLFifoInstPtr);
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

	// Enable the interrupts for the IIC and FIFO.
	XIntc_Enable(&InterruptController, IIC_INTR_ID);
	XIntc_Enable(&InterruptController, FIFO_INTR_ID);

	Xil_ExceptionInit();	// Initialize the exception table.

	// Register the interrupt controller handler with the exception table.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				 (Xil_ExceptionHandler) XIntc_InterruptHandler,
				 &InterruptController);

	Xil_ExceptionEnable();	// Enable non-critical exceptions.

	return XST_SUCCESS;
}

