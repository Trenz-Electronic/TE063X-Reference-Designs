#include "func.h"

Xuint32 timeout_count; //global timeout counter

void cycsleep(Xuint32 Cycles){
  for(Cycles=Cycles; Cycles>0 ;Cycles--);
}

//====================================================
//DDR RAM test function mode;0-Ftest, 1-counter test, 2-verify only
//====================================================
XStatus RAM_test(Xuint32 Start_addr, Xuint32 Words, Xuint8 Mode) {
  Xuint32 i, k, data;
  Xuint32 errors=0;
  k=0;
  if (Words == 0) {
	 xil_printf("Nothing to test!\r\n");
    return XST_FAILURE;
	}
  Words--; //korekcija
  xil_printf("Performing RAM test: from addr 0x%08X to 0x%08X...", Start_addr, Start_addr+4*Words);
  //prewriting RAM
  if (Mode<2) {
	  for (i=Start_addr; i<=Start_addr+(Words<<2) ;i+=4) {
		 XIo_Out32(i,0xFFFFFFFF);
		 k++;
	  }
	  xil_printf("written...");
	  k=0;
  }
  if (Mode==1){ //Counter only
	  //writing RAM
	  for (i=Start_addr; i<=Start_addr+(Words<<2) ;i+=4) {
		 XIo_Out32(i,k);
	//    xil_printf("\r\nWriting Data %04d: addr 0x%08X written 0x%08X", k, i, k);
		 k++;
	  }
	  xil_printf("counted...");
  }
	k=0;
  //reading RAM
  for (i=Start_addr; i<=Start_addr+(Words<<2) ;i+=4) {
    data=XIo_In32(i);
//	 if (Mode==2) xil_printf("\r\nAddr 0x%08X = 0x%08X",i, data);
    if (k != data && Mode>0) { //Counter
      errors++;
      if (errors<10) xil_printf("\r\nData mismatch %04d: addr 0x%08X is 0x%08X, should be 0x%08X!", k, i, data, k);
    } else { if (0xFFFFFFFF != data && Mode==0){ //F-test
			errors++;
			if (errors<10) xil_printf("\r\nData mismatch %04d: addr 0x%08X is 0x%08X, should be 0xFFFFFFFF!", k, i, data);
		}
	 } 
    k++;
  }
  if (errors==0) {
    xil_printf("PASSED\r\n");
    return XST_SUCCESS;
  }
  else {
    xil_printf("FAILED\r\n");
    return XST_FAILURE;
  }
}

//====================================================
// checks for control fifo overflow
//====================================================

void check_control_fifo_overlow(FIFO_TYPE* ctrl_fifo) {
	if (ctrl_fifo->FIFO_count > CONTROL_FIFO_SIZE-10)
	{
		FIFO_ResetChar(ctrl_fifo);
		xil_printf("\r\n-- CONTROL FIFO FULL --\r\n");
	}
}


//====================================================
// enables/disables cache
//====================================================
void caching(Xuint8 Enable) {
	if (Enable) {
		  /*
		 * Enable and initialize cache
		 */
		#if XPAR_MICROBLAZE_0_USE_ICACHE
			xil_printf("Enabling and initializing instruction cache\r\n");
			microblaze_enable_icache();
		#endif
	
		#if XPAR_MICROBLAZE_0_USE_DCACHE	
			xil_printf("Enabling and initializing data cache\r\n");	
			microblaze_enable_dcache();
		#endif
	} else {
		#if XPAR_MICROBLAZE_0_USE_DCACHE
			xil_printf("Disabling data cache\r\n");
			microblaze_disable_dcache();
		#endif
	
		#if XPAR_MICROBLAZE_0_USE_ICACHE
			xil_printf("Disabling instruction cache\r\n");
			microblaze_disable_icache();
		#endif
	}
}

