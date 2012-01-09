#include <stdio.h>
#include <string.h>
#include <cctype>

typedef unsigned char Xuint8;
typedef unsigned int Xuint32;

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

// function to convert 32bit int from little to BIG endian
unsigned int toBig32(unsigned int data)
{
	return  (data >> 24) | ((data >> 8) & 0x0000ff00) | ((data << 8) & 0x00ff0000) | (data << 24);
}

// function to convert 32bit int from little to BIG endian
unsigned short toBig16(unsigned short data)
{
	return  (data >> 8) | (data << 8);
}

// function to convert 32bit int from big to LITTLE endian
unsigned int toLittle32(unsigned int data)
{
	return  (data >> 24) | ((data >> 8) & 0x0000ff00) | ((data << 8) & 0x00ff0000) | (data << 24);
}

// Converts a hexadecimal string to integer
int xtoi(const char* xs, unsigned int* result)
{
 size_t szlen = strlen(xs);
 int i, xv, fact;

 if (szlen > 0)
 {
  // Converting more than 32bit hexadecimal value?
  if (szlen>8) return 2; // exit

  // Begin conversion here
  *result = 0;
  fact = 1;

  // Run until no more character to convert
  for(i=szlen-1; i>=0 ;i--)
  {
   if (isxdigit(*(xs+i)))
   {
    if (*(xs+i)>=97)
    {
     xv = ( *(xs+i) - 97) + 10;
    }
    else if ( *(xs+i) >= 65)
    {
     xv = (*(xs+i) - 65) + 10;
    }
    else
    {
     xv = *(xs+i) - 48;
    }
    *result += (xv * fact);
    fact *= 16;
   }
   else
   {
    // Conversion was abnormally terminated
    // by non hexadecimal digit, hence
    // returning only the converted with
    // an error value 4 (illegal hex character)
    return 4;
   }
  }
  	//Sucess
   return 0;
 }

 // Nothing to convert
 return 1;
}

int main(int argc, char *argv[]) {

	printf ("Created by Costantino Grana - University of Modena and Reggio Emilia, Italy\r\n");
	printf ("Edited by Ales Gorkic - Optomotive d.o.o., Slovenia\r\n\r\n");
	printf ("Usage: prepare bootdata <flash_offset>\r\n");
	printf ("       takes executable.bin and vectors.bin\r\n");
	printf ("       and creates bootdata.bin which is an executable with boot headear\r\n");

	FILE *pfIn=NULL,*pfOut=NULL,*pfVect=NULL;
	if (argc!=2) {
		printf ("Error: please specify a executable jump address\n");
		goto error;
	}
	if (fopen_s(&pfIn,"executable.bin","rb")!=0) {
		printf ("Error: cannot open input file\n");
		goto error;
	}
	if (fopen_s(&pfOut,"bootdata.bin","wb")!=0) {
		printf ("Error: cannot open bootdata.bin\n");
		goto error;
	}
	if (fopen_s(&pfVect,"vectors.bin","rb")!=0) {
		printf ("Error: cannot open _vectors.bin\n");
		goto error;
	}

	BOOTDATAHEADER bdh;

	//detect input length
	fseek (pfIn,0,SEEK_END);
	bdh.size = ftell(pfIn);
	//printf ("File size %d", bdh.size);
	//load input files to memory
	char *buffer = new char[bdh.size];
	fseek (pfIn,0,SEEK_SET);
	//int bytes = fread(buffer,1,bdh.size,pfIn);
	if (fread(buffer,1,bdh.size,pfIn)!=bdh.size) {
		printf ("Error: failed while reading input file\n");
		goto error;
	}
	//load vectors to memory
	char *vect_buffer = new char[40];
	if (fread(vect_buffer,1,40,pfVect)!=40) {
		printf ("Error: failed while reading vectors file\n");
		goto error;
	}

	//calculate CRC
	bdh.crc16 = crc16((void *) buffer, bdh.size);

	//CREATE HEADER

	// Convert to big endian
	bdh.size = toBig32(bdh.size);
	bdh.crc16 = toBig16(bdh.crc16);
	
	if (0 != xtoi((const char*) argv[1]+2, &bdh.boot_jump)) {
		printf ("Error: Incorrect boot_jump address\n");
		goto error;
	}
	printf ("boot_jump address: 0x%08X", bdh.boot_jump);

	bdh.boot_jump = toBig32(bdh.boot_jump);
	
	//copy vectors data
	memcpy(bdh.vectors,vect_buffer,40);
	
	unsigned int boot_vctr;
	memcpy(&boot_vctr, bdh.vectors, 4);
	printf ("Vect0: 0x%08X", boot_vctr);

	//write header to file
	fseek (pfIn,0,SEEK_SET);
	fwrite (&bdh,sizeof(bdh),1,pfOut);
	
	// Convert back to little endian
	bdh.size = toLittle32(bdh.size);

	//write output file
	if (fwrite(buffer,1,bdh.size,pfOut)!=bdh.size) {
		printf ("Error: failed while writing output file\n");
		goto error;
	}

end:
	fclose (pfVect);
	fclose (pfIn);
	fclose (pfOut);
	return 0;

error:
	if (pfVect!=NULL) fclose (pfVect);
	if (pfIn!=NULL) fclose (pfIn);
	if (pfOut!=NULL) fclose (pfOut);
	return 1;
}