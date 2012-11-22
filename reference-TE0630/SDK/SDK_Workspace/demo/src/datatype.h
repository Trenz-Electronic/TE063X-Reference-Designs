//prevent circular inclusions
#ifndef TEDATATYPE_H
#define TEDATATYPE_H

#include "control_fifo.h"

typedef struct
{
	Xuint32 FIFO_head;
	Xuint32 FIFO_tail;
	Xuint32 FIFO_count;
	Xuint32 FIFO_base;
	Xuint32 FIFO_words;
	Xuint32 FIFO_high;
	Xuint32 FIFO_last;
}RAM_TYPE;

typedef struct
{
	Xuint8 Major;
	Xuint8 Minor;
	Xuint8 Release;
	Xuint8 Build;
}TE_VERSION;

typedef struct
{
	Xuint16 Cols;
	Xuint16 Rows;
	Xuint16 StartRow;
	Xuint16 StopRow;
	Xuint16 StartCol;
	Xuint16 StopCol;
}IMAGING_PROP;

typedef struct
{
	Xuint16 PktSize;
	Xuint16 USBPktSize;
}PIXELS_PROP;

typedef struct
{
	Xuint16 PktSize;
	Xuint16 USBPktSize;
}PROFILES_PROP;

typedef struct
{
	Xuint8 xfer_run, imaging_run, loopback, read_only, fx2_fifo_empty, verify, ext_trig;
	Xuint32 LastCmd;//Last command
	Xuint8 acq_started;//1..PIXELS; 2..PROFILES
	IMAGING_PROP sensor;
	PIXELS_PROP Pixels;
	PROFILES_PROP Profiles;
}TE_PROP;

typedef struct
{
	RAM_TYPE Pixels_fifo;
	RAM_TYPE Profiles_fifo;
	TE_VERSION version;	
	TE_PROP prop;
	FIFO_TYPE control_fifo;
}MTE;

#endif // TEDATATYPE_H

