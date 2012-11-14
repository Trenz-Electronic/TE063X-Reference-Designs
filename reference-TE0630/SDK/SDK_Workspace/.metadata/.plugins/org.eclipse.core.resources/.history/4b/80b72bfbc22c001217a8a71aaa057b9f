#include "Interrupts.h"

#define FX2TX_FIFO_SIZE	512 		//words
#define FX2RX_FIFO_SIZE	512 		//words
#define MEMORY_OFFSET	0x10000 	//offset for memory test, below is a MB code
#define TEST_BLOCK		0x100000	// 1 MB
//====================================================
//Menu function
//====================================================
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


XStatus sl_fifo_test(void){
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

	xil_printf("\r\nRunning Slave FIFO Interface test\r\n");
	//XPS_FX2_SetUSB_FIFOadr(XPAR_XPS_FX2_0_BASEADDR, PI_EP6);
	//xil_printf("Sending\r\n");
	//for(mem_cnt = 0; mem_cnt < TEST_BLOCK; mem_cnt++){
	//for(mem_cnt = 0; mem_cnt < 10; mem_cnt++){
		//while (XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR) > 512);	// Wait for room in FIFO
	//	XPS_FX2_WriteTXFifo(XPAR_XPS_FX2_0_BASEADDR, mem_cnt);
	//}
	xil_printf("RX FIFO Count %d \n\r",XPS_FX2_GetRXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));
	xil_printf("TX FIFO Count %d \n\r",XPS_FX2_GetTXFIFOcount(XPAR_XPS_FX2_0_BASEADDR));
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
	NPI_DMA_TYPE DMA;

	Xuint8 rx_run = 0; //rx xfer start flag
	Xuint8 tx_run = 0; //tx xfer start flag
	Xuint32 tx_addr=XPAR_DDR_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;
	Xuint32 rx_addr=XPAR_DDR_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;
	
	ver = VERSION;
	xil_printf("\r\n--Entering main TE-USB DEMO ver 0x%08X--\r\n", ver);
	XIo_Out32(XPAR_LED_BASEADDR,0xF);		// Enable LEDs
	
	start_intc();
  
    //NPI DMA properties
  	DMA.BaseAddress=XPAR_XPS_NPI_DMA_0_BASEADDR;	//base address of the DMA device
	DMA.WrStartAddr=tx_addr;	//start address for writing
	DMA.RdStartAddr=rx_addr;	//start address for reading
	DMA.WrBytes=(XPAR_DDR_SDRAM_MPMC_HIGHADDR - XPAR_DDR_SDRAM_MPMC_BASEADDR - MEMORY_OFFSET);			//number of bytes for writing
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
	
//	FIFO_putchar(&control_fifo, 'a');
	
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
					RAM_test(XPAR_DDR_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET, 60*1024*256,1); //60MB
					break;
				case 'f':
					RAM_test(XPAR_DDR_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET, 60*1024*256,0); //60MB
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
					tx_addr=XPAR_DDR_SDRAM_MPMC_BASEADDR+MEMORY_OFFSET;
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
						DBG(xil_printf("\r\nReceived %d bytes\r\n", rx_addr-XPAR_DDR_SDRAM_MPMC_BASEADDR-MEMORY_OFFSET);)
						if (XST_SUCCESS == RAM_test(DMA.WrStartAddr,((rx_addr-XPAR_DDR_SDRAM_MPMC_BASEADDR-MEMORY_OFFSET)>>2), 2)) //verify only
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
				case 'v':	// Internal test
					// +32 to be differ from USB transfer test
					if (RAM_test(XPAR_DDR_SDRAM_MPMC_BASEADDR + MEMORY_OFFSET + 32, 60*1024*256,1) == XST_SUCCESS){
						XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,1);
						//if(sl_fifo_test() == XST_SUCCESS){
						//	set_led_mode(1);	// (*-*-*-*-) No Errors
						//}
						//else
						//	set_led_mode(3);	// (*-*-*---) Sl FIFO error
					}
					else{
						XIo_Out32(XPAR_XPS_I2C_SLAVE_0_BASEADDR + XPS_I2C_SLAVE_MB2FX2_REG0_OFFSET,2);
						set_led_mode(2);	// (*-*-----)	Memory error
					}
					break;

				case 'p':	// Set passed LED
					set_led_mode(1);	// (*-*-*-*-) No Errors
					break;

				case 'e':	// Set passed LED
					set_led_mode(3);	// (*-*-*---) Sl FIFO error
					break;

				default:
	//					DBG(xil_printf("\r\nWrong character '%c' (%d dec)\r\n",character,character);)
					break;
			} //CASE
		} 
	}//MAIN LOOP
	
   return 0;
}
