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
#include "xgpio.h"
//=============================================================================
#define FX2TX_FIFO_SIZE	512 		//words
#define FX2RX_FIFO_SIZE	512 		//words
#define MEMORY_OFFSET	0x10000 	//offset for memory test, below is a MB code
#define TEST_BLOCK		0x100000	// 1 MB
//=============================================================================
// GPIO
XGpio Gpio;
#define GPIO_LEDS 			0x0F
#define GPIO_RST_EN			0x0010
#define GPIO_S1C			0x0001
#define GPIO_S1D			0x0002
#define GPIO_AV				0x003C
#define GPIO_BR				0x03C0
#define GPIO_RST			0x0400
#define GPIO_OUT_CHANNEL	1
#define GPIO_IN_CHANNEL		2

void set_led_mode(Xuint8 mode, Xuint32 leds);

//-----------------------------------------------------------------------------
//	Read Board Revision and Assembly Variant
//-----------------------------------------------------------------------------
Xuint8 get_board_revision(void){
	Xuint32 gpio_data;
	gpio_data = XGpio_DiscreteRead(&Gpio, GPIO_IN_CHANNEL);
	return (Xuint8)((gpio_data & (GPIO_BR | GPIO_AV)) >> 2);
}
//-----------------------------------------------------------------------------
//	Menu function
//-----------------------------------------------------------------------------
void menu(void) {
	xil_printf("\r\nType:\r\n");
	xil_printf("    'a' RAM test\r\n");
	xil_printf("    'f' RAM Ftest\r\n");
	xil_printf("    'c' toggles caching\r\n");
	xil_printf("    't' starts TX transmission\r\n");	
	xil_printf("    'r' starts RX transmission\r\n");	
	xil_printf("    's' stops all transmissions\r\n");
	xil_printf("    'v' for internal test\r\n");
	xil_printf("    'm' for the redraw menu\r\n");
}

//-----------------------------------------------------------------------------
//	Blink mode
//-----------------------------------------------------------------------------
void set_led_mode(Xuint8 mode, Xuint32 leds){
	Xuint32 pause = 700000;
	switch(mode){
		default:
		case 1:
			while (1){
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
			}
			break;
		case 2:
			while (1){
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause * 4);
			}
			break;
		case 3:
			while (1){
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, leds); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause);
				XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0); cycsleep(pause * 4);
			}
			break;
	}
}



XStatus standalone_test(void){
	Xuint32	test1w, test2w;

	xil_printf("\r\nRunning Standalone test\r\n");
	// Switch FX2 to test mode
	//XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG2_OFFSET, 0x11111111);
	//XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG1_OFFSET, 0x11111111);
	XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET, 0x11111111);
	// Show user that test started
	XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, GPIO_LEDS);	// Turn LEDs ON
	cycsleep(1500000);
	XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, 0);			// Turn LEDs OFF
