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

#include "Interrupts.h"

Xuint8 get_board_revision(void);	// We use this from main.c
//====================================================
//Interrupt controller inicialization function
//====================================================
//FIFO_TYPE* intcontrol_fifo;

void start_intc(void) {
  
  // Setting Interupt controller
    xil_printf("\r\nSetting up Interrupt Controller:\r\n");

  //Initialize exception handling
    xil_printf("     Initialize exception handling\r\n");
  microblaze_enable_exceptions();
  
  // Register external interrupt handler
    xil_printf("     Register external interrupt handler\r\n");
  microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
                            (void *)XPAR_XPS_INTC_0_DEVICE_ID);
 
//  // Register UART interrupt handler
//  //  xil_printf("     Register UART interrupt handler\r\n");
//  XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR,XPAR_INTC_0_RS232_INTERRUPT_INTR,
//      (XInterruptHandler)uart_int_handler,(void *)XPAR_RS232_BASEADDR);

  // Register OPB_FX2 interrupt handler
    xil_printf("     Register I2C_SLAVE interrupt handler\r\n");
  XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR,XPAR_XPS_INTC_0_XPS_I2C_SLAVE_0_IP2INTC_IRPT_INTR,
      (XInterruptHandler)i2c_slave_int_handler,(void *)XPAR_XPS_I2C_SLAVE_0_BASEADDR);

  // Enable timer interrupts
//  //  xil_printf("     Register OPB_TIMER interrupt handler\r\n");
//	XIntc_mMasterEnable(XPAR_OPB_TIMER_0_BASEADDR);  // Start the interrupt controller
//	XIntc_SetIntrSvcOption( XPAR_INTC_0_BASEADDR, XIN_SVC_ALL_ISRS_OPTION);

  // Enable uart interrupt in the interrupt controller
    xil_printf("     Enable interrupts in the interrupt controller\r\n");
  XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
//                    XPAR_RS232_INTERRUPT_MASK | 
//							XPAR_XPS_TIMER_0_INTERRUPT_MASK |
//							XPAR_OPB_FX2_0_INTERRUPT_MASK |
						  XPAR_XPS_I2C_SLAVE_0_IP2INTC_IRPT_MASK);

    xil_printf("     Start the interrupt controller\r\n");
  XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

  // Enable uart interrupts
//  //  xil_printf("     Enable uart interrupt in Uartlite\r\n");
//  XUartLite_mEnableIntr(XPAR_RS232_BASEADDR);
  
//  xil_printf("     Enable all interrupts in XPS_FX2\r\n");
					
  // Enable MB interrupts
  //  xil_printf("     Enable MB interrupts\r\n");
  microblaze_enable_interrupts();
}


//====================================================
//FX2 interrupt handler
//====================================================

void i2c_slave_int_handler(void *baseaddr_p) {
	
	Xuint32 BaseAddress = (Xuint32)baseaddr_p;
	Xuint32 reg0, reg1, reg2;

	DBGINT(xil_printf("\r\nCommand from host PC received!\r\n");)
	reg0=XIo_In32(BaseAddress + XPS_I2C_SLAVE_FX2MB_REG0_OFFSET); //read control reg0
	DBGINT(xil_printf(" FX22MB_REG0 = 0x%08X\r\n", reg0);)
	reg1=XIo_In32(BaseAddress + XPS_I2C_SLAVE_FX2MB_REG1_OFFSET); //read control reg1
	DBGINT(xil_printf(" FX22MB_REG1 = 0x%08X\r\n", reg1);)
	reg2=XIo_In32(BaseAddress + XPS_I2C_SLAVE_FX2MB_REG2_OFFSET); //read control reg2
	DBGINT(xil_printf(" FX22MB_REG2 = 0x%08X\r\n", reg2);)

	switch (reg0)
	{
		//device
		case FX22MB_REG0_NOP :
		{
			//no opearation
			break;
		}
		case FX22MB_REG0_GETVERSION :
		{
			reg0 = VERSION;
			break;
		}         
		case FX22MB_REG0_START_TX : 
		{
			FIFO_putchar(&control_fifo, 't'); //to FIFO
			break;
		}
		case FX22MB_REG0_START_RX :
		{
			FIFO_putchar(&control_fifo, 'r'); //to FIFO
			break;
		}
		case FX22MB_REG0_STOP :
		{
			FIFO_putchar(&control_fifo, 's');//to FIFO
			break;
		}
		case FX22MB_REG0_PING :
		{
			reg0 = 0x706F6E67;//'pong'
			break;
		}
		case FX22MB_REG0_TEST :
		{
			reg0 = 0;
			FIFO_putchar(&control_fifo, 'v'); //to FIFO
			break;
		}
		case FX22MB_REG0_PASSED :
		{
			reg0 = 0;
			FIFO_putchar(&control_fifo, 'p'); //to FIFO
			break;
		}
		case FX22MB_REG0_ERROR :
		{
			reg0 = 0;
			FIFO_putchar(&control_fifo, 'e'); //to FIFO
			break;
		}
		case FX22MB_REG0_GET_REVISION :
			reg0 = get_board_revision();
			break;
	} //END SWITCH
	
	XIo_Out32(BaseAddress + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,reg0); //write status reg0
	XIo_Out32(BaseAddress + XPS_I2C_SLAVE_MB2FX2_REG1_OFFSET,reg1); //write status reg1
	XIo_Out32(BaseAddress + XPS_I2C_SLAVE_MB2FX2_REG2_OFFSET,reg2); //write status reg2	
}

