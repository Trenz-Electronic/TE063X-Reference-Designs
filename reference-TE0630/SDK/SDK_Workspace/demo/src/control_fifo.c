#include "control_fifo.h"

//====================================================
//CONTROL FIFO_putchar function
//====================================================

void FIFO_ResetChar(FIFO_TYPE* fifo)
{
	fifo->FIFO_head = 0;
	fifo->FIFO_tail = 0;
	fifo->FIFO_count = 0;
}

//====================================================
//CONTROL FIFO_putchar function
//====================================================
void FIFO_putchar(FIFO_TYPE* fifo, Xuint8 c) 
{
    fifo->FIFO[fifo->FIFO_head] = c;
	 fifo->FIFO_count++;
	 fifo->FIFO_head++;
	 if (CONTROL_FIFO_SIZE <= (fifo->FIFO_head)) 
		fifo->FIFO_head = 0;
}

//====================================================
//CONTROL FIFO_getchar function
//====================================================
Xint16 FIFO_getchar(FIFO_TYPE* fifo) 
{
  Xuint8 c;
  if(fifo->FIFO_count > 0) 
  {
    c = fifo->FIFO[fifo->FIFO_tail];
	 fifo->FIFO_count--;
	 fifo->FIFO_tail++;
	 if (CONTROL_FIFO_SIZE <= (fifo->FIFO_tail)) 
		fifo->FIFO_tail = 0;
    return(c);
  }
  return(-1); //FIFO empty
}