/*
	XPS_FX2_SetUSB_FIFOadr(XPAR_XPS_FX2_0_BASEADDR, PI_EP6);	// Set EP
	xil_printf("Sending\r\n");
	for(mem_cnt = 0; mem_cnt < 10; mem_cnt++){
		//while (XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR) > 512);	// Wait for room in FIFO
		XPS_FX2_WriteTXFifo(XPAR_XPS_FX2_0_BASEADDR, mem_cnt);
	}
*/
	//xil_printf("RX FIFO Count %d \n\r",XPS_FX2_GetRXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));
	//xil_printf("TX FIFO Count %d \n\r",XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));

	test1w = XPS_FX2_ReadRXFifo(XPAR_XPS_FX2_0_BASEADDR);
	test2w = XPS_FX2_ReadRXFifo(XPAR_XPS_FX2_0_BASEADDR);



	if (RAM_test(XPAR_DDR3_SDRAM_MPMC_BASEADDR + MEMORY_OFFSET + 32, 60*1024*256,1) == XST_SUCCESS){
		if((test1w == 0x12345678) && (test2w == 0x90ABCDEF)){
			xil_printf("Test Passed\r\n");
			set_led_mode(1, GPIO_LEDS | GPIO_RST_EN);	// (*-*-*-*-) No Errors
		}
		else{
			xil_printf("FX2 SlaveFIFO interface test FAILED!\r\n");
			set_led_mode(3, GPIO_LEDS | GPIO_RST_EN);	// (*-*-----)	Memory error
		}
	}
	else{
		xil_printf("DDR Memory test FAILED!\r\n");
		set_led_mode(2, GPIO_LEDS | GPIO_RST_EN);	// (*-*-----)	Memory error
	}


	//NPI_DMA_TYPE TST_DMA;
	//Xuint32 rx_end_addr, rx_start_addr, tx_start_addr, mem_cnt, i;
	//Xuint32 pause = 10000000;

    //NPI DMA properties
	//rx_start_addr = XPAR_DDR_SDRAM_MPMC_BASEADDR + TEST_BLOCK * 2;
	//tx_start_addr = XPAR_DDR_SDRAM_MPMC_BASEADDR + TEST_BLOCK * 3;
	//TST_DMA.BaseAddress = XPAR_XPS_NPI_DMA_0_BASEADDR;	// base address of the DMA device
	//TST_DMA.WrStartAddr = tx_start_addr;	// start address for writing
	//TST_DMA.RdStartAddr = rx_start_addr;	// start address for reading
	//TST_DMA.WrBytes = TEST_BLOCK;			// number of bytes for writing
	//TST_DMA.RdBytes = 1024;					// number of bytes for reading
	//TST_DMA.WrBurstSize = 4;				// 128bytes write burst size (for detailed description see SetWrBurstSize)
	//TST_DMA.RdBurstSize = 3;				// 64bytes read burst size (for detailed description see SetRdBurstSize)
	//TST_DMA.WrLoop = 0;						// continous writing flag
	//TST_DMA.RdLoop = 0;						// continous reading flag

	//XPS_FX2_SetUSB_FIFOadr(XPAR_XPS_FX2_0_BASEADDR, PI_EP6);
	//xil_printf("Sending\r\n");
	//for(mem_cnt = 0; mem_cnt < TEST_BLOCK; mem_cnt++){
	//for(mem_cnt = 0; mem_cnt < 10; mem_cnt++){
		//while (XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR) > 512);	// Wait for room in FIFO
	//	XPS_FX2_WriteTXFifo(XPAR_XPS_FX2_0_BASEADDR, mem_cnt);
	//}
	//xil_printf("RX FIFO Count %d \n\r",XPS_FX2_GetRXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));
	//xil_printf("TX FIFO Count %d \n\r",XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));
	//	XIo_Out32((rx_start_addr + mem_cnt),mem_cnt);

	//xil_printf("Configuring DMA\r\n");

	//XPS_FX2_TXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
	//XPS_FX2_RXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
	//XPS_NPI_DMA_Reset(&TST_DMA);
	//XPS_NPI_DMA_WriteRAM(&TST_DMA); 		//non blocking call

	// Wait till transfer ends
	//cycsleep(pause);
	//rx_end_addr = XIo_In32(TST_DMA.BaseAddress + XPS_NPI_DMA_WCR_OFFSET); //gets number of transmitted bytes

	//xil_printf("Start address = 0x%04x \n\r",rx_start_addr);
	//xil_printf("Stop  address = 0x%04x \n\r",rx_end_addr);
	//xil_printf("%d bytes transfered\n\r", (rx_end_addr - rx_start_addr));
	//for(mem_cnt = 0; mem_cnt < TEST_BLOCK; mem_cnt++){
	//	if(XIo_In32(rx_start_addr + mem_cnt) != mem_cnt){
	//		xil_printf("Memory check error at Addr %d \r\n",mem_cnt);
	//		for(i = 0; i<10; i++){
	//			xil_printf("0x%08x: 0x%08x\r\n",(rx_start_addr + i),XIo_In32(rx_start_addr + i));
	//		}

	//		return XST_FAILURE;
	//	}
	//}
	xil_printf("Test Passed\r\n");
	return XST_SUCCESS;
}

