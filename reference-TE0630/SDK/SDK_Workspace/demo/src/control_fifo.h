#include "xbasic_types.h"

#define CONTROL_FIFO_SIZE     200
typedef struct
{
	Xuint8 FIFO[CONTROL_FIFO_SIZE];
	Xuint8 FIFO_head;
	Xuint8 FIFO_tail;
	Xuint8 FIFO_count;
} FIFO_TYPE;

//====================================================
//CONTROL FIFO_putchar function
//====================================================
void FIFO_putchar(FIFO_TYPE*, Xuint8);

//====================================================
//CONTROL FIFO_getchar function
//====================================================
Xint16 FIFO_getchar(FIFO_TYPE*);

//====================================================
//CONTROL FIFO_resetchar function
//====================================================
void FIFO_ResetChar(FIFO_TYPE*);
