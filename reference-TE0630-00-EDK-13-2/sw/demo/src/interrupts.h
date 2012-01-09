//prevent circular inclusions
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "func.h"
#include "xps_i2c_slave.h"
#include "xio.h"
#include "xps_npi_dma.h"

#define GLOBDEBUG
#define INTDEBUG

#ifdef GLOBDEBUG
	#define DBG(x) x
#else
	#define DBG(x)
#endif

#ifdef INTDEBUGDEBUG
	#define DBGINT(x) x
#else
	#define DBGINT(x)
#endif

// firmware version 08.01.02.03
#define VERSION (0x08010203);

/* Constant Declaration*/
//device
#define FX22MB_REG0_NOP 0
#define FX22MB_REG0_GETVERSION 1
#define FX22MB_REG0_START_TX 2
#define FX22MB_REG0_START_RX 3
#define FX22MB_REG0_STOP 4
#define FX22MB_REG0_PING 5

FIFO_TYPE control_fifo;

void start_intc(void);
void i2c_slave_int_handler(void *baseaddr_p);

#endif // INTERRUPTS_H