//====================================================
//MAIN
//====================================================
int main (void) 
{	
	Xint16 character;
	Xint8  cache=1;
	Xuint32 ver;
	Xuint32 counter=0;
	Xuint16 tx_count;
	int Status;
	NPI_DMA_TYPE DMA;
	Xuint32 gpio_data;

	Xuint8 rx_run = 0; //rx xfer start flag
	Xuint8 tx_run = 0; //tx xfer start flag
	Xuint32 tx_addr=XPAR_DDR3_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;
	Xuint32 rx_addr=XPAR_DDR3_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;

	Status = XGpio_Initialize(&Gpio, XPAR_LED_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&Gpio, GPIO_OUT_CHANNEL, ~(GPIO_LEDS | GPIO_RST_EN));
	
	ver = VERSION;
	xil_printf("\r\n-- TE-USB DEMO ver 0x%08X--\r\n", ver);
	
	gpio_data = XGpio_DiscreteRead(&Gpio, GPIO_IN_CHANNEL);
	if(gpio_data & GPIO_RST)	// Button is pressed
		standalone_test();		// Run selftest
	else	// Button not pressed = Normal mode (Enable LEDs and reset button)
		XGpio_DiscreteWrite(&Gpio, GPIO_OUT_CHANNEL, GPIO_LEDS | GPIO_RST_EN);


	start_intc();
    //NPI DMA properties
  	DMA.BaseAddress=XPAR_XPS_NPI_DMA_0_BASEADDR;	//base address of the DMA device
	DMA.WrStartAddr=tx_addr;	//start address for writing
	DMA.RdStartAddr=rx_addr;	//start address for reading
	DMA.WrBytes=(XPAR_DDR3_SDRAM_MPMC_HIGHADDR - XPAR_DDR3_SDRAM_MPMC_BASEADDR - MEMORY_OFFSET);			//number of bytes for writing
	DMA.RdBytes=1024;			//number of bytes for reading
	DMA.WrBurstSize=4;	//128bytes write burst size (for detailed description see SetWrBurstSize)
	DMA.RdBurstSize=3;	//64bytes read burst size (for detailed description see SetRdBurstSize)
	DMA.WrLoop=0;			//continous writing flag
	DMA.RdLoop=0;			//continous reading flag
  

	//sets FX2 packet_end timeout to 800cycles = 10us
	XPS_FX2_PktendTimeout(XPAR_XPS_FX2_0_BASEADDR, 800);
	
	//initalize control fifo
	FIFO_ResetChar(&control_fifo);
	
	caching(cache); //cache setup - usually better off
	
	menu();
	
//====================================================
//MAIN LOOP
//====================================================

	while(1)
	{
		//TRANSFER DATA at readback
		if (tx_run==1) { //RAM readout
			tx_count = XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR);
			if (tx_count < FX2TX_FIFO_SIZE-(DMA.RdBytes>>2)) {
		//	if (tx_count == 0) {
				DMA.RdStartAddr=tx_addr;
				u32 ret;
				ret = XPS_NPI_DMA_ReadRAM_Blocking(&DMA);
				tx_addr = ret; //increment tx_address
				//xil_printf("\r\nRead addr: 0x%08X, ret 0x%08X\r\n", tx_addr, ret);
			}
		}

	
		//check the main control fifo
		check_control_fifo_overlow(&control_fifo);
		
		//check for received char
		if (1 != XUartLite_IsReceiveEmpty(STDIN_BASEADDRESS))
		{ 	
			character = (u8)XIo_In32(STDIN_BASEADDRESS + XUL_RX_FIFO_OFFSET); //receive char
			if (character != 255)  //character valid!
				FIFO_putchar(&control_fifo, character);
		}
		character = FIFO_getchar(&control_fifo);
		
		if (character > 1) 
		{
			switch (character) {
				case 'a':
					RAM_test(XPAR_DDR3_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET, 60*1024*256,1); //60MB
					break;
				case 'f':
					RAM_test(XPAR_DDR3_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET, 60*1024*256,0); //60MB
					break;
				case 'c':
					cache=~cache;
					caching(cache);
					break;
				case 't':
					XPS_FX2_TXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					XPS_FX2_RXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					XPS_NPI_DMA_Reset(&DMA);
					DBG(xil_printf("\r\nRunning TX test\r\n");)
					tx_run = 1;
					tx_addr=XPAR_DDR3_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;
					XPS_FX2_SetUSB_FIFOadr(XPAR_XPS_FX2_0_BASEADDR, PI_EP6);
					break;
				case 'r':
					XPS_FX2_TXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					XPS_FX2_RXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					XPS_NPI_DMA_Reset(&DMA);
					XPS_NPI_DMA_WriteRAM(&DMA); //non blocking call
					DBG(xil_printf("\r\nRunning RX test\r\n");)
					rx_run = 1;
					counter=0;
					break;
				case 's':
					if (rx_run==1) {
						rx_addr = XIo_In32(DMA.BaseAddress + XPS_NPI_DMA_WCR_OFFSET); //gets number of transmitted bytes
						DBG(xil_printf("\r\nrx_addr 0x%08X\r\n", rx_addr);)
						DBG(xil_printf("\r\nReceived %d bytes\r\n", rx_addr-XPAR_DDR3_SDRAM_MPMC_BASEADDR-MEMORY_OFFSET);)
						if (XST_SUCCESS == RAM_test(DMA.WrStartAddr,((rx_addr-XPAR_DDR3_SDRAM_MPMC_BASEADDR-MEMORY_OFFSET)>>2), 2)) //verify only
							XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,1); //write status sucess
						else
							XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,0); //write status failure
					}
					XPS_NPI_DMA_Reset(&DMA); //stop DMA
					tx_run = 0;
					rx_run = 0;
					//reset all FX2 fifos
					XPS_FX2_TXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					XPS_FX2_RXFIFOrst(XPAR_XPS_FX2_0_BASEADDR);
					DBG(xil_printf("\r\nALL Transfers canceled!");)
					break;
				case 'm' :
					menu();	
					break;
				case 'v':	// Internal Memory test
					// +32 to be differ from USB transfer test
					if (RAM_test(XPAR_DDR3_SDRAM_MPMC_BASEADDR + MEMORY_OFFSET + 32, 60*1024*256,1) == XST_SUCCESS){
						XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,1);
					}
					else{
						XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,2);
						set_led_mode(2, GPIO_LEDS | GPIO_RST_EN);	// (*-*-----)	Memory error
					}
					break;

				case 'p':	// Set passed LED
					set_led_mode(1, GPIO_LEDS | GPIO_RST_EN);	// (*-*-*-*-) No Errors
					break;

				case 'e':	// Set passed LED
					set_led_mode(3, GPIO_LEDS | GPIO_RST_EN);	// (*-*-*---) Sl FIFO error
					break;

				case 'u':	// Run selftest
					standalone_test();
					break;

				default:
	//					DBG(xil_printf("\r\nWrong character '%c' (%d dec)\r\n",character,character);)
					break;
			} //CASE
		} 
	}//MAIN LOOP
	
   return 0;
}
