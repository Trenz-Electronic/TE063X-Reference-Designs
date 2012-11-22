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
#define VERSION (0x08030000);

/* Constant Declaration*/
//device
#define FX22MB_REG0_NOP 			0
#define FX22MB_REG0_GETVERSION		1
#define FX22MB_REG0_START_TX 		2
#define FX22MB_REG0_START_RX 		3
#define FX22MB_REG0_STOP 			4
#define FX22MB_REG0_PING 			5
#define FX22MB_REG0_TEST			6
#define FX22MB_REG0_PASSED			7
#define FX22MB_REG0_ERROR			8
#define FX22MB_REG0_GET_REVISION	9

FIFO_TYPE control_fifo;

void start_intc(void);
void i2c_slave_int_handler(void *baseaddr_p);

#endif // INTERRUPTS_H


