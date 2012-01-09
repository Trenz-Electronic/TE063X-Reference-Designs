//Created by Costantino Grana - University of Modena and Reggio Emilia, Italy
//Edited by Ales Gorkic - Optomotive d.o.o., Slovenia

#include "xparameters.h"
#include "spi_flash.h"
#include "xio.h"
#include <string.h>

typedef struct tagBOOTDATAHEADER {
	unsigned int size;
	unsigned int boot_jump;
	unsigned short crc16;
	unsigned char vectors[40];
} BOOTDATAHEADER;

// function to calculate CRC-16
unsigned short crc16(void * data, unsigned int len)
{
	unsigned char * dataPtr = (unsigned char *)data;
	unsigned int index = 0;
	// Update the CRC for transmitted and received data using
	// the CCITT 16bit algorithm (X^16 + X^12 + X^5 + 1).
	unsigned short crc = 0;
	while(len--)
	{
		crc = (unsigned char)(crc >> 8) | (crc << 8);
		crc ^= dataPtr[index++];
		crc ^= (unsigned char)(crc & 0xff) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	}
	return crc;
}

//**************** CONSTANTS ************************************************//
#define M25P32_BOOTSECTOR		16	// Boot data starts at sector 16 (1MB from the begining of the flash)

BOOTDATAHEADER bdh;
Xuint8 *destination_address;

int (*boot_app) (void);

#define DEBUG
#ifdef DEBUG
	#define DBG(x) x
#else
	#define DBG(x)
#endif

//***************************************************************************//
int main (void) {
	Xuint32 i,j, l=0, LED=0;;

	// Disable cache
	#if XPAR_MICROBLAZE_0_USE_ICACHE
		microblaze_disable_icache ();
	#endif
	#if XPAR_MICROBLAZE_0_USE_DCACHE
		microblaze_disable_dcache ();
	#endif
		
	// First write in dynamic RAM fails for unknown reasons...
	// This is an hack which works, but still doesn't explain 
	// the cause...
	*((Xuint8 *)XPAR_DDR_SDRAM_MPMC_BASEADDR) = 0x42;
	
	XIo_Out32(XPAR_LED_BASEADDR, 1); //LED ON
	
	print("\r\n\r\nBootLoader v1.1\r\n\r\n");
	DBG(print ("Sending READ command... ");)
	SPI_CommandREAD (M25P32_BOOTSECTOR,0,0);
	DBG(print ("done.\r\n");)

	DBG(xil_printf ("Boot data header size is: %d\r\n",sizeof(BOOTDATAHEADER));)
	
	// Loading boot data header
	DBG(print ("Loading boot data header... ");)
	SPI_Receive ((unsigned char *)&bdh,sizeof(BOOTDATAHEADER));
	DBG(print ("done.\r\n");)
	
	DBG(xil_printf ("Boot data size is: 0x%08X\r\n",bdh.size);)
	DBG(xil_printf ("Boot jump address is: 0x%08X\r\n",bdh.boot_jump);)
	DBG(print ("Loading boot data... ");)
	destination_address = (Xuint8 *)bdh.boot_jump;
	SPI_Receive (destination_address,bdh.size);
	DBG(print ("done.\r\n");)

	DBG(print ("Sending READ command... ");)
	SPI_CommandREAD (M25P32_BOOTSECTOR,0,sizeof(BOOTDATAHEADER));
	DBG(print ("done.\r\n");)
	
	// Verify data in memory
	DBG(print ("Verifying boot data... ");)
	u16 crc = crc16(destination_address, bdh.size);
	DBG(xil_printf ("done. CRC = %d should be %d\r\n", crc, bdh.crc16);)
	if (crc != bdh.crc16) goto boot_error;

	// End SPI_flash communication
	SPI_DeselectDevices();
	
 	XIo_Out32(XPAR_LED_BASEADDR, 0); //LED OFF

/*	Xuint32 i;
	Xuint8 *memory = (Xuint8 *)0;
	int x;

	print ("Dumping memory contents from 0x00000000 to 0x00000040\r\n");
	for (i=0x00000000;i<0x00000040;i+=4) {
		xil_printf ("0x%08x: %02x %02x %02x %02x\r\n",i,memory[i],memory[i+1],memory[i+2],memory[i+3]);
	}*/
	
	DBG(print ("Changing vector table... ");)
	memcpy((void *)(0), (void *)bdh.vectors, sizeof(bdh.vectors));
	
/*	print ("Dumping memory contents from 0x00000000 to 0x00000040\r\n");
	for (i=0x00000000;i<0x00000040;i+=4) {
		xil_printf ("0x%08x: %02x %02x %02x %02x\r\n",i,memory[i],memory[i+1],memory[i+2],memory[i+3]);
	}*/

 	
	// prepare function pointer
	boot_app = (int (*) (void)) bdh.boot_jump;
	// jump to start execution code 
	boot_app();

boot_error:
	print ("Boot error.\r\n");
	
	while(1)
	{
		l++;
		if (l>2000000) {
			XIo_Out32(XPAR_LED_BASEADDR, LED); //LED blink
			LED = ~LED;
			l=0;
		}
	}
}
