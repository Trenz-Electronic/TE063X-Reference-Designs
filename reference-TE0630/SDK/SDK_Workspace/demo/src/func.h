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
#ifndef HSSFUNC_H
#define HSSFUNC_H

#include "xbasic_types.h"
#include "xstatus.h"
#include "stdio.h"
#include "xio.h"
#include "xutil.h"
#include "xintc_l.h"
#include "xuartlite_l.h"
#include "mb_interface.h"
#include "xps_fx2.h"
#include "control_fifo.h"
#include "xtmrctr.h"
#include "xparameters.h"

void cycsleep(Xuint32 Cycles);
XStatus RAM_test(Xuint32 Start_addr, Xuint32 Words, Xuint8 Mode);
void check_control_fifo_overlow(FIFO_TYPE* ctrl_fifo);
void caching(Xuint8 Enable);

#endif // HSSFUNC_H

