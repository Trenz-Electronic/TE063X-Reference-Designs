#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_

#include "xbasic_types.h"

void SPI_Send (Xuint8 *pdata, Xuint32 uiNumBytes);
void SPI_Receive (Xuint8 *pdata, Xuint32 uiNumBytes);
void SPI_CommandREAD (Xuint8 bySector, Xuint8 byPage, Xuint8 byByte);
void SPI_DeselectDevices (void);
void SPI_DeepPowerDown (void);
			
#endif /*SPI_FLASH_H_*/
