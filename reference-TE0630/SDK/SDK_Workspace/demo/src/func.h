//prevent circular inclusions
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
void set_led_mode(Xuint8 mode);

#endif // HSSFUNC_H

